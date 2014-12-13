#include "DXUT.h"
#include <cmath>
#include "fp_camera.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"

//コンストラクタ
CFPCamera::CFPCamera()
{
	//初期化
	Init();
}

//初期化
void CFPCamera::Init()
{
	//CSV
	m_Csv = CCsv::Create( "Content/csv/FPCameraData.csv" );

	//Y軸移動量
	m_JumpMovement = 0.0f;
    
	//傾き度合い
    m_AccumPitchDegrees = 0.0f;
	
	//回転速度
	m_RotationSpeed = m_Csv->GetToken< float >( 0 , 0 );

	//初期位置
    SetPosition( 0 , m_Csv->GetToken< float >( 1 , 0 ) , 0 );

	//X座標
	m_XAxis = D3DXVECTOR3( 1.0f , 0.0f , 0.0f );
    
	//Y座標
	m_YAxis = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );
    
	//Z座標
	m_ZAxis = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
    
	//視野方向
	m_ViewDir = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
    
	//流速
	m_CurrentVelocity = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//射影変換	 //画角(x)		//アスペクト比 //近接限界 //遠隔限界
	Perspective( m_Csv->GetToken< float >( 2 , 0 ) , 1.0f , m_Csv->GetToken< float >( 3 , 0 ) , m_Csv->GetToken< float >( 4 , 0 ) );

	//初期加速度
	m_Acceleration = D3DXVECTOR3( m_Csv->GetToken< float >( 5 , 0 ) , m_Csv->GetToken< float >( 5 , 1 ) , m_Csv->GetToken< float >( 5 , 2 ) );

	//初期速度
	m_Velocity = D3DXVECTOR3( m_Csv->GetToken< float >( 6 , 0 ) , m_Csv->GetToken< float >( 6 , 1 ) , m_Csv->GetToken< float >( 6 , 2 ) );

	//最大限界
	m_BoundsMax.x = m_Csv->GetToken< float >( 7 , 0 );
	m_BoundsMax.y = m_Csv->GetToken< float >( 7 , 1 );
	m_BoundsMax.z = m_Csv->GetToken< float >( 7 , 2 );

	//最小限界
	m_BoundsMin.x = m_Csv->GetToken< float >( 8 , 0 );
	m_BoundsMin.y = m_Csv->GetToken< float >( 8 , 1 );
	m_BoundsMin.z = m_Csv->GetToken< float >( 8 , 2 );

	//方位	
    D3DXQuaternionIdentity( &m_Orientation );

	//重力フラグ
	m_IsGravity = true;
}

//カメラ更新
void CFPCamera::UpdateCamera( const float elapsedTimeSec )
{
	//前のフレームの座標を保存
	m_PrevEye = m_Eye;

	//向き
    D3DXVECTOR3 direction;
	//移動方向
	GetMovementDirection( direction );

	//x軸で回転する方向
	float pitch = MOUSE->yPosRelative() * GetRotationSpeed();
	//y軸で回転する方向
	float heading = MOUSE->xPosRelative() * GetRotationSpeed();
	//回転
	Rotate( heading , pitch , 0.0f );

	//座標更新
	UpdatePosition( direction , elapsedTimeSec );

	//挙動の制限
	PerformCameraCollisionDetection();

	//地面接地フラグをオフに
	m_IsOnGround = false;
}

//移動方向
void CFPCamera::GetMovementDirection( D3DXVECTOR3& direction )
{
	//-------------移動キーフラグ-------------
	//前
	static bool moveForwardsPressed = false;
	//後ろ
	static bool moveBackwardsPressed = false;
	//右
	static bool moveRightPressed = false;
	//左
	static bool moveLeftPressed = false;
	//上
	static bool moveUpPressed = false;
	//下
	static bool moveDownPressed = false;

	//速さを保存
	D3DXVECTOR3 velocity = GetCurrentVelocity();

	//向き初期化
	direction.x = direction.y = direction.z = 0.0f;

	//------------------------------------移動------------------------------------
	//前
	if( K_B->KeyDown( K_B->KEY_UP ) || K_B->KeyDown( K_B->KEY_W ) )
	{
		if( !moveForwardsPressed )
		{
			moveForwardsPressed = true;
			//z軸以外の流速設定
			m_CurrentVelocity = D3DXVECTOR3( velocity.x , velocity.y , 0.0f );
		}

		//前へ(z軸)
		direction.z += 1.0f;
	}
	else
	{
		moveForwardsPressed = false;
	}

	//後ろ
	if( K_B->KeyDown( K_B->KEY_DOWN ) || K_B->KeyDown( K_B->KEY_S ) )
	{
		if( !moveBackwardsPressed )
		{
			moveBackwardsPressed = true;
			//z軸以外の流速設定
			m_CurrentVelocity = D3DXVECTOR3( velocity.x , velocity.y , 0.0f );
		}

		//後ろへ(z軸)
		direction.z -= 1.0f;
	}
	else
	{
		moveBackwardsPressed = false;
	}

	//右
	if( K_B->KeyDown( K_B->KEY_RIGHT ) || K_B->KeyDown( K_B->KEY_D ) )
	{
		if( !moveRightPressed )
		{
			moveRightPressed = true;
			//x軸以外の流速設定
			m_CurrentVelocity = D3DXVECTOR3( 0.0f , velocity.y , velocity.z );
		}

		//右へ(x軸)
		direction.x += 1.0f;
	}
	else
	{
		moveRightPressed = false;
	}

	//左
	if( K_B->KeyDown( K_B->KEY_LEFT ) || K_B->KeyDown( K_B->KEY_A ) )
	{
		if(!moveLeftPressed)
		{
			moveLeftPressed = true;
			//x軸以外の流速設定
			m_CurrentVelocity = D3DXVECTOR3( 0.0f , velocity.y , velocity.z );
		}

		//左へ(x軸)
		direction.x -= 1.0f;
	}
	else
	{
		moveLeftPressed = false;
	}

	//重力がかかっている状態なら以下の処理を行わない
	if( m_IsGravity == true ) return;

	//上
	if( K_B->KeyDown( K_B->KEY_E ) )
	{
		if( !moveUpPressed )
		{
			moveUpPressed = true;
			//y軸以外の流速設定
			m_CurrentVelocity = D3DXVECTOR3( velocity.x , 0.0f , velocity.z );
		}

		//上へ(y軸)
		direction.y += 1.0f;
	}
	else
	{
		moveUpPressed = false;
	}

	//下
	if( K_B->KeyDown( K_B->KEY_Q ) )
	{
		if( !moveDownPressed )
		{
			moveDownPressed = true;
			//y軸以外の流速設定
			m_CurrentVelocity = D3DXVECTOR3( velocity.x , 0.0f , velocity.z );
		}

		//下へ(y軸)
		direction.y -= 1.0f;
	}
	else
	{
		moveDownPressed = false;
	}
}

//回転
void CFPCamera::Rotate( float headingDegrees , float pitchDegrees , float rollDegrees )
{
	//マウスが進んだ方に回転するようにする
    pitchDegrees = -pitchDegrees;
    headingDegrees = -headingDegrees;

	//一人称回転
	RotateFirstPerson( headingDegrees , pitchDegrees );

    //ビュー行列更新
	UpdateViewMatrix();
}

//一人称回転
void CFPCamera::RotateFirstPerson( float headingDegrees , float pitchDegrees )
{
	//傾き度合い
    m_AccumPitchDegrees += pitchDegrees;

	//回転制御
    if( m_AccumPitchDegrees > 90.0f )
	{
        pitchDegrees = 90.0f - ( m_AccumPitchDegrees - pitchDegrees );
        m_AccumPitchDegrees = 90.0f;
    }

	//回転制御
    if( m_AccumPitchDegrees < -90.0f )
	{
        pitchDegrees = -90.0f - ( m_AccumPitchDegrees - pitchDegrees );
        m_AccumPitchDegrees = -90.0f;
    }

	//ラジアンへ変換
    float heading = D3DXToRadian( headingDegrees );
    float pitch = D3DXToRadian( pitchDegrees );

    D3DXQUATERNION rot;

	//ワールドy軸周りにカメラを回転
    if( heading != 0.0f )
	{
        D3DXQuaternionRotationAxis( &rot , &WORLD_YAXIS , heading );
        D3DXQuaternionMultiply( &m_Orientation , &rot , &m_Orientation );
    }

    //ローカルx軸の周りにカメラを回転
    if( pitch != 0.0f )
	{
		D3DXQuaternionRotationAxis( &rot , &D3DXVECTOR3( 1.0f , 0.0f , 0.0f ) , pitch );
        D3DXQuaternionMultiply( &m_Orientation , &m_Orientation , &rot );
    }
}

//ビュー行列更新
void CFPCamera::UpdateViewMatrix()
{
	//クォータニオンの正規化
    D3DXQuaternionNormalize( &m_Orientation , &m_Orientation );
	//クォータニオンから回転行列を作成
    D3DXMatrixRotationQuaternion( &m_ViewMatrix , &m_Orientation );

    m_XAxis = D3DXVECTOR3( m_ViewMatrix( 0 , 0 ) , m_ViewMatrix( 1 , 0 ) , m_ViewMatrix( 2 , 0 ) );
    m_YAxis = D3DXVECTOR3( m_ViewMatrix( 0 , 1 ) , m_ViewMatrix( 1 , 1 ) , m_ViewMatrix( 2 , 1 ) );
    m_ZAxis = D3DXVECTOR3( m_ViewMatrix( 0 , 2 ) , m_ViewMatrix( 1 , 2 ) , m_ViewMatrix( 2 , 2 ) );
    m_ViewDir = m_ZAxis;

							   //2つのベクトルの内積
    //m_ViewMatrix( 3 , 0 ) = -D3DXVec3Dot( &m_XAxis , &m_Eye );
    //m_ViewMatrix( 3 , 1 ) = -D3DXVec3Dot( &m_YAxis , &m_Eye );
    //m_ViewMatrix( 3 , 2 ) = -D3DXVec3Dot( &m_ZAxis , &m_Eye );

	//注視点
	m_Target = m_Eye + m_ViewDir;//D3DXVECTOR3( m_XAxis.x , m_YAxis.y , m_ZAxis.z );

	//ビュー行列作成(カメラの配置)
	D3DXMatrixLookAtLH( &m_ViewMatrix , &m_Eye , &m_Target , &WORLD_YAXIS );
	
	//DirectXに設定(変換を反映する)
	CGraphicsManager::m_pD3dDevice->SetTransform( D3DTS_VIEW, &m_ViewMatrix );
}

//位置更新
void CFPCamera::UpdatePosition( const D3DXVECTOR3& direction , const float elapsedTimeSec )
{
    //方向ベクトルは範囲は1から-1

	//転移量
    D3DXVECTOR3 displacement( 0 , 0 , 0 );

	if( m_IsGravity == true )
	{
		//ジャンプ
		Jump( elapsedTimeSec );
	}

	//流速が0で無い時のみカメラを移動する
    if( D3DXVec3LengthSq( &m_CurrentVelocity ) != 0.0f )
	//if( m_CurrentVelocity )
	{
		//転移量
		displacement.x = m_CurrentVelocity.x * m_Acceleration.x * elapsedTimeSec;
		displacement.y = m_CurrentVelocity.y * m_Acceleration.y * elapsedTimeSec;
		displacement.z = m_CurrentVelocity.z * m_Acceleration.z * elapsedTimeSec;

		//カメラが意図しない動きをしないように防ぐ(各軸に沿って移動する)
		if( direction.x == 0.0f && fabsf( m_CurrentVelocity.x ) < 1e-6f ) displacement.x = 0.0f;

		if( direction.y == 0.0f && fabsf( m_CurrentVelocity.y ) < 1e-6f ) displacement.y = 0.0f;

        if( direction.z == 0.0f && fabsf( m_CurrentVelocity.z ) < 1e-6f ) displacement.z = 0.0f;

		Move( displacement.x , displacement.y , displacement.z );
    }

	//速度更新
	UpdateVelocity( direction , elapsedTimeSec );
}

//ジャンプ
void CFPCamera::Jump( const float elapsedTimeSec )
{
	//ジャンプ開始
	if( m_IsOnGround == true && K_B->KeyPressed( K_B->KEY_SPACE ) )
	{
		//地面接地フラグをオフに
		m_IsOnGround = false;

		//ジャンプ力を設定
		m_JumpMovement = 0.5f;
	}

	//重力
	if( m_IsOnGround == false )
	{
		m_JumpMovement -= 0.02f;
		//m_JumpMovement -= elapsedTimeSec * 0.98f;
		m_Eye.y += m_JumpMovement;
	}

	//y移動量の制限
	if( m_JumpMovement < -1.0f ) m_JumpMovement = -1.0f;
}

//カメラ移動←こっちを使う
void CFPCamera::Move( const float dx , const float dy , const float dz )
{
	//カメラ座標
	D3DXVECTOR3 eye = m_Eye;

	//前方
	D3DXVECTOR3 forwards;

	//移動方向を計算
	//カメラのX軸とワールドY軸(m_YAxisじゃない)の外積で進む方向(前方)のベクトルを出す
	D3DXVec3Cross( &forwards , &m_XAxis , &WORLD_YAXIS );

	//ベクトルの正規化
	D3DXVec3Normalize( &forwards , &forwards );

	//dxによって左右にカメラを移動
	eye += m_XAxis * dx;

	//dyによって上下にカメラを移動
	eye += WORLD_YAXIS * dy;

	//dzによって前後にカメラを移動
	eye += forwards * dz;

	//座標を設定(m_Eyeとビューマトリックス更新)
	SetPosition( eye );
}

//カメラ移動
void CFPCamera::Move( const D3DXVECTOR3& direction , const D3DXVECTOR3& amount )
{
	//ワールド座標で指定された方向に指定された量だけ移動させる
    m_Eye.x += direction.x * amount.x;
    m_Eye.y += direction.y * amount.y;
    m_Eye.z += direction.z * amount.z;

	//ビュー行列を更新
    UpdateViewMatrix();
}

//速度更新
void CFPCamera::UpdateVelocity( const D3DXVECTOR3& direction , float elapsedTimeSec )
{
	//方向の範囲は1から-1

	//x軸
    if( direction.x != 0.0f )
	{
        m_CurrentVelocity.x += direction.x * m_Acceleration.x * elapsedTimeSec;

		//加速
        if( m_CurrentVelocity.x > m_Velocity.x )
		{
			m_CurrentVelocity.x = m_Velocity.x;
		}
		else if( m_CurrentVelocity.x < -m_Velocity.x )
		{
            m_CurrentVelocity.x = -m_Velocity.x;
		}
    }
	else
	{
        //減速
        if( m_CurrentVelocity.x > 0.0f )
		{
            if( ( m_CurrentVelocity.x -= m_Acceleration.x * elapsedTimeSec ) < 0.0f ) m_CurrentVelocity.x = 0.0f;
		}
		else
		{
            if( ( m_CurrentVelocity.x += m_Acceleration.x * elapsedTimeSec ) > 0.0f ) m_CurrentVelocity.x = 0.0f;
        }
    }

	//y軸
    if( direction.y != 0.0f )
	{
        m_CurrentVelocity.y += direction.y * m_Acceleration.y * elapsedTimeSec;
        
		//加速
        if( m_CurrentVelocity.y > m_Velocity.y )
		{
			m_CurrentVelocity.y = m_Velocity.y;
		}
		else if( m_CurrentVelocity.y < -m_Velocity.y )
		{
            m_CurrentVelocity.y = -m_Velocity.y;
		}
    }
	else
	{
        //減速
        if( m_CurrentVelocity.y > 0.0f )
		{
            if( ( m_CurrentVelocity.y -= m_Acceleration.y * elapsedTimeSec ) < 0.0f ) m_CurrentVelocity.y = 0.0f;
        }
		else
		{
            if( ( m_CurrentVelocity.y += m_Acceleration.y * elapsedTimeSec ) > 0.0f ) m_CurrentVelocity.y = 0.0f;
        }
    }

	//z軸
    if( direction.z != 0.0f )
	{
        m_CurrentVelocity.z += direction.z * m_Acceleration.z * elapsedTimeSec * 2;

		//加速
        if( m_CurrentVelocity.z > m_Velocity.z )
		{
            m_CurrentVelocity.z = m_Velocity.z;
		}
		else if( m_CurrentVelocity.z < -m_Velocity.z )
		{
            m_CurrentVelocity.z = -m_Velocity.z;
		}
    }
	else
	{
        //減速
        if( m_CurrentVelocity.z > 0.0f )
		{
            if( ( m_CurrentVelocity.z -= m_Acceleration.z * elapsedTimeSec ) < 0.0f ) m_CurrentVelocity.z = 0.0f;
        }
		else
		{
            if( ( m_CurrentVelocity.z += m_Acceleration.z * elapsedTimeSec ) > 0.0f ) m_CurrentVelocity.z = 0.0f;
        }
    }
}

//挙動の制限
void CFPCamera::PerformCameraCollisionDetection()
{
	//現在の座標
    const D3DXVECTOR3& pos = GetPosition();
	//新しい座標
    D3DXVECTOR3 newPos( pos );

	//------------限界の値を超えないように補正------------
    if( pos.x > m_BoundsMax.x ) newPos.x = m_BoundsMax.x;

    if( pos.x < m_BoundsMin.x ) newPos.x = m_BoundsMin.x;

    if( pos.y > m_BoundsMax.y ) newPos.y = m_BoundsMax.y;

    if( pos.y < m_BoundsMin.y ) newPos.y = m_BoundsMin.y;

    if( pos.z > m_BoundsMax.z ) newPos.z = m_BoundsMax.z;

    if( pos.z < m_BoundsMin.z ) newPos.z = m_BoundsMin.z;

	//座標を設定
    SetPosition( newPos );
}

//遠近投影
void CFPCamera::Perspective( const float fovx , const float aspect , const float znear , const float zfar )
{
	//射影変換(カメラのパラメータ設定)			//視野角 //アスペクト比 //最近点 //最遠点
	D3DXMatrixPerspectiveFovLH( &m_ProjMatrix , fovx , aspect , znear , zfar );
	
	//DirectXに設定(変換を反映する)
	CGraphicsManager::m_pD3dDevice->SetTransform( D3DTS_PROJECTION , &m_ProjMatrix );
}
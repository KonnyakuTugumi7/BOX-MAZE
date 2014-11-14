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
	m_csv = CCsv::Create( "Content/csv/FPCameraData.csv" );

	//Y軸移動量
	m_jump_movement = 0.0f;
    
	//傾き度合い
    m_accumPitchDegrees = 0.0f;
	
	//回転速度
	m_rotationSpeed = m_csv->GetToken< float >( 0 , 0 );

	//初期位置
    SetPosition( 0 , m_csv->GetToken< float >( 1 , 0 ) , 0 );

	//X座標
	m_xAxis = D3DXVECTOR3( 1.0f , 0.0f , 0.0f );
    
	//Y座標
	m_yAxis = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );
    
	//Z座標
	m_zAxis = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
    
	//視野方向
	m_viewDir = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
    
	//流速
	m_currentVelocity = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//射影変換	 //画角(x)		//アスペクト比 //近接限界 //遠隔限界
	Perspective( m_csv->GetToken< float >( 2 , 0 ) , 1.0f , m_csv->GetToken< float >( 3 , 0 ) , m_csv->GetToken< float >( 4 , 0 ) );

	//初期加速度
	m_acceleration = D3DXVECTOR3( m_csv->GetToken< float >( 5 , 0 ) , m_csv->GetToken< float >( 5 , 1 ) , m_csv->GetToken< float >( 5 , 2 ) );

	//初期速度
	m_velocity = D3DXVECTOR3( m_csv->GetToken< float >( 6 , 0 ) , m_csv->GetToken< float >( 6 , 1 ) , m_csv->GetToken< float >( 6 , 2 ) );

	//最大限界
	m_boundsmax.x = m_csv->GetToken< float >( 7 , 0 );
	m_boundsmax.y = m_csv->GetToken< float >( 7 , 1 );
	m_boundsmax.z = m_csv->GetToken< float >( 7 , 2 );

	//最小限界
	m_boundsmin.x = m_csv->GetToken< float >( 8 , 0 );
	m_boundsmin.y = m_csv->GetToken< float >( 8 , 1 );
	m_boundsmin.z = m_csv->GetToken< float >( 8 , 2 );

	//方位	
    D3DXQuaternionIdentity( &m_orientation );

	//重力フラグ
	m_is_gravity = true;
}

//カメラ更新
void CFPCamera::UpdateCamera( const float elapsedTimeSec )
{
	//前のフレームの座標を保存
	m_prev_eye = m_eye;

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
	m_is_on_ground = false;
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
			m_currentVelocity = D3DXVECTOR3( velocity.x , velocity.y , 0.0f );
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
			m_currentVelocity = D3DXVECTOR3( velocity.x , velocity.y , 0.0f );
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
			m_currentVelocity = D3DXVECTOR3( 0.0f , velocity.y , velocity.z );
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
			m_currentVelocity = D3DXVECTOR3( 0.0f , velocity.y , velocity.z );
		}

		//左へ(x軸)
		direction.x -= 1.0f;
	}
	else
	{
		moveLeftPressed = false;
	}

	//重力がかかっている状態なら以下の処理を行わない
	if( m_is_gravity == true ) return;

	//上
	if( K_B->KeyDown( K_B->KEY_E ) )
	{
		if( !moveUpPressed )
		{
			moveUpPressed = true;
			//y軸以外の流速設定
			m_currentVelocity = D3DXVECTOR3( velocity.x , 0.0f , velocity.z );
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
			m_currentVelocity = D3DXVECTOR3( velocity.x , 0.0f , velocity.z );
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
    m_accumPitchDegrees += pitchDegrees;

	//回転制御
    if( m_accumPitchDegrees > 90.0f )
	{
        pitchDegrees = 90.0f - ( m_accumPitchDegrees - pitchDegrees );
        m_accumPitchDegrees = 90.0f;
    }

	//回転制御
    if( m_accumPitchDegrees < -90.0f )
	{
        pitchDegrees = -90.0f - ( m_accumPitchDegrees - pitchDegrees );
        m_accumPitchDegrees = -90.0f;
    }

	//ラジアンへ変換
    float heading = D3DXToRadian( headingDegrees );
    float pitch = D3DXToRadian( pitchDegrees );

    D3DXQUATERNION rot;

	//ワールドy軸周りにカメラを回転
    if( heading != 0.0f )
	{
        D3DXQuaternionRotationAxis( &rot , &WORLD_YAXIS , heading );
        D3DXQuaternionMultiply( &m_orientation , &rot , &m_orientation );
    }

    //ローカルx軸の周りにカメラを回転
    if( pitch != 0.0f )
	{
		D3DXQuaternionRotationAxis( &rot , &D3DXVECTOR3( 1.0f , 0.0f , 0.0f ) , pitch );
        D3DXQuaternionMultiply( &m_orientation , &m_orientation , &rot );
    }
}

//ビュー行列更新
void CFPCamera::UpdateViewMatrix()
{
	//クォータニオンの正規化
    D3DXQuaternionNormalize( &m_orientation , &m_orientation );
	//クォータニオンから回転行列を作成
    D3DXMatrixRotationQuaternion( &m_viewMatrix , &m_orientation );

    m_xAxis = D3DXVECTOR3( m_viewMatrix( 0 , 0 ) , m_viewMatrix( 1 , 0 ) , m_viewMatrix( 2 , 0 ) );
    m_yAxis = D3DXVECTOR3( m_viewMatrix( 0 , 1 ) , m_viewMatrix( 1 , 1 ) , m_viewMatrix( 2 , 1 ) );
    m_zAxis = D3DXVECTOR3( m_viewMatrix( 0 , 2 ) , m_viewMatrix( 1 , 2 ) , m_viewMatrix( 2 , 2 ) );
    m_viewDir = m_zAxis;

							   //2つのベクトルの内積
    //m_viewMatrix( 3 , 0 ) = -D3DXVec3Dot( &m_xAxis , &m_eye );
    //m_viewMatrix( 3 , 1 ) = -D3DXVec3Dot( &m_yAxis , &m_eye );
    //m_viewMatrix( 3 , 2 ) = -D3DXVec3Dot( &m_zAxis , &m_eye );

	//注視点
	m_target = m_eye + m_viewDir;//D3DXVECTOR3( m_xAxis.x , m_yAxis.y , m_zAxis.z );

	//ビュー行列作成(カメラの配置)
	D3DXMatrixLookAtLH( &m_viewMatrix , &m_eye , &m_target , &WORLD_YAXIS );
	
	//DirectXに設定(変換を反映する)
	CCommon::m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_viewMatrix );
}

//位置更新
void CFPCamera::UpdatePosition( const D3DXVECTOR3& direction , const float elapsedTimeSec )
{
    //方向ベクトルは範囲は1から-1

	//転移量
    D3DXVECTOR3 displacement( 0 , 0 , 0 );

	if( m_is_gravity == true )
	{
		//ジャンプ
		Jump( elapsedTimeSec );
	}

	//流速が0で無い時のみカメラを移動する
    if( D3DXVec3LengthSq( &m_currentVelocity ) != 0.0f )
	//if( m_currentVelocity )
	{
		//転移量
		displacement.x = m_currentVelocity.x * m_acceleration.x * elapsedTimeSec;
		displacement.y = m_currentVelocity.y * m_acceleration.y * elapsedTimeSec;
		displacement.z = m_currentVelocity.z * m_acceleration.z * elapsedTimeSec;

		//カメラが意図しない動きをしないように防ぐ(各軸に沿って移動する)
		if( direction.x == 0.0f && fabsf( m_currentVelocity.x ) < 1e-6f ) displacement.x = 0.0f;

		if( direction.y == 0.0f && fabsf( m_currentVelocity.y ) < 1e-6f ) displacement.y = 0.0f;

        if( direction.z == 0.0f && fabsf( m_currentVelocity.z ) < 1e-6f ) displacement.z = 0.0f;

		Move( displacement.x , displacement.y , displacement.z );
    }

	//速度更新
	UpdateVelocity( direction , elapsedTimeSec );
}

//ジャンプ
void CFPCamera::Jump( const float elapsedTimeSec )
{
	//ジャンプ開始
	if( m_is_on_ground == true && K_B->KeyPressed( K_B->KEY_SPACE ) )
	{
		//地面接地フラグをオフに
		m_is_on_ground = false;

		//ジャンプ力を設定
		m_jump_movement = 0.5f;
	}

	//重力
	if( m_is_on_ground == false )
	{
		m_jump_movement -= 0.02f;
		//m_jump_movement -= elapsedTimeSec * 0.98f;
		m_eye.y += m_jump_movement;
	}

	//y移動量の制限
	if( m_jump_movement < -1.0f ) m_jump_movement = -1.0f;
}

//カメラ移動←こっちを使う
void CFPCamera::Move( const float dx , const float dy , const float dz )
{
	//カメラ座標
	D3DXVECTOR3 eye = m_eye;

	//前方
	D3DXVECTOR3 forwards;

	//移動方向を計算
	//カメラのX軸とワールドY軸(m_yAxisじゃない)の外積で進む方向(前方)のベクトルを出す
	D3DXVec3Cross( &forwards , &m_xAxis , &WORLD_YAXIS );

	//ベクトルの正規化
	D3DXVec3Normalize( &forwards , &forwards );

	//dxによって左右にカメラを移動
	eye += m_xAxis * dx;

	//dyによって上下にカメラを移動
	eye += WORLD_YAXIS * dy;

	//dzによって前後にカメラを移動
	eye += forwards * dz;

	//座標を設定(m_eyeとビューマトリックス更新)
	SetPosition( eye );
}

//カメラ移動
void CFPCamera::Move( const D3DXVECTOR3& direction , const D3DXVECTOR3& amount )
{
	//ワールド座標で指定された方向に指定された量だけ移動させる
    m_eye.x += direction.x * amount.x;
    m_eye.y += direction.y * amount.y;
    m_eye.z += direction.z * amount.z;

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
        m_currentVelocity.x += direction.x * m_acceleration.x * elapsedTimeSec;

		//加速
        if( m_currentVelocity.x > m_velocity.x )
		{
			m_currentVelocity.x = m_velocity.x;
		}
		else if( m_currentVelocity.x < -m_velocity.x )
		{
            m_currentVelocity.x = -m_velocity.x;
		}
    }
	else
	{
        //減速
        if( m_currentVelocity.x > 0.0f )
		{
            if( ( m_currentVelocity.x -= m_acceleration.x * elapsedTimeSec ) < 0.0f ) m_currentVelocity.x = 0.0f;
		}
		else
		{
            if( ( m_currentVelocity.x += m_acceleration.x * elapsedTimeSec ) > 0.0f ) m_currentVelocity.x = 0.0f;
        }
    }

	//y軸
    if( direction.y != 0.0f )
	{
        m_currentVelocity.y += direction.y * m_acceleration.y * elapsedTimeSec;
        
		//加速
        if( m_currentVelocity.y > m_velocity.y )
		{
			m_currentVelocity.y = m_velocity.y;
		}
		else if( m_currentVelocity.y < -m_velocity.y )
		{
            m_currentVelocity.y = -m_velocity.y;
		}
    }
	else
	{
        //減速
        if( m_currentVelocity.y > 0.0f )
		{
            if( ( m_currentVelocity.y -= m_acceleration.y * elapsedTimeSec ) < 0.0f ) m_currentVelocity.y = 0.0f;
        }
		else
		{
            if( ( m_currentVelocity.y += m_acceleration.y * elapsedTimeSec ) > 0.0f ) m_currentVelocity.y = 0.0f;
        }
    }

	//z軸
    if( direction.z != 0.0f )
	{
        m_currentVelocity.z += direction.z * m_acceleration.z * elapsedTimeSec * 2;

		//加速
        if( m_currentVelocity.z > m_velocity.z )
		{
            m_currentVelocity.z = m_velocity.z;
		}
		else if( m_currentVelocity.z < -m_velocity.z )
		{
            m_currentVelocity.z = -m_velocity.z;
		}
    }
	else
	{
        //減速
        if( m_currentVelocity.z > 0.0f )
		{
            if( ( m_currentVelocity.z -= m_acceleration.z * elapsedTimeSec ) < 0.0f ) m_currentVelocity.z = 0.0f;
        }
		else
		{
            if( ( m_currentVelocity.z += m_acceleration.z * elapsedTimeSec ) > 0.0f ) m_currentVelocity.z = 0.0f;
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
    if( pos.x > m_boundsmax.x ) newPos.x = m_boundsmax.x;

    if( pos.x < m_boundsmin.x ) newPos.x = m_boundsmin.x;

    if( pos.y > m_boundsmax.y ) newPos.y = m_boundsmax.y;

    if( pos.y < m_boundsmin.y ) newPos.y = m_boundsmin.y;

    if( pos.z > m_boundsmax.z ) newPos.z = m_boundsmax.z;

    if( pos.z < m_boundsmin.z ) newPos.z = m_boundsmin.z;

	//座標を設定
    SetPosition( newPos );
}

//遠近投影
void CFPCamera::Perspective( const float fovx , const float aspect , const float znear , const float zfar )
{
	//射影変換(カメラのパラメータ設定)			//視野角 //アスペクト比 //最近点 //最遠点
	D3DXMatrixPerspectiveFovLH( &m_projMatrix , fovx , aspect , znear , zfar );
	
	//DirectXに設定(変換を反映する)
	CGraphicsManager::m_pd3dDevice->SetTransform( D3DTS_PROJECTION , &m_projMatrix );
}
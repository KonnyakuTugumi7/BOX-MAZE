#include "DXUT.h"
#include <cmath>
#include "tp_camera.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"

//コンストラクタ
CTPCamera::CTPCamera()
{
	//CSV
	m_csv = CCsv::Create( "Content/csv/TPCameraData.csv" );

	//経度
    m_longitudeDegrees = 0.0f;

	//緯度
    m_latitudeDegrees = 0.0f;

	//カメラと注視点のオフセット
    m_offset = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//回転速度
	m_rot_speed = m_csv->GetToken< float >( 5 , 0 );

	//射影変換
	Perspective( m_csv->GetToken< float >( 0 , 0 ) , 1.0f , m_csv->GetToken< float >( 1 , 0 ) , m_csv->GetToken< float >( 2 , 0 ) );

	//注視点から移動
	LookAt( D3DXVECTOR3( 0.0f , /*m_csv->GetToken< float >( 3 , 0 )*/0 , m_csv->GetToken< float >( 3 , 1 ) ) , D3DXVECTOR3( 0.0f , /*m_csv->GetToken< float >( 4 , 0 )*/0 , 0 ) , D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );
}

//注視点
void CTPCamera::LookAt( const D3DXVECTOR3& target )
{
    m_target = target;
}

//注視点から移動
void CTPCamera::LookAt( const D3DXVECTOR3& eye , const D3DXVECTOR3& target , const D3DXVECTOR3& up )
{
	//カメラ座標とターゲットをセット
    m_eye = eye;
    m_target = target;

    m_zAxis = target - eye;
    m_offset.x = -m_zAxis.x;
    m_offset.y = -m_zAxis.y;
    m_offset.z = -m_zAxis.z;
    D3DXVec3Normalize( &m_zAxis , &m_zAxis );

	//x軸方向のベクトルを求める
    D3DXVec3Cross( &m_xAxis, &up , &m_zAxis );
    D3DXVec3Normalize( &m_xAxis , &m_xAxis );

	//y軸方向のベクトルを求める
    D3DXVec3Cross( &m_yAxis , &m_zAxis , &m_xAxis );
    D3DXVec3Normalize( &m_yAxis , &m_yAxis );
    D3DXVec3Normalize( &m_xAxis , &m_xAxis );

    D3DXMatrixIdentity( &m_viewMatrix );

	////ビュー行列更新
 //   m_viewMatrix( 0 , 0 ) = m_xAxis.x;
 //   m_viewMatrix( 1 , 0 ) = m_xAxis.y;
 //   m_viewMatrix( 2 , 0 ) = m_xAxis.z;
 //   m_viewMatrix( 3 , 0 ) = -D3DXVec3Dot( &m_xAxis , &eye );

 //   m_viewMatrix( 0 , 1 ) = m_yAxis.x;
 //   m_viewMatrix( 1 , 1 ) = m_yAxis.y;
 //   m_viewMatrix( 2 , 1 ) = m_yAxis.z;
 //   m_viewMatrix( 3 , 1 ) = -D3DXVec3Dot( &m_yAxis, &eye );

 //   m_viewMatrix( 0 , 2 ) = m_zAxis.x;
 //   m_viewMatrix( 1 , 2 ) = m_zAxis.y;
 //   m_viewMatrix( 2 , 2 ) = m_zAxis.z;
 //   m_viewMatrix( 3 , 2 ) = -D3DXVec3Dot( &m_zAxis , &eye );

	//視野方向を設定
	m_viewDir = m_zAxis;

	//ビュー行列更新
	D3DXMatrixLookAtLH( &m_viewMatrix , &m_eye , &m_target , &up );

    D3DXQuaternionRotationMatrix( &m_orientation , &m_viewMatrix );
}

//遠近投影
void CTPCamera::Perspective( const float fovx , const float aspect , const float znear , const float zfar )
{
    float e = 1.0f / tanf( D3DXToRadian( fovx ) / 2.0f );
    float aspectInv = 1.0f / aspect;
    float fovy = 2.0f * atanf( aspectInv / e );
    //float xScale = 1.0f / tanf( 0.5f * fovy );
    //float yScale = xScale / aspectInv;

	//射影変換
	D3DXMatrixPerspectiveFovLH( &m_projMatrix , fovy , aspect , znear , zfar );
	CGraphicsManager::m_pd3dDevice->SetTransform( D3DTS_PROJECTION , &m_projMatrix );

	////射影変換
 //   m_projMatrix( 0 , 0 ) = xScale;
 //   m_projMatrix( 1 , 0 ) = 0.0f;
 //   m_projMatrix( 2 , 0 ) = 0.0f;
 //   m_projMatrix( 3 , 0 ) = 0.0f;
	//			        
 //   m_projMatrix( 0 , 1 ) = 0.0f;
 //   m_projMatrix( 1 , 1 ) = yScale;
 //   m_projMatrix( 2 , 1 ) = 0.0f;
 //   m_projMatrix( 3 , 1 ) = 0.0f;
	//			        
 //   m_projMatrix( 0 , 2 ) = 0.0f;
 //   m_projMatrix( 1 , 2 ) = 0.0f;
 //   m_projMatrix( 2 , 2 ) = zfar / ( zfar - znear );
 //   m_projMatrix( 3 , 2 ) = -znear * zfar / ( zfar - znear );
	//			        
 //   m_projMatrix( 0 , 3 ) = 0.0f;
 //   m_projMatrix( 1 , 3 ) = 0.0f;
 //   m_projMatrix( 2 , 3 ) = 1.0f;
 //   m_projMatrix( 3 , 3 ) = 0.0f;
}

//回転
void CTPCamera::Rotate( float longitudeDegrees , float latitudeDegrees , float hoge )
{
    m_latitudeDegrees = latitudeDegrees;
    m_longitudeDegrees = longitudeDegrees;
}

//カメラ更新
void CTPCamera::UpdateCamera( const float elapsedTimeSec )
{
	//経過時間を考慮して回転を決定
    float latitudeElapsed = m_latitudeDegrees * m_rot_speed * elapsedTimeSec;
    float longitudeElapsed = m_longitudeDegrees * m_rot_speed * elapsedTimeSec;

	//回転
    D3DXQUATERNION rot;
    
	//ロールは無し
    D3DXQuaternionRotationYawPitchRoll( &rot , D3DXToRadian( longitudeElapsed ) , D3DXToRadian( latitudeElapsed ) , 0.0f );

	//現在のカメラの回転に基づいてオフセットベクトルを回転させる
	D3DXVECTOR3 new_eye = CCommon::QuaternionTransformCoord( m_offset , rot ) + m_target;

	//ビュー行列を再構成
    LookAt( new_eye , m_target , WORLD_YAXIS );
}
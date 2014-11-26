#include "DXUT.h"
#include "view_camera.h"
#include "../../Library/Graphic/graphics_manager.h"

//コンストラクタ
CViewCamera::CViewCamera() : m_distance( 1.0f )
{
	Perspective( /*80.0f*/D3DXToRadian( 45.0f ) , 0.0f , 0.0f , 0.0f );
	LookAt( D3DXVECTOR3( 0 , 0 , 0 ) , D3DXVECTOR3( 0 , 0 , 0 ) , D3DXVECTOR3( 0 , 1 , 0 ) );
}

//注視点から移動
void CViewCamera::LookAt( const D3DXVECTOR3& eye , const D3DXVECTOR3& target , const D3DXVECTOR3& up )
{
	//カメラ座標とターゲットをセット
    m_eye = eye;
	m_eye.z += m_distance;
    m_target = target;

	//ビュー行列更新
	D3DXMatrixLookAtLH( &m_viewMatrix , &m_eye , &m_target , &up );

    D3DXQuaternionRotationMatrix( &m_orientation , &m_viewMatrix );
}

//遠近投影
void CViewCamera::Perspective( const float fov , const float aspect , const float znear , const float zfar )
{
	//画角からアスペクト比を考慮したウィンドウ幅で投影するZ距離を算出する
	m_distance = ( CGraphicsManager::m_window_height * 0.5f ) * -tan( ( D3DX_PI - fov ) * 0.5f);

	//プロジェクション行列を更新する
	D3DXMatrixPerspectiveFovLH( &m_projMatrix , fov , CGraphicsManager::m_window_width / CGraphicsManager::m_window_height , 1 , abs( m_distance * 2.0f ) );
	//CGraphicsManager::m_pd3dDevice->SetTransform( D3DTS_PROJECTION , &m_projMatrix );
}
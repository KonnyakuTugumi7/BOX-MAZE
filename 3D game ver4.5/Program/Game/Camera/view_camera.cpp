#include "DXUT.h"
#include "view_camera.h"
#include "../../Library/Graphic/graphics_manager.h"

//コンストラクタ
CViewCamera::CViewCamera() : m_Distance( 1.0f )
{
	Perspective( /*80.0f*/D3DXToRadian( 45.0f ) , 0.0f , 0.0f , 0.0f );
	LookAt( D3DXVECTOR3( 0 , 0 , 0 ) , D3DXVECTOR3( 0 , 0 , 0 ) , D3DXVECTOR3( 0 , 1 , 0 ) );
}

//注視点から移動
void CViewCamera::LookAt( const D3DXVECTOR3& eye , const D3DXVECTOR3& target , const D3DXVECTOR3& up )
{
	//カメラ座標とターゲットをセット
    m_Eye = eye;
	m_Eye.z += m_Distance;
    m_Target = target;

	//ビュー行列更新
	D3DXMatrixLookAtLH( &m_ViewMatrix , &m_Eye , &m_Target , &up );

    D3DXQuaternionRotationMatrix( &m_Orientation , &m_ViewMatrix );
}

//遠近投影
void CViewCamera::Perspective( const float fov , const float aspect , const float znear , const float zfar )
{
	//画角からアスペクト比を考慮したウィンドウ幅で投影するZ距離を算出する
	m_Distance = ( CGraphicsManager::m_WindowHeight * 0.5f ) * -tan( ( D3DX_PI - fov ) * 0.5f);

	//プロジェクション行列を更新する
	D3DXMatrixPerspectiveFovLH( &m_ProjMatrix , fov , CGraphicsManager::m_WindowWidth / CGraphicsManager::m_WindowHeight , 1 , abs( m_Distance * 2.0f ) );
	//CGraphicsManager::m_pD3dDevice->SetTransform( D3DTS_PROJECTION , &m_ProjMatrix );
}
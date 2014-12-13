#include "DXUT.h"
#include "graphics_manager.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/vertex.h"
#include "../../GameEngine/Camera/camera_base.h"
#include "../../GameEngine/Collision/collision_manager.h"
#include "../../Game/Camera/fp_camera.h"

//graphics_managerクラスの静的メンバの実体
IDirect3DDevice9* CGraphicsManager::m_pD3dDevice;
ShaderBaseSP CGraphicsManager::m_SdrUse = NULL;
ShaderBaseSP CGraphicsManager::m_SdrBase = NULL;
ShaderDirectionalLightSP CGraphicsManager::m_SdrDirLight = NULL;
std::vector< std::list< GraphicBaseSP > > CGraphicsManager::m_RenderList;
float CGraphicsManager::m_WindowWidth = 800.0f;
float CGraphicsManager::m_WindowHeight = 600.0f;
CameraBaseSP CGraphicsManager::m_2dCamera;
CameraBaseSP CGraphicsManager::m_3dCamera;

//初期化
void CGraphicsManager::Initialize( IDirect3DDevice9* pd3dDevice )
{
	//描画使用するデバイスのポインタ
	m_pD3dDevice = pd3dDevice;

	//基本シェーダ作成
	m_SdrBase = CShaderBase::Create( "Content/hlsl/simple.fx" );

	//平行光源シェーダ作成
	m_SdrDirLight = CShaderDirectionalLight::Create( "Content/hlsl/directional_light.fx" );

	//レンダリングリスト作成
	for( int i = 0 ; i < STATE_NUM ; ++i )
	{
		std::list< GraphicBaseSP > list;
		m_RenderList.push_back( list );
	}

	//カメラ
	m_2dCamera = CViewCamera::Create();
	m_3dCamera = CFPCamera::Create();
}

//解放
void CGraphicsManager::Destroy()
{
	for( int i = 0 ; i < RENDERLIST_STATE::STATE_NUM ; ++i )
	{
		//レンダーリスト解放
		std::list< GraphicBaseSP >::iterator it = m_RenderList[ i ].begin();
		while( it != m_RenderList[ i ].end() )
		{
			( *it ).reset();
			++it;
		}
		m_RenderList[ i ].clear();
	}
	m_SdrUse.reset();
	m_SdrBase.reset();
	m_SdrDirLight.reset();
}

//描画
void CGraphicsManager::SysRender( const CameraBaseWP camera )
{
	//イテレータ
	std::list< GraphicBaseSP >::iterator it;

	//レンダリングリストの中身をソート(2D)
	m_RenderList[ RENDERLIST_STATE::BACK_2D ].sort( CGraphicBase::comp2D );
	m_RenderList[ RENDERLIST_STATE::FRONT_2D ].sort( CGraphicBase::comp2D );

	for( int i = 0 ; i < 2 ; ++i )
	{
		it = m_RenderList[ i + 1 ].begin();

		//カメラとオブジェクトの距離を計算
		while( it != m_RenderList[ i + 1 ].end() )
		{
			//距離計算
			( *it )->m_CameraDistance = D3DXVec3Length( &( ( *it )->m_Pos - camera.lock()->GetPosition() ) );

			++it;
		}
	}

	//レンダリングリストの中身をソート(3D)
	m_RenderList[ RENDERLIST_STATE::NORMAL_3D ].sort( CGraphicBase::comp3D );
	m_RenderList[ RENDERLIST_STATE::ALPHA_3D ].sort( CGraphicBase::comp3DAlpha );

	//レンダリングリストの中身を描画
	for( int i = 0 ; i < RENDERLIST_STATE::STATE_NUM ; ++i )
	{
		it = m_RenderList[ i ].begin();

		if( i == RENDERLIST_STATE::ALPHA_3D || i == RENDERLIST_STATE::FRONT_2D )
		{
			HRESULT hr;
			//zバッファのみクリア
			V( CGraphicsManager::m_pD3dDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255 , 0 , 0 , 0/*255, 0, 0, 255*/ ), 1.0f, 0 ) );
		}

		if( i == RENDERLIST_STATE::NORMAL_3D || i == RENDERLIST_STATE::ALPHA_3D ) CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
		else CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );

		while( it != m_RenderList[ i ].end() )
		{
			if( ( *it )->m_IsRender == true )
			{
				if( i == BACK_2D || i == FRONT_2D ) ( *it )->Render( m_2dCamera );
				else ( *it )->Render( m_3dCamera );
			}
			++it;
		}
	}
}

//更新
void CGraphicsManager::Update()
{
	for( int i = 0 ; i < RENDERLIST_STATE::STATE_NUM ; ++i )
	{
		std::list< GraphicBaseSP >::iterator it = m_RenderList[ i ].begin();
		while( it != m_RenderList[ i ].end() )
		{
			//カウント数が1(listもsceneのオブジェクトもshared_ptrにしているので)のオブジェクトを解放
			if( ( *it ).use_count() == 1 )
			{
				( *it ).reset();
				it = m_RenderList[ i ].erase( it );
				continue;
			}
			++it;
		}
	}
}

//オブジェクト登録
void CGraphicsManager::RegistObj( const GraphicBaseSP obj , const RENDERLIST_STATE list )
{
	m_RenderList[ list ].push_back( obj );
}
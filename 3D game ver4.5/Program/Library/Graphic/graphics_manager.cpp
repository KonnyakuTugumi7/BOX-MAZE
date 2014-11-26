#include "DXUT.h"
#include "graphics_manager.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/vertex.h"
#include "../../GameEngine/Camera/camera_base.h"
#include "../../GameEngine/Collision/collision_manager.h"

//graphics_managerクラスの静的メンバの実体
IDirect3DDevice9* CGraphicsManager::m_pd3dDevice;
ShaderBaseSP CGraphicsManager::m_sdr_use = NULL;
ShaderBaseSP CGraphicsManager::m_sdr_base = NULL;
ShaderDirectionalLightSP CGraphicsManager::m_sdr_dir_light = NULL;
std::vector< std::list< GraphicBaseSP > > CGraphicsManager::m_render_list;
float CGraphicsManager::m_window_width = 800.0f;
float CGraphicsManager::m_window_height = 600.0f;

//初期化
void CGraphicsManager::Initialize( IDirect3DDevice9* pd3dDevice )
{
	//描画使用するデバイスのポインタ
	m_pd3dDevice = pd3dDevice;

	//基本シェーダ作成
	m_sdr_base = CShaderBase::Create( "Content/hlsl/simple.fx" );

	//平行光源シェーダ作成
	m_sdr_dir_light = CShaderDirectionalLight::Create( "Content/hlsl/directional_light.fx" );

	//レンダリングリスト作成
	for( int i = 0 ; i < STATE_NUM ; ++i )
	{
		std::list< GraphicBaseSP > list;
		m_render_list.push_back( list );
	}
}

//解放
void CGraphicsManager::Destroy()
{
	for( int i = 0 ; i < RENDERLIST_STATE::STATE_NUM ; ++i )
	{
		//レンダーリスト解放
		std::list< GraphicBaseSP >::iterator it = m_render_list[ i ].begin();
		while( it != m_render_list[ i ].end() )
		{
			( *it ).reset();
			++it;
		}
		m_render_list[ i ].clear();
	}
	m_sdr_use.reset();
	m_sdr_base.reset();
	m_sdr_dir_light.reset();
}

//描画
void CGraphicsManager::SysRender( const CameraBaseWP camera )
{
	//イテレータ
	std::list< GraphicBaseSP >::iterator it;

	//レンダリングリストの中身をソート(2D)
	m_render_list[ RENDERLIST_STATE::BACK_2D ].sort( CGraphicBase::comp2D );
	m_render_list[ RENDERLIST_STATE::FRONT_2D ].sort( CGraphicBase::comp2D );

	for( int i = 0 ; i < 2 ; ++i )
	{
		it = m_render_list[ i + 2 ].begin();

		//カメラとオブジェクトの距離を計算
		while( it != m_render_list[ i + 2 ].end() )
		{
			//距離計算
			( *it )->m_camera_distance = D3DXVec3Length( &( ( *it )->m_pos - camera.lock()->GetPosition() ) );

			++it;
		}
	}

	//レンダリングリストの中身をソート(3D)
	m_render_list[ RENDERLIST_STATE::NORMAL_3D ].sort( CGraphicBase::comp3D );
	m_render_list[ RENDERLIST_STATE::ALPHA_3D ].sort( CGraphicBase::comp3DAlpha );

	//レンダリングリストの中身を描画
	for( int i = 0 ; i < RENDERLIST_STATE::STATE_NUM ; ++i )
	{
		it = m_render_list[ i ].begin();

		while( it != m_render_list[ i ].end() )
		{
			if( ( *it )->m_is_render == true )
			{
				( *it )->Render( camera );
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
		std::list< GraphicBaseSP >::iterator it = m_render_list[ i ].begin();
		while( it != m_render_list[ i ].end() )
		{
			//カウント数が1(listもsceneのオブジェクトもshared_ptrにしているので)のオブジェクトを解放
			if( ( *it ).use_count() == 1 )
			{
				( *it ).reset();
				it = m_render_list[ i ].erase( it );
				continue;
			}
			++it;
		}
	}
}

//オブジェクト登録
void CGraphicsManager::RegistObj( const GraphicBaseSP obj , const RENDERLIST_STATE list )
{
	m_render_list[ list ].push_back( obj );
}
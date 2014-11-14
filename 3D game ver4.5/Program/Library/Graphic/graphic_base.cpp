#include "DXUT.h"
#include "graphic_base.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../GameEngine/Camera/camera_base.h"

//描画
void CGraphicBase::Render( const CameraBaseWP camera )
{
	HRESULT hr;
	
	//ワールドマトリクス設定(ローカル座標からワールド座標へ)
	SetWldMtx();
	m_wld_mtx = m_wld_mtx * camera.lock()->GetViewMatrix() * camera.lock()->GetProjectionMatrix();

	//カリング設定
	CGraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_CULLMODE , m_cull_state );

	//シェーダ設定
	SetSDR();

	if( m_is_render == false ) return;

	//----------------------------描画処理-------------------------------

	// 3D モデルのパーツ分ループして描画
	for( int i = 0 ; i < m_mesh->GetMaterialNum() ; i++ )
	{
		// 格パーツに対応するテクスチャを設定
		// シェーダにテクスチャを渡す
		if( 0 != m_texs.size() )
		{
			if( NULL != m_texs[ i ] )
			{
				CGraphicsManager::m_sdr_use->m_sdr->SetTexture( CGraphicsManager::m_sdr_use->m_sdr_tex , m_texs[ i ]->GetTex() );
				CGraphicsManager::m_sdr_use->m_sdr->SetVector( CGraphicsManager::m_sdr_use->m_sdr_color , &D3DXVECTOR4( 1 , 1 , 1 , 1 ) );
			}
			else
			{
				CGraphicsManager::m_sdr_use->m_sdr->SetVector( CGraphicsManager::m_sdr_use->m_sdr_color , &D3DXVECTOR4( m_col.x , m_col.y , m_col.z , m_col.w ) );
			}
		}
		else
		{
			CGraphicsManager::m_sdr_use->m_sdr->SetVector( CGraphicsManager::m_sdr_use->m_sdr_color , &D3DXVECTOR4( m_col.x , m_col.y , m_col.z , m_col.w ) );
		}

		//シェーダの使用開始
		CGraphicsManager::m_sdr_use->Begin();

		// シェーダのパス設定
		if( 0 != m_texs.size() )
		{
			if( !m_is_add_blend )
			{
				CGraphicsManager::m_sdr_use->BeginPass( 0 );
			}
			else
			{
				CGraphicsManager::m_sdr_use->BeginPass( 1 );
			}
		}
		else
		{
			CGraphicsManager::m_sdr_use->BeginPass( 2 );
		}

		if( SUCCEEDED( CGraphicsManager::m_pd3dDevice->BeginScene() ) )
		{
			//パーツの描画
			m_mesh->GetMesh()->DrawSubset( i ) ;
			V( CGraphicsManager::m_pd3dDevice->EndScene() );
		}

		//パス終了
		CGraphicsManager::m_sdr_use->EndPass();

		//シェーダ終了
		CGraphicsManager::m_sdr_use->End();
	}
}

//シェーダ設定
void CGraphicBase::SetSDR()
{
	//シェーダを切り替える
	if( m_sdr_state == SDR_BASE )
	{
		CGraphicsManager::m_sdr_use = CGraphicsManager::m_sdr_base;
	}
	else if( m_sdr_state == SDR_DIR_LIGHT )
	{
		CGraphicsManager::m_sdr_use = CGraphicsManager::m_sdr_dir_light;
	}

	//テクニック設定
	CGraphicsManager::m_sdr_use->m_sdr->SetTechnique( CGraphicsManager::m_sdr_use->m_sdr_technique );
	
	//ワールド行列設定
	CGraphicsManager::m_sdr_use->m_sdr->SetMatrix( CGraphicsManager::m_sdr_use->m_sdr_wvmp , &m_wld_mtx );

	//カラー設定
	CGraphicsManager::m_sdr_use->m_sdr->SetVector( CGraphicsManager::m_sdr_use->m_sdr_color , &m_col );

	//平行光源の時
	if( m_sdr_state == SDR_DIR_LIGHT )
	{
		//ライトの設定
		CGraphicsManager::m_sdr_dir_light->m_sdr->SetVector( CGraphicsManager::m_sdr_dir_light->m_dir_light , &CGraphicsManager::m_sdr_dir_light->m_dir );

		//回転行列の設定
		CGraphicsManager::m_sdr_dir_light->m_sdr->SetMatrix( CGraphicsManager::m_sdr_dir_light->m_rot_mtx , &m_rot_mtx );

		//アンビエント色の設定
		CGraphicsManager::m_sdr_dir_light->m_sdr->SetFloat( CGraphicsManager::m_sdr_dir_light->m_ambient , 0.08f );
	}
}
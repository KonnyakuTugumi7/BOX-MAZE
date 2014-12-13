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
	m_WldMtx = m_WldMtx * camera.lock()->GetViewMatrix() * camera.lock()->GetProjectionMatrix();

	//カリング設定
	CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_CULLMODE , m_CullState );

	//シェーダ設定
	SetSDR();

	if( m_IsRender == false ) return;

	//----------------------------描画処理-------------------------------

	// 3D モデルのパーツ分ループして描画
	for( int i = 0 ; i < m_Mesh->GetMaterialNum() ; i++ )
	{
		// 格パーツに対応するテクスチャを設定
		// シェーダにテクスチャを渡す
		if( 0 != m_Texs.size() )
		{
			if( NULL != m_Texs[ i ] )
			{
				CGraphicsManager::m_SdrUse->m_Sdr->SetTexture( CGraphicsManager::m_SdrUse->m_SdrTex , m_Texs[ i ]->GetTex() );
				CGraphicsManager::m_SdrUse->m_Sdr->SetVector( CGraphicsManager::m_SdrUse->m_SdrColor , &D3DXVECTOR4( m_Col.x , m_Col.y , m_Col.z , m_Col.w ) );
			}
			else
			{
				CGraphicsManager::m_SdrUse->m_Sdr->SetVector( CGraphicsManager::m_SdrUse->m_SdrColor , &D3DXVECTOR4( m_Col.x , m_Col.y , m_Col.z , m_Col.w ) );
			}
		}
		else
		{
			CGraphicsManager::m_SdrUse->m_Sdr->SetVector( CGraphicsManager::m_SdrUse->m_SdrColor , &D3DXVECTOR4( m_Col.x , m_Col.y , m_Col.z , m_Col.w ) );
		}

		//シェーダの使用開始
		CGraphicsManager::m_SdrUse->Begin();

		// シェーダのパス設定
		if( 0 != m_Texs.size() )
		{
			if( !m_IsAddBlend )
			{
				CGraphicsManager::m_SdrUse->BeginPass( 0 );
			}
			else
			{
				CGraphicsManager::m_SdrUse->BeginPass( 1 );
			}
		}
		else
		{
			CGraphicsManager::m_SdrUse->BeginPass( 2 );
		}

		if( SUCCEEDED( CGraphicsManager::m_pD3dDevice->BeginScene() ) )
		{
			//パーツの描画
			m_Mesh->GetMesh()->DrawSubset( i ) ;
			V( CGraphicsManager::m_pD3dDevice->EndScene() );
		}

		//パス終了
		CGraphicsManager::m_SdrUse->EndPass();

		//シェーダ終了
		CGraphicsManager::m_SdrUse->End();
	}
}

//シェーダ設定
void CGraphicBase::SetSDR()
{
	//シェーダを切り替える
	if( m_sdr_state == SDR_BASE )
	{
		CGraphicsManager::m_SdrUse = CGraphicsManager::m_SdrBase;
	}
	else if( m_sdr_state == SDR_DIR_LIGHT )
	{
		CGraphicsManager::m_SdrUse = CGraphicsManager::m_SdrDirLight;
	}

	//テクニック設定
	CGraphicsManager::m_SdrUse->m_Sdr->SetTechnique( CGraphicsManager::m_SdrUse->m_SdrTechnique );
	
	//ワールド行列設定
	CGraphicsManager::m_SdrUse->m_Sdr->SetMatrix( CGraphicsManager::m_SdrUse->m_SdrWvmp , &m_WldMtx );

	//カラー設定
	CGraphicsManager::m_SdrUse->m_Sdr->SetVector( CGraphicsManager::m_SdrUse->m_SdrColor , &m_Col );

	//平行光源の時
	if( m_sdr_state == SDR_DIR_LIGHT )
	{
		//ライトの設定
		CGraphicsManager::m_SdrDirLight->m_Sdr->SetVector( CGraphicsManager::m_SdrDirLight->m_DirLight , &CGraphicsManager::m_SdrDirLight->m_Dir );

		//回転行列の設定
		CGraphicsManager::m_SdrDirLight->m_Sdr->SetMatrix( CGraphicsManager::m_SdrDirLight->m_RotMtx , &m_RotMtx );

		//アンビエント色の設定
		CGraphicsManager::m_SdrDirLight->m_Sdr->SetFloat( CGraphicsManager::m_SdrDirLight->m_Ambient , 0.08f );
	}
}
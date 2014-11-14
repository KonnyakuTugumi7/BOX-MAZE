#include "DXUT.h"
#include "board.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Graphic/texture.h"
#include "../../Library/Graphic/vertex.h"
#include "../../Library/Graphic/mesh.h"
#include "../../GameEngine/Graphic/cube.h"
#include "../../GameEngine/Camera/camera_base.h"

//生成
BoardSP CBoard::Create( const int width , const int height , const std::string path  , const CGraphicsManager::RENDERLIST_STATE list )
{
	//ボード
	BoardSP board = std::make_shared< CBoard >();

	//初期化
	board->Init( width , height , path );

	//レンダリングリストへの登録
	CGraphicsManager::RegistObj( board , list );

	return board;
}

//初期化
void CBoard::Init( const int width , const int height , const std::string path )
{
	//ビルボードフラグ
	m_is_billboard = false;

	//カリング設定(カリングしない)
	//m_cull_state = D3DCULL_NONE;

	if( path == "" )
	{
		//色
		m_col.x = 0.0f;
		m_col.y = 0.0f;
		m_col.z = 1.0f;
		m_col.w = 0.5f;
	}

	//板ポリサイズ
	m_board_size.x = width;
	m_board_size.y = height;
	m_board_size.z = 0;

	//シェーダ
	m_sdr_state = SDR_BASE;

	//名前
	std::stringstream ss;
	ss << path << width << height;

	//テクスチャ作成
	if( path != "" ) m_texs.push_back( CTexture::Create( path ) );

	//メッシュ作成(名前は使うテクスチャのパスと幅と高さを合わせたもの)
	m_mesh = CMesh::CreateFromFVF( ss.str() );

	//インデックスデータ作成
	WORD* pIndexs;

	m_mesh->GetMesh()->LockIndexBuffer( 0 , ( void ** )&pIndexs );
	pIndexs[ 0 ] = 0;
	pIndexs[ 1 ] = 1;
	pIndexs[ 2 ] = 2;
					 
	pIndexs[ 3 ] = 1;
	pIndexs[ 4 ] = 3;
	pIndexs[ 5 ] = 2;
	m_mesh->GetMesh()->UnlockIndexBuffer();

	//頂点データ作成
	CVertex* pVertex;

	m_mesh->GetMesh()->LockVertexBuffer( 0 , ( void** )&pVertex );
	pVertex[ 0 ].m_pos = D3DXVECTOR3( -m_board_size.x / 2 , m_board_size.y / 2 , 0 );
	pVertex[ 1 ].m_pos = D3DXVECTOR3( m_board_size.x / 2 , m_board_size.y / 2 , 0 );
	pVertex[ 2 ].m_pos = D3DXVECTOR3( -m_board_size.x / 2 , -m_board_size.y / 2 , 0 );
	pVertex[ 3 ].m_pos = D3DXVECTOR3( m_board_size.x / 2 , -m_board_size.y / 2 , 0 );

	pVertex[ 0 ].m_nml = D3DXVECTOR3( 0 , 0 , -1 );
	pVertex[ 1 ].m_nml = D3DXVECTOR3( 0 , 0 , -1 );
	pVertex[ 2 ].m_nml = D3DXVECTOR3( 0 , 0 , -1 );
	pVertex[ 3 ].m_nml = D3DXVECTOR3( 0 , 0 , -1 );

	pVertex[ 0 ].m_uv = D3DXVECTOR2( 0.0f , 0.0f );
	pVertex[ 1 ].m_uv = D3DXVECTOR2( 1.0f , 0.0f );
	pVertex[ 2 ].m_uv = D3DXVECTOR2( 0.0f , 1.0f );
	pVertex[ 3 ].m_uv = D3DXVECTOR2( 1.0f , 1.0f );
	m_mesh->GetMesh()->UnlockIndexBuffer();
}

//描画
void CBoard::Render( const CameraBaseWP camera )
{
	HRESULT hr;

	//ワールドマトリクス設定(ローカル座標からワールド座標へ)
	if( m_is_billboard == true )
	{
		SetWldMtxBillBoard( camera );
	}
	else
	{
		//親(cube)が存在する場合は親のワールドマトリックスを渡す
		m_cube.expired() ? SetWldMtx() : SetWldMtx( &m_cube.lock()->GetWldMtx() );
	}
	m_wld_mtx = m_wld_mtx * camera.lock()->GetViewMatrix() * camera.lock()->GetProjectionMatrix();

	//親が存在してかつ親の描画フラグがオフなら以下の処理を行わない
	if( m_cube.expired() == false && m_cube.lock()->GetIsRender() == false ) return;

	//カリング設定
	CGraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_CULLMODE , m_cull_state );

	//シェーダ設定
	SetSDR();

	//----------------------------描画処理-------------------------------

	//シェーダにテクスチャを渡す
	if( 0 != m_texs.size() )
	{
		CGraphicsManager::m_sdr_use->m_sdr->SetTexture( CGraphicsManager::m_sdr_use->m_sdr_tex , m_texs[ 0 ]->GetTex() );
		CGraphicsManager::m_sdr_use->m_sdr->SetVector( CGraphicsManager::m_sdr_use->m_sdr_color , &D3DXVECTOR4( 1 , 1 , 1 , 1 ) );
	}
	//テクスチャが無ければカラーを渡す
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
		if(D3D_OK != m_mesh->GetMesh()->DrawSubset( 0 ))
		{
			return;
		}
		V( CGraphicsManager::m_pd3dDevice->EndScene() );
	}

	//パス終了
	CGraphicsManager::m_sdr_use->EndPass();

	//シェーダ終了
	CGraphicsManager::m_sdr_use->End();
}
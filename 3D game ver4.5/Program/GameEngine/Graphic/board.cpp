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
	m_IsBillboard = false;

	//カリング設定(カリングしない)
	//m_CullState = D3DCULL_NONE;

	if( path == "" )
	{
		//色
		m_Col.x = 0.0f;
		m_Col.y = 0.0f;
		m_Col.z = 1.0f;
		m_Col.w = 0.5f;
	}

	//板ポリサイズ
	m_BoardSize.x = width;
	m_BoardSize.y = height;
	m_BoardSize.z = 0;

	//シェーダ
	m_sdr_state = SDR_BASE;

	//名前
	std::stringstream ss;
	ss << path << width << height;

	//テクスチャ作成
	if( path != "" ) m_Texs.push_back( CTexture::Create( path ) );

	//メッシュ作成(名前は使うテクスチャのパスと幅と高さを合わせたもの)
	m_Mesh = CMesh::CreateFromFVF( ss.str() );

	//インデックスデータ作成
	WORD* pIndexs;

	m_Mesh->GetMesh()->LockIndexBuffer( 0 , ( void ** )&pIndexs );
	pIndexs[ 0 ] = 0;
	pIndexs[ 1 ] = 1;
	pIndexs[ 2 ] = 2;
					 
	pIndexs[ 3 ] = 1;
	pIndexs[ 4 ] = 3;
	pIndexs[ 5 ] = 2;
	m_Mesh->GetMesh()->UnlockIndexBuffer();

	//頂点データ作成
	CVertex* pVertex;

	m_Mesh->GetMesh()->LockVertexBuffer( 0 , ( void** )&pVertex );
	pVertex[ 0 ].m_Pos = D3DXVECTOR3( -m_BoardSize.x / 2 , m_BoardSize.y / 2 , 0 );
	pVertex[ 1 ].m_Pos = D3DXVECTOR3( m_BoardSize.x / 2 , m_BoardSize.y / 2 , 0 );
	pVertex[ 2 ].m_Pos = D3DXVECTOR3( -m_BoardSize.x / 2 , -m_BoardSize.y / 2 , 0 );
	pVertex[ 3 ].m_Pos = D3DXVECTOR3( m_BoardSize.x / 2 , -m_BoardSize.y / 2 , 0 );

	pVertex[ 0 ].m_Nml = D3DXVECTOR3( 0 , 0 , -1 );
	pVertex[ 1 ].m_Nml = D3DXVECTOR3( 0 , 0 , -1 );
	pVertex[ 2 ].m_Nml = D3DXVECTOR3( 0 , 0 , -1 );
	pVertex[ 3 ].m_Nml = D3DXVECTOR3( 0 , 0 , -1 );

	pVertex[ 0 ].m_Uv = D3DXVECTOR2( 0.0f , 0.0f );
	pVertex[ 1 ].m_Uv = D3DXVECTOR2( 1.0f , 0.0f );
	pVertex[ 2 ].m_Uv = D3DXVECTOR2( 0.0f , 1.0f );
	pVertex[ 3 ].m_Uv = D3DXVECTOR2( 1.0f , 1.0f );
	m_Mesh->GetMesh()->UnlockIndexBuffer();
}

//描画
void CBoard::Render( const CameraBaseWP camera )
{
	HRESULT hr;

	//ワールドマトリクス設定(ローカル座標からワールド座標へ)
	if( m_IsBillboard == true )
	{
		SetWldMtxBillBoard( camera );
	}
	else
	{
		//親(cube)が存在する場合は親のワールドマトリックスを渡す
		m_Cube.expired() ? SetWldMtx() : SetWldMtx( &m_Cube.lock()->GetWldMtx() );
	}
	m_WldMtx = m_WldMtx * camera.lock()->GetViewMatrix() * camera.lock()->GetProjectionMatrix();

	//親が存在してかつ親の描画フラグがオフなら以下の処理を行わない
	if( m_Cube.expired() == false && m_Cube.lock()->GetIsRender() == false ) return;

	//カリング設定
	CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_CULLMODE , m_CullState );

	//シェーダ設定
	SetSDR();

	//----------------------------描画処理-------------------------------

	//シェーダにテクスチャを渡す
	if( 0 != m_Texs.size() )
	{
		CGraphicsManager::m_SdrUse->m_Sdr->SetTexture( CGraphicsManager::m_SdrUse->m_SdrTex , m_Texs[ 0 ]->GetTex() );
		CGraphicsManager::m_SdrUse->m_Sdr->SetVector( CGraphicsManager::m_SdrUse->m_SdrColor , &D3DXVECTOR4( m_Col.x , m_Col.y , m_Col.z , m_Col.w ) );
	}
	//テクスチャが無ければカラーのみを渡す
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
		if(D3D_OK != m_Mesh->GetMesh()->DrawSubset( 0 ))
		{
			return;
		}
		V( CGraphicsManager::m_pD3dDevice->EndScene() );
	}

	//パス終了
	CGraphicsManager::m_SdrUse->EndPass();

	//シェーダ終了
	CGraphicsManager::m_SdrUse->End();
}

//ワールドマトリックス設定(billboard用) //カメラの方向を向く用
void CBoard::SetWldMtxBillBoard( const CameraBaseWP camera )
{
	//-----------------------------ワールドマトリクス設定(ローカル座標からワールド座標へ)-------------------------------------

	D3DXMATRIX SclMtx , RotMtx , PosMtx;

	//---------------------------拡大---------------------------
	D3DXMatrixScaling( &SclMtx , m_Scl.x , m_Scl.y , m_Scl.z );

	//--------------------------平行移動--------------------------
	D3DXMatrixTranslation( &PosMtx, m_Pos.x, m_Pos.y, m_Pos.z );

	//ビュー行列を作成
	D3DXMatrixLookAtLH( &RotMtx , &camera.lock()->GetPosition() , &m_Pos , &D3DXVECTOR3( 0 , 1 , 0 ) );

	//平行移動行列を無効(0)に
	RotMtx._41 = 0;
	RotMtx._42 = 0;
	RotMtx._43 = 0;

	//逆行列作成
	D3DXMatrixInverse( &RotMtx , NULL , &RotMtx );

	//ワールドマトリックス
	m_WldMtx =  SclMtx * RotMtx * PosMtx;
}

//テクスチャセッター
void CBoard::SetTex( const TextureSP tex ){ m_Texs.clear();m_Texs.push_back( tex ); }
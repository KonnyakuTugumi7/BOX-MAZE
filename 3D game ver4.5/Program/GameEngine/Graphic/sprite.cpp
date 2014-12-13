#include "DXUT.h"
#include "sprite.h"
#include "../../Library/Support/common.h"
#include "../../GameEngine/Camera/camera_base.h"

//生成
SpriteSP CSprite::Create( const std::string file_name , const CGraphicsManager::RENDERLIST_STATE list , const int x_num , const int y_num )
{
	//スプライト
	SpriteSP obj = std::make_shared< CSprite >();

	//初期化
	obj->Init( file_name , x_num , y_num );

	//レンダリングリストへ登録
	CGraphicsManager::RegistObj( obj , list );

	return obj;
}

//初期化
void CSprite::Init( const std::string file_name , const int x_num , const int y_num )
{
	//名前
	m_Name = file_name;

	//----------------------------------------------------------------------------
	// ファイル名を WCHAR に変換
	WCHAR buff_name[256] = {0};
	CCommon::DXconvAnsiToWide( buff_name, file_name.c_str(), (file_name.length() + 1) );
	
	//----------------------------------------------------------------------------
	// テクスチャのロード
	if( FAILED(D3DXCreateTextureFromFileEx( CGraphicsManager::m_pD3dDevice,
		buff_name,			// ファイルパス
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		D3DXCOLOR(0,0,0,0),	// カラーキーの設定( 透明色 )　※ 全て0指定でカラーキーなし
		&m_ImageInfo,		// 画像情報の取得
		NULL,
		&m_Texture			// テクスチャの受け取り
	) ) );

	//テクスチャからサーフェイスを取得
	IDirect3DSurface9 *pSurface;
	m_Texture->GetSurfaceLevel( 0 , &pSurface );

	//サーフェイス情報から画像サイズを取得
	D3DSURFACE_DESC SurfaceInfo;
	pSurface->GetDesc( &SurfaceInfo );

	//分割する画像の1つの大きさ
	int x_split = m_ImageInfo.Width / x_num;
	int y_split = m_ImageInfo.Height / y_num;

	//画像を分割して描画する範囲を格納する
	for( int y = 0 ; y < y_num ; ++y )
	{
		for( int x = 0 ; x < x_num ; ++x )
		{
			RECT rect = { x * x_split , y * y_split , ( x + 1 ) * x_split , ( y + 1 ) * y_split };
			m_Rect.push_back( rect );
		}
	}

	//画像のサイズ
	m_ImgSize = D3DXVECTOR3( m_ImageInfo.Width , m_ImageInfo.Height , 0 );

	//画像の中心
	m_Center = D3DXVECTOR3( m_ImgSize.x / x_num * 0.5f , m_ImgSize.y / y_num * 0.5f , 0 );

	//使い終わったのでサーフェイス情報は解放
	SAFE_RELEASE( pSurface ) ;

	//スプライトの生成
	D3DXCreateSprite( CGraphicsManager::m_pD3dDevice , &m_Sprite );

	//ソートに使う値
	m_CameraDistance = 1.0f;
}

//描画
void CSprite::Render( const CameraBaseWP camera )
{
	HRESULT hr;

	//ワールドマトリックス設定
	SetWldMtx();

	//スプライトにワールドマトリクスを設定
	m_Sprite->SetTransform( &m_WldMtx );

	//描画フラグがオフの時以下の処理を行わない
	if( m_IsRender == false ) return;

	//加算ブレンドの設定
	if( m_IsAddBlend )
	{
		CGraphicsManager::m_pD3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		CGraphicsManager::m_pD3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
		CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
		CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	}
	else
	{
		CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	}

	//デバイスの描画準備
	if( SUCCEEDED( CGraphicsManager::m_pD3dDevice->BeginScene() ) )
	{
		//スプライトの描画準備
		m_Sprite->Begin( D3DXSPRITE_ALPHABLEND );

		//スプライトの描画命令
		m_Sprite->Draw(	m_Texture ,							//描画に使用するテクスチャ
			&m_Rect[ m_AnimationFrame ] ,					//画像の描画範囲
			&m_Center ,										//サーフェイスの中心
			&D3DXVECTOR3( 0 , 0 , 0 ) ,						//描画座標(行列で制御するので 0,0,0 でOK)
			D3DCOLOR_ARGB( ( int )( m_Col.w * 255.0f ) ,	//アルファと色
						   ( int )( m_Col.x * 255 ) , 
						   ( int )( m_Col.y * 255 ) , 
						   ( int )( m_Col.z * 255 ) )
			);

		//加算切り替え
		if( m_IsAddBlend )
		{
			m_Sprite->Flush();
		}
		else
		{
			m_Sprite->End();
		}

		//描画終了
		V( CGraphicsManager::m_pD3dDevice->EndScene() );
	}
}
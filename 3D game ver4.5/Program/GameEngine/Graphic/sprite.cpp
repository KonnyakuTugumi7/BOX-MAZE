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
	m_name = file_name;

	//----------------------------------------------------------------------------
	// ファイル名を WCHAR に変換
	WCHAR buff_name[256] = {0};
	CCommon::DXconvAnsiToWide( buff_name, file_name.c_str(), (file_name.length() + 1) );
	
	//----------------------------------------------------------------------------
	// テクスチャのロード
	if( FAILED(D3DXCreateTextureFromFileEx( CGraphicsManager::m_pd3dDevice,
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
		&m_image_info,		// 画像情報の取得
		NULL,
		&m_texture			// テクスチャの受け取り
	) ) );

	//テクスチャからサーフェイスを取得
	IDirect3DSurface9 *pSurface;
	m_texture->GetSurfaceLevel( 0 , &pSurface );

	//サーフェイス情報から画像サイズを取得
	D3DSURFACE_DESC SurfaceInfo;
	pSurface->GetDesc( &SurfaceInfo );

	//分割する画像の1つの大きさ
	int x_split = m_image_info.Width / x_num;
	int y_split = m_image_info.Height / y_num;

	//画像を分割して描画する範囲を格納する
	for( int y = 0 ; y < y_num ; ++y )
	{
		for( int x = 0 ; x < x_num ; ++x )
		{
			RECT rect = { x * x_split , y * y_split , ( x + 1 ) * x_split , ( y + 1 ) * y_split };
			m_rect.push_back( rect );
		}
	}

	//画像のサイズ
	m_img_size = D3DXVECTOR3( m_image_info.Width , m_image_info.Height , 0 );

	//画像の中心
	m_center = D3DXVECTOR3( m_img_size.x / x_num * 0.5f , m_img_size.y / y_num * 0.5f , 0 );

	//使い終わったのでサーフェイス情報は解放
	SAFE_RELEASE( pSurface ) ;

	//スプライトの生成
	D3DXCreateSprite( CGraphicsManager::m_pd3dDevice , &m_sprite );

	//ソートに使う値
	m_camera_distance = 1.0f;
}

//描画
void CSprite::Render( const CameraBaseWP camera )
{
	HRESULT hr;

	//ワールドマトリックス設定
	SetWldMtx();

	//スプライトにワールドマトリクスを設定
	m_sprite->SetTransform( &m_wld_mtx );

	//描画フラグがオフの時以下の処理を行わない
	if( m_is_render == false ) return;

	//加算ブレンドの設定
	if( m_is_add_blend )
	{
		CGraphicsManager::m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		CGraphicsManager::m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
		CGraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		CGraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
		CGraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		CGraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	}
	else
	{
		CGraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	}

	//デバイスの描画準備
	if( SUCCEEDED( CGraphicsManager::m_pd3dDevice->BeginScene() ) )
	{
		//スプライトの描画準備
		m_sprite->Begin( D3DXSPRITE_ALPHABLEND );

		//スプライトの描画命令
		m_sprite->Draw(	m_texture ,							//描画に使用するテクスチャ
			&m_rect[ m_animation_frame ] ,					//画像の描画範囲
			&m_center ,										//サーフェイスの中心
			&D3DXVECTOR3( 0 , 0 , 0 ) ,						//描画座標(行列で制御するので 0,0,0 でOK)
			D3DCOLOR_ARGB( ( int )( m_col.w * 255.0f ) ,	//アルファと色
						   ( int )( m_col.x * 255 ) , 
						   ( int )( m_col.y * 255 ) , 
						   ( int )( m_col.z * 255 ) )
			);

		//加算切り替え
		if( m_is_add_blend )
		{
			m_sprite->Flush();
		}
		else
		{
			m_sprite->End();
		}

		//描画終了
		V( CGraphicsManager::m_pd3dDevice->EndScene() );
	}
}
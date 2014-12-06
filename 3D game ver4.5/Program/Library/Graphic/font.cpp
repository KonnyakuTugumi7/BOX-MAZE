#include "DXUT.h"
#include "font.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Graphic/vertex.h"
#include "../../GameEngine/Graphic/cube.h"
#include "../../GameEngine/Camera/camera_base.h"
#include <cassert>

//デストラクタ
CFont::~CFont()
{
	//データをすべて解放
	for ( auto it = CFont::m_data.begin() ; it != CFont::m_data.end() ; it++ )
	{
		SAFE_RELEASE( ( *it ).second );
	}
	m_data.clear();
}

//描画
void CFont::Draw( const CameraBaseWP camera , const std::wstring& string , const D3DXVECTOR2& pos , const D3DXVECTOR4& color)
{
	HRESULT hr;
	//zバッファのみクリア
	V( CGraphicsManager::m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255 , 0 , 0 , 0/*255, 0, 0, 255*/ ), 1.0f, 0 ) );

	//左上を(0,0)にする為のオフセットを定義
	D3DXVECTOR2 offsetPt( CGraphicsManager::m_window_width * 0.5f , CGraphicsManager::m_window_height * 0.5f);

	// 座標を定義
	D3DXVECTOR3 drawPt( 0 , offsetPt.y - pos.y , 0 );

	for( std::size_t i = 0 ; i < string.size() ; i++ )
	{
		// 座標を整列順に補正
		switch(m_align)
		{
		// 左揃え
		case LEFT:
			drawPt.x = pos.x + ( i * m_scale.x ) - offsetPt.x;
			break;

		// 右揃え
		case RIGHT:
			drawPt.x = pos.x - ( m_scale.x * ( string.size() - 1 - i ) ) - offsetPt.x;
			break;

		// 真ん中揃え
		case CENTER:
			drawPt.x = pos.x + ( i * m_scale.x ) - ( m_scale.x * 0.5f * ( string.size() - 1 ) ) - offsetPt.x;
			break;
		}
		m_pos = drawPt;
		m_scl = m_scale;
		m_col = color;
		m_texs[ 0 ]->SetTex( m_data[ string.at( i ) ] );
		//描画
		Render( camera );
	}
}

bool CFont::CreateTexture( std::size_t size , std::size_t weight , const std::wstring& type , const std::wstring& fonts )
{
	// フォントサイズを２の累乗サイズで算出する
	std::size_t fontSize = 1;
	while (fontSize < size) fontSize <<= 1;

	LOGFONT lf =
	{
		size,                      // 文字セルまたは文字の高さ
		0,                         // 平均文字幅
		0,                         // 文字送りの方向とX軸との角度
		0,                         // ベースラインとX軸との角度
		weight,                    // フォントの太さ
		0,                         // イタリック体指定
		0,                         // 下線付き指定
		0,                         // 打ち消し線付き指定
		SHIFTJIS_CHARSET,          // キャラクタセット
		OUT_TT_ONLY_PRECIS,        // 出力精度
		CLIP_DEFAULT_PRECIS,       // クリッピングの精度
		PROOF_QUALITY,             // 出力品質
		DEFAULT_PITCH | FF_MODERN, // ピッチとファミリ
		NULL                       // フォント名
	};
	
	// 字体を設定する
	lstrcpyW(lf.lfFaceName, type.c_str());

	HFONT hFont = CreateFontIndirect(&lf);
	if (!hFont)
	{
		MessageBox(NULL, TEXT(" !! Font Create Err 01 !! "), NULL, MB_OK | MB_ICONSTOP);
		return false;
	}	
	
	// デバイスコンテキスト取得
	// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
	
	for(std::size_t i = 0; i < fonts.size(); i++)
	{
		// フォントビットマップ取得
		const wchar_t c = fonts.at(i);

		// 多重生成を防ぐ
		auto it = m_data.find(c);
		if (it != m_data.end()) continue;

		// 1 文字コード取得
		UINT code = (UINT)c;
	
		// フォントビットマップ取得
		TEXTMETRIC TM;
		GetTextMetrics(hdc, &TM);
		GLYPHMETRICS GM;
		CONST MAT2 Mat = { {0,1}, {0,0}, {0,0}, {0,1} };
	
		//DWORD outline = GetGlyphOutline(hdc, code, GGO_BITMAP, &GM, 0, NULL, &Mat);
		//DWORD outline = GetGlyphOutline(hdc, code, GGO_GRAY2_BITMAP, &GM, 0, NULL, &Mat);
		//DWORD outline = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat);
		DWORD outline = GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &GM, 0, NULL, &Mat);
	
		BYTE *ptr = new BYTE[outline];
		
		//BYTE *ptr = (BYTE*)T2Malloc(sizeof(BYTE) * outline);
		//GetGlyphOutline(hdc, code, GGO_BITMAP, &GM, outline, ptr, &Mat);
		//GetGlyphOutline(hdc, code, GGO_GRAY2_BITMAP, &GM, outline, ptr, &Mat);
		//GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, outline, ptr, &Mat);
		GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &GM, outline, ptr, &Mat);

		// テクスチャ作成
		D3DLOCKED_RECT LockedRect;
		LPDIRECT3DTEXTURE9 texture = NULL;

		//if (FAILED(device->CreateTexture(GM.gmCellIncX, TM.tmHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL)))
		if (FAILED(CGraphicsManager::m_pd3dDevice->CreateTexture(GM.gmCellIncX, TM.tmHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture, NULL)))
		{
			MessageBox(NULL, TEXT(" !! Font Create Err 02 !! "), NULL, MB_OK | MB_ICONSTOP);
		}

		if (FAILED(texture->LockRect(0, &LockedRect, NULL, D3DLOCK_DISCARD)))
		{
			MessageBox(NULL, TEXT(" !! Font Create Err 03 !! "), NULL, MB_OK | MB_ICONSTOP);
		}

		// フォント情報の書き込み
		// iOfs_x, iOfs_y : 書き出し位置(左上)
		// iBmp_w, iBmp_h : フォントビットマップの幅高
		// Level : α値の段階 (GGO_GRAY4_BITMAPは17段階、GGO_GRAY8_BITMAPは65段階)
		int iOfs_x = GM.gmptGlyphOrigin.x;
		int iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
		int iBmp_w = GM.gmBlackBoxX + (4 - (GM.gmBlackBoxX % 4)) % 4;
		int iBmp_h = GM.gmBlackBoxY;
		int Level = 65;

		FillMemory(LockedRect.pBits, LockedRect.Pitch * TM.tmHeight, 0);
		
		for (int y = iOfs_y ; y < iOfs_y+iBmp_h ; y++)
		{
			for (int x = iOfs_x ; x < iOfs_x+iBmp_w ; x++)
			{	
				DWORD Alpha = (DWORD)ptr[x-iOfs_x + iBmp_w * (y-iOfs_y)];
				Alpha = Alpha * (256 / (Level - 1));
				if (Alpha > 255) Alpha = 255;

				// 色は板ポリゴンで設定するため白にする
				DWORD Color = D3DCOLOR_ARGB(Alpha, 255, 255, 255);
				memcpy((BYTE*)LockedRect.pBits + LockedRect.Pitch * y + 4 * x, &Color, sizeof(DWORD));
			}
		}
		
		texture->UnlockRect(0);
		
		delete[] ptr;
		
		// データマップに登録する
		CFont::m_data[c] = texture;
	}
	
	// デバイスコンテキストとフォントハンドルを解放する
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	return true;
}

//描画
void CFont::Render( const CameraBaseWP camera )
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
		CGraphicsManager::m_sdr_use->m_sdr->SetVector( CGraphicsManager::m_sdr_use->m_sdr_color , /*&D3DXVECTOR4( 1 , 1 , 1 , 1 )*/&D3DXVECTOR4( m_col.x , m_col.y , m_col.z , m_col.w ) );
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

//生成
DebugFontSP CDebugFont::Create( const int width , const int height , const int type )
{
	//フォント
	DebugFontSP font = std::make_shared< CDebugFont >();

	//初期化
	font->Init( width , height , type );

	return font;
}

//初期化
void CDebugFont::Init( const int width , const int height , const int type )
{
	//フォント情報
	D3DXFONT_DESC FontInfo;

	FontInfo.Height          = height;
	FontInfo.Width           = width;
    FontInfo.Weight          = 0;		
    FontInfo.MipLevels       = 0;
    FontInfo.Italic          = FALSE;
    FontInfo.CharSet         = SHIFTJIS_CHARSET;
    FontInfo.OutputPrecision = OUT_TT_ONLY_PRECIS;
    FontInfo.Quality         = DEFAULT_QUALITY;
    FontInfo.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
	if( type == 0 )
	{
		lstrcpy( FontInfo.FaceName , L"ＭＳ ゴシック" );
	}
	else
	{
		lstrcpy( FontInfo.FaceName , L"ＭＳ 明朝" );
	}
	
	//フォント作成
	D3DXCreateFontIndirect( CGraphicsManager::m_pd3dDevice , &FontInfo , &m_font );
}

//描画
void CDebugFont::DrawFont( const std::string str , const int posX , const int posY , const FontColor fontcolor )
{
	char buff[ 256 ] = { 0 };
	va_list ap;
 	va_start( ap , str );
	vsprintf( buff , str.c_str() , ap );
	va_end( ap );
	
	//描画する文字列
	WCHAR buff_name[ 256 ] = { 0 };
	//WCHAR型へ変換
	CCommon::DXconvAnsiToWide( buff_name , buff , 256 );

	//描画範囲
	RECT rect;

	rect.left = posX;
	rect.top = posY;
	rect.right = 0;
	rect.bottom = 0;

	HRESULT hr;

	//テキスト描画
	if( SUCCEEDED( CGraphicsManager::m_pd3dDevice->BeginScene() ) )
	{
		m_font->DrawText( NULL ,
						  buff_name ,
						  -1 ,
						  &rect ,
						  DT_LEFT | DT_NOCLIP ,
						  fontcolor
						);

		V( CGraphicsManager::m_pd3dDevice->EndScene() );
	}
}
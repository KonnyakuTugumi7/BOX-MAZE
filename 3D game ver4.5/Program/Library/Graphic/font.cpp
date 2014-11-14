#include "DXUT.h"
#include "font.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Graphic/vertex.h"
#include "../../GameEngine/Graphic/cube.h"
#include "../../GameEngine/Camera/camera_base.h"
#include <cassert>

//生成
FontSP CFont::Create( const int width , const int height , const int type )
{
	//フォント
	FontSP font = std::make_shared< CFont >();

	//初期化
	font->Init( width , height , type );

	return font;
}

//初期化
void CFont::Init( const int width , const int height , const int type )
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
void CFont::DrawFont( const std::string str , const int posX , const int posY , const FontColor fontcolor )
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
#include "DXUT.h"
#include "font.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Graphic/vertex.h"
#include "../../GameEngine/Graphic/cube.h"
#include "../../GameEngine/Camera/camera_base.h"
#include <cassert>

//����
FontSP CFont::Create( const int width , const int height , const int type )
{
	//�t�H���g
	FontSP font = std::make_shared< CFont >();

	//������
	font->Init( width , height , type );

	return font;
}

//������
void CFont::Init( const int width , const int height , const int type )
{
	//�t�H���g���
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
		lstrcpy( FontInfo.FaceName , L"�l�r �S�V�b�N" );
	}
	else
	{
		lstrcpy( FontInfo.FaceName , L"�l�r ����" );
	}
	
	//�t�H���g�쐬
	D3DXCreateFontIndirect( CGraphicsManager::m_pd3dDevice , &FontInfo , &m_font );
}

//�`��
void CFont::DrawFont( const std::string str , const int posX , const int posY , const FontColor fontcolor )
{
	char buff[ 256 ] = { 0 };
	va_list ap;
 	va_start( ap , str );
	vsprintf( buff , str.c_str() , ap );
	va_end( ap );
	
	//�`�悷�镶����
	WCHAR buff_name[ 256 ] = { 0 };
	//WCHAR�^�֕ϊ�
	CCommon::DXconvAnsiToWide( buff_name , buff , 256 );

	//�`��͈�
	RECT rect;

	rect.left = posX;
	rect.top = posY;
	rect.right = 0;
	rect.bottom = 0;

	HRESULT hr;

	//�e�L�X�g�`��
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
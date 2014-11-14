#include "DXUT.h"
#include "texture.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"

//textureクラスの静的メンバの実態
std::map< std::string, TextureSP > CTexture::m_textures ;

//生成
TextureSP CTexture::Create( const std::string& _path )
{
	//管理マップから既にメモリ上に存在しないか重複チェック
	std::map< std::string , TextureSP >::iterator it = m_textures.find( _path );

	//存在しなければ新しくロード
	if( it == m_textures.end() )
	{
		//テクスチャ
		TextureSP texSP = std::make_shared< CTexture >();
		texSP->m_name = _path;
		WCHAR wc_buff[ 255 ] = { 0 };
		CCommon::DXconvAnsiToWide( wc_buff , _path.c_str() , 255 );
		D3DXCreateTextureFromFile( CGraphicsManager::m_pd3dDevice , wc_buff , &texSP->m_directx_tex );

		//管理マップに登録
		m_textures.insert( std::make_pair( _path , texSP ) );

		return texSP;
	}

	//既に存在するならそのテクスチャを返す
	return it->second;
}

//空のテクスチャ作成(多重ロードの可能性アリなので要修正)

TextureSP CTexture::CreateEmptyTex( const DWORD w , const DWORD h )
{
	//テクスチャ
	TextureSP texSP = std::make_shared< CTexture >();
	D3DXCreateTexture( CGraphicsManager::m_pd3dDevice , w , h , 0 , 0 , D3DFMT_A8R8G8B8 , D3DPOOL_MANAGED , &texSP->m_directx_tex ); 
	return texSP;
}

//テクスチャの解放
void CTexture::Destroy( const std::string _name )
{
	std::map< std::string , TextureSP >::iterator it = m_textures.begin();

	//全部解放
	if( _name == "all" )
	{
		while( it != m_textures.end() )
		{
			( *it ).second->m_directx_tex->Release();
			( *it ).second.reset();
			++it;
		}
		//マップをクリア
		m_textures.clear();
	}//テクスチャを１つだけ解放
	else
	{
		it = m_textures.find( _name );
		if( it != m_textures.end() )
		{
			SAFE_RELEASE( ( *it ).second->m_directx_tex );
			( *it ).second.reset();
		}
	}
}


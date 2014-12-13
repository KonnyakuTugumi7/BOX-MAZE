#include "DXUT.h"
#include "texture.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"

//textureクラスの静的メンバの実態
std::map< std::string, TextureSP > CTexture::m_Textures ;

//生成
TextureSP CTexture::Create( const std::string& _path )
{
	//管理マップから既にメモリ上に存在しないか重複チェック
	std::map< std::string , TextureSP >::iterator it = m_Textures.find( _path );

	//存在しなければ新しくロード
	if( it == m_Textures.end() )
	{
		//テクスチャ
		TextureSP texSP = std::make_shared< CTexture >();
		texSP->m_Name = _path;
		WCHAR wc_buff[ 255 ] = { 0 };
		CCommon::DXconvAnsiToWide( wc_buff , _path.c_str() , 255 );
		D3DXCreateTextureFromFile( CGraphicsManager::m_pD3dDevice , wc_buff , &texSP->m_DirectxTex );

		//管理マップに登録
		m_Textures.insert( std::make_pair( _path , texSP ) );

		return texSP;
	}

	//既に存在するならそのテクスチャを返す
	return it->second;
}

//空のテクスチャ作成
TextureSP CTexture::CreateEmptyTex( const DWORD w , const DWORD h )
{
	//名前
	std::stringstream ss;
	ss << w << h;

	//管理マップから既にメモリ上に存在しないか重複チェック
	std::map< std::string , TextureSP >::iterator it = m_Textures.find( ss.str() );

	//存在しなければ新しくロード
	if( it == m_Textures.end() )
	{
		//テクスチャ
		TextureSP texSP = std::make_shared< CTexture >();
		D3DXCreateTexture( CGraphicsManager::m_pD3dDevice , w , h , 0 , 0 , D3DFMT_A8R8G8B8 , D3DPOOL_MANAGED , &texSP->m_DirectxTex ); 
		texSP->m_Name = ss.str();
		//管理マップに登録
		m_Textures.insert( std::make_pair( ss.str() , texSP ) );

		return texSP;
	}

	//既に存在するならそのテクスチャを返す
	return it->second;
}

//テクスチャの解放
void CTexture::Destroy( const std::string _name )
{
	std::map< std::string , TextureSP >::iterator it = m_Textures.begin();

	//全部解放
	if( _name == "all" )
	{
		while( it != m_Textures.end() )
		{
			SAFE_RELEASE( ( *it ).second->m_DirectxTex );
			( *it ).second.reset();
			++it;
		}
		//マップをクリア
		m_Textures.clear();
	}//テクスチャを１つだけ解放
	else
	{
		it = m_Textures.find( _name );
		if( it != m_Textures.end() )
		{
			SAFE_RELEASE( ( *it ).second->m_DirectxTex );
			( *it ).second.reset();
		}
	}
}


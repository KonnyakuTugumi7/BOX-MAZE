#include "DXUT.h"
#include "mesh.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Graphic/vertex.h"

//meshクラスの静的メンバの実態
std::map< std::string , MeshSP > CMesh::m_meshs;

//メッシュを生成
MeshSP CMesh::Create( const std::string& _path )
{
	// 管理マップから既にメモリ上に存在しないか重複チェック
	std::map< std::string, MeshSP >::iterator it = m_meshs.find( _path ) ;

	// 存在しなければ新しくロード
	if( it == m_meshs.end() )
	{
		//メッシュ
		MeshSP meshSP = std::make_shared< CMesh >();
		meshSP->m_name = _path ;
		WCHAR wc_buff[ 255 ] = { 0 } ;

		CCommon::DXconvAnsiToWide( wc_buff, _path.c_str(), 255 ) ;
		D3DXLoadMeshFromX( wc_buff, D3DXMESH_MANAGED, CGraphicsManager::m_pd3dDevice, NULL, &meshSP->m_material_buff, NULL, &meshSP->m_material_num, &meshSP->m_directx_mesh ) ;

		// 管理マップに登録
		m_meshs.insert( std::make_pair( _path , meshSP ) ) ;

		return meshSP;
	}

	//既に存在するならメッシュを返す
	return it->second;
}

//独自の頂点フォーマット(FVF)を用いてメッシュを生成
MeshSP CMesh::CreateFromFVF( const std::string _path )
{
	// 管理マップから既にメモリ上に存在しないか重複チェック
	std::map< std::string, MeshSP >::iterator it = m_meshs.find( _path ) ;

	// 存在しなければ新しくロード
	if( it == m_meshs.end() )
	{
		//メッシュ
		MeshSP meshSP = std::make_shared< CMesh >();
		meshSP->m_name = _path;

		//メッシュ生成	  //メッシュの面の数	//メッシュの頂点の数
		D3DXCreateMeshFVF( 2 , 4 , D3DXMESH_MANAGED , CVertex::FVF , CGraphicsManager::m_pd3dDevice , &meshSP->m_directx_mesh );

		// 管理マップに登録
		m_meshs.insert( std::make_pair( _path , meshSP ) ) ;

		return meshSP;
	}

	//既に存在するならメッシュを返す
	return it->second;
}

// 登録
MeshSP CMesh::Regist( const std::string& _path, MeshSP p_mesh )
{
	// 管理マップから既にメモリ上に存在しないか重複チェック
	std::map< std::string, MeshSP >::iterator it = m_meshs.find( _path ) ;

	// 存在しなければ新しくロード
	if( it == m_meshs.end() )
	{
		// 管理マップに登録
		m_meshs.insert( std::make_pair( _path , p_mesh ) ) ;
	}

	return p_mesh;
}

// メッシュの解放
void CMesh::Destroy( const std::string m_name )
{
	std::map< std::string, MeshSP >::iterator it = m_meshs.begin() ;

	// 全部解放
	if( m_name == "all" )
	{
		while( it != m_meshs.end() )
		{
			SAFE_RELEASE( (it->second)->m_directx_mesh ) ;
			SAFE_RELEASE( (it->second)->m_material_buff ) ;
			it->second.reset();
			m_meshs.erase( it++ ) ;
		}
		//マップをクリア
		m_meshs.clear();
	}//テクスチャを１つだけ解放
	else
	{
		it = m_meshs.find( m_name ) ;
		if( it != m_meshs.end() )
		{
			SAFE_RELEASE( (it->second)->m_directx_mesh ) ;
			SAFE_RELEASE( (it->second)->m_material_buff ) ;
			it->second.reset();
		}
	}
}

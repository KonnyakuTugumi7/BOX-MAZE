#include "DXUT.h"
#include "mesh.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Graphic/vertex.h"

//meshクラスの静的メンバの実態
std::map< std::string , MeshSP > CMesh::m_Meshs;

//メッシュを生成
MeshSP CMesh::Create( const std::string& _path )
{
	// 管理マップから既にメモリ上に存在しないか重複チェック
	std::map< std::string, MeshSP >::iterator it = m_Meshs.find( _path ) ;

	// 存在しなければ新しくロード
	if( it == m_Meshs.end() )
	{
		//メッシュ
		MeshSP meshSP = std::make_shared< CMesh >();
		meshSP->m_Name = _path ;
		WCHAR wc_buff[ 255 ] = { 0 } ;

		CCommon::DXconvAnsiToWide( wc_buff, _path.c_str(), 255 ) ;
		D3DXLoadMeshFromX( wc_buff, D3DXMESH_MANAGED, CGraphicsManager::m_pD3dDevice, NULL, &meshSP->m_MaterialBuff, NULL, &meshSP->m_MaterialNum, &meshSP->m_DirectxMesh ) ;

		// 管理マップに登録
		m_Meshs.insert( std::make_pair( _path , meshSP ) ) ;

		return meshSP;
	}

	//既に存在するならメッシュを返す
	return it->second;
}

//独自の頂点フォーマット(FVF)を用いてメッシュを生成
MeshSP CMesh::CreateFromFVF( const std::string _path )
{
	// 管理マップから既にメモリ上に存在しないか重複チェック
	std::map< std::string, MeshSP >::iterator it = m_Meshs.find( _path ) ;

	// 存在しなければ新しくロード
	if( it == m_Meshs.end() )
	{
		//メッシュ
		MeshSP meshSP = std::make_shared< CMesh >();
		meshSP->m_Name = _path;

		//メッシュ生成	  //メッシュの面の数	//メッシュの頂点の数
		D3DXCreateMeshFVF( 2 , 4 , D3DXMESH_MANAGED , CVertex::FVF , CGraphicsManager::m_pD3dDevice , &meshSP->m_DirectxMesh );

		// 管理マップに登録
		m_Meshs.insert( std::make_pair( _path , meshSP ) ) ;

		return meshSP;
	}

	//既に存在するならメッシュを返す
	return it->second;
}

// 登録
MeshSP CMesh::Regist( const std::string& _path, MeshSP p_mesh )
{
	// 管理マップから既にメモリ上に存在しないか重複チェック
	std::map< std::string, MeshSP >::iterator it = m_Meshs.find( _path ) ;

	// 存在しなければ新しくロード
	if( it == m_Meshs.end() )
	{
		// 管理マップに登録
		m_Meshs.insert( std::make_pair( _path , p_mesh ) ) ;
	}

	return p_mesh;
}

// メッシュの解放
void CMesh::Destroy( const std::string _name )
{
	std::map< std::string, MeshSP >::iterator it = m_Meshs.begin() ;

	// 全部解放
	if( _name == "all" )
	{
		while( it != m_Meshs.end() )
		{
			SAFE_RELEASE( (it->second)->m_DirectxMesh ) ;
			SAFE_RELEASE( (it->second)->m_MaterialBuff ) ;
			it->second.reset();
			m_Meshs.erase( it++ ) ;
		}
		//マップをクリア
		m_Meshs.clear();
	}//テクスチャを１つだけ解放
	else
	{
		it = m_Meshs.find( _name ) ;
		if( it != m_Meshs.end() )
		{
			SAFE_RELEASE( (it->second)->m_DirectxMesh ) ;
			SAFE_RELEASE( (it->second)->m_MaterialBuff ) ;
			it->second.reset();
		}
	}
}

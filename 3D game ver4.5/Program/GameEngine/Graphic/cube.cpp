#include "DXUT.h"
#include "cube.h"
#include "../../Library/Graphic/graphic_base.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../GameEngine/Camera/camera_base.h"
#include "../../GameEngine/Graphic/cube_manager.h"
#include "../../GameEngine/Graphic/board.h"

//生成
CubeSP CCube::Create( const D3DXVECTOR3 length , const std::string file_path )
{
	//キューブ
	CubeSP cube = std::make_shared< CCube >();

	//初期化
	cube->Init( length , file_path );

	//レンダリングリストへの登録
	CGraphicsManager::RegistObj( cube , CGraphicsManager::NORMAL_3D );

	//板ポリにキューブの情報を設定
	auto it = cube->m_BoardVec.begin();
	while( it != cube->m_BoardVec.end() )
	{
		( *it )->SetCubeWP( cube );
		++it;
	}

	return cube;
}

//初期化
void CCube::Init( const D3DXVECTOR3 length , const std::string file_path )
{
	//各辺の長さ
	m_Length = length;

	if( file_path != "" )
	{
		//CSV
		m_Csv = CCsv::Create( file_path );

		//各面の板ポリを作成
		//上辺
		m_BoardVec.push_back( CBoard::Create( m_Length.x , m_Length.z , m_Csv->GetToken< std::string >( 0 , 0 ) ) );
		//左辺
		m_BoardVec.push_back( CBoard::Create( m_Length.z , m_Length.y , m_Csv->GetToken< std::string >( 1 , 0 ) ) );
		//手前
		m_BoardVec.push_back( CBoard::Create( m_Length.x , m_Length.y , m_Csv->GetToken< std::string >( 2 , 0 ) ) );
		//右辺
		m_BoardVec.push_back( CBoard::Create( m_Length.z , m_Length.y , m_Csv->GetToken< std::string >( 3 , 0 ) ) );
		//奥
		m_BoardVec.push_back( CBoard::Create( m_Length.x , m_Length.y , m_Csv->GetToken< std::string >( 4 , 0 ) ) );
		//底辺
		m_BoardVec.push_back( CBoard::Create( m_Length.x , m_Length.z , m_Csv->GetToken< std::string >( 5 , 0 ) ) );
	}
	else
	{
		//各面の板ポリを作成
		//上辺
		m_BoardVec.push_back( CBoard::Create( m_Length.x , m_Length.z ) );
		//左辺
		m_BoardVec.push_back( CBoard::Create( m_Length.z , m_Length.y ) );
		//手前
		m_BoardVec.push_back( CBoard::Create( m_Length.x , m_Length.y ) );
		//右辺
		m_BoardVec.push_back( CBoard::Create( m_Length.z , m_Length.y ) );
		//奥
		m_BoardVec.push_back( CBoard::Create( m_Length.x , m_Length.y ) );
		//底辺
		m_BoardVec.push_back( CBoard::Create( m_Length.x , m_Length.z ) );
	}

	//座標と姿勢
	//上辺
	m_BoardVec[ 0 ]->m_Pos.y = ( m_Length.y / 2 );
	m_BoardVec[ 0 ]->UpdateRotMtx( 90.0f , 0.0f , 0.0f );
	//左辺
	m_BoardVec[ 1 ]->m_Pos.x = -( m_Length.x / 2 );
	m_BoardVec[ 1 ]->UpdateRotMtx( 0.0f , 90.0f , 0.0f );
	//手前
	m_BoardVec[ 2 ]->m_Pos.z = -( m_Length.z / 2 );
	//右辺
	m_BoardVec[ 3 ]->m_Pos.x = ( m_Length.x / 2 );
	m_BoardVec[ 3 ]->UpdateRotMtx( 0.0f , -90.0f , 0.0f );
	//奥
	m_BoardVec[ 4 ]->m_Pos.z = ( m_Length.z / 2 );
	m_BoardVec[ 4 ]->UpdateRotMtx( 0.0f , 180.0f , 0.0f );
	//底辺
	m_BoardVec[ 5 ]->m_Pos.y = -( m_Length.y / 2 );
	m_BoardVec[ 5 ]->UpdateRotMtx( -90.0f , 0.0f , 0.0f );

	//D3DXMATRIX trans_mtx[ 6 ];
	//LPD3DXMESH meshs[ 6 ];
	//for( int i = 0 ; i < 6 ; ++i )
	//{
	//	meshs[ i ] = m_BoardVec[ i ]->GetMeshSP()->m_DirectxMesh;
	//	D3DXMatrixIdentity(&trans_mtx[ i ]);
	//}

	////メッシュの結合
	//D3DXConcatenateMeshes( meshs , 6 , D3DXMESH_MANAGED , NULL , NULL , NULL , CGraphicsManager::m_pD3dDevice , &m_Mesh->m_DirectxMesh );
}

//ロジック
void CCube::Render( const CameraBaseWP camera )
{
	//キューブの種類設定に応じてフラグの設定
	if( m_CubeState == CCubeManager::CUBE_STATE::NON )
	{
		m_IsRender = false;
	}
	else
	{
		m_IsRender = true;
	}

	//ワールドマトリクス設定(ローカル座標からワールド座標へ)
	SetWldMtx();

	//キューブのカラーを板ポリに設定
	for( int i = 0 ; i < m_BoardVec.size() ; ++i )
	{
		m_BoardVec[ i ]->m_Col = m_Col;
	}
}

//板ポリセッター(指定された板ポリのテクスチャを設定する)
void CCube::SetBoardTex( const TextureSP tex , const int num )
{
	m_BoardVec[ num ]->SetTex( tex );
}
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
	auto it = cube->m_board_vec.begin();
	while( it != cube->m_board_vec.end() )
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
	m_length = length;

	if( file_path != "" )
	{
		//CSV
		m_csv = CCsv::Create( file_path );

		//各面の板ポリを作成
		//上辺
		m_board_vec.push_back( CBoard::Create( m_length.x , m_length.z , m_csv->GetToken< std::string >( 0 , 0 ) ) );
		//左辺
		m_board_vec.push_back( CBoard::Create( m_length.z , m_length.y , m_csv->GetToken< std::string >( 1 , 0 ) ) );
		//手前
		m_board_vec.push_back( CBoard::Create( m_length.x , m_length.y , m_csv->GetToken< std::string >( 2 , 0 ) ) );
		//右辺
		m_board_vec.push_back( CBoard::Create( m_length.z , m_length.y , m_csv->GetToken< std::string >( 3 , 0 ) ) );
		//奥
		m_board_vec.push_back( CBoard::Create( m_length.x , m_length.y , m_csv->GetToken< std::string >( 4 , 0 ) ) );
		//底辺
		m_board_vec.push_back( CBoard::Create( m_length.x , m_length.z , m_csv->GetToken< std::string >( 5 , 0 ) ) );
	}
	else
	{
		//各面の板ポリを作成
		//上辺
		m_board_vec.push_back( CBoard::Create( m_length.x , m_length.z ) );
		//左辺
		m_board_vec.push_back( CBoard::Create( m_length.z , m_length.y ) );
		//手前
		m_board_vec.push_back( CBoard::Create( m_length.x , m_length.y ) );
		//右辺
		m_board_vec.push_back( CBoard::Create( m_length.z , m_length.y ) );
		//奥
		m_board_vec.push_back( CBoard::Create( m_length.x , m_length.y ) );
		//底辺
		m_board_vec.push_back( CBoard::Create( m_length.x , m_length.z ) );
	}

	//座標と姿勢
	//上辺
	m_board_vec[ 0 ]->m_pos.y = ( m_length.y / 2 );
	m_board_vec[ 0 ]->UpdateRotMtx( 90.0f , 0.0f , 0.0f );
	//左辺
	m_board_vec[ 1 ]->m_pos.x = -( m_length.x / 2 );
	m_board_vec[ 1 ]->UpdateRotMtx( 0.0f , 90.0f , 0.0f );
	//手前
	m_board_vec[ 2 ]->m_pos.z = -( m_length.z / 2 );
	//右辺
	m_board_vec[ 3 ]->m_pos.x = ( m_length.x / 2 );
	m_board_vec[ 3 ]->UpdateRotMtx( 0.0f , -90.0f , 0.0f );
	//奥
	m_board_vec[ 4 ]->m_pos.z = ( m_length.z / 2 );
	m_board_vec[ 4 ]->UpdateRotMtx( 0.0f , 180.0f , 0.0f );
	//底辺
	m_board_vec[ 5 ]->m_pos.y = -( m_length.y / 2 );
	m_board_vec[ 5 ]->UpdateRotMtx( -90.0f , 0.0f , 0.0f );

	//D3DXMATRIX trans_mtx[ 6 ];
	//LPD3DXMESH meshs[ 6 ];
	//for( int i = 0 ; i < 6 ; ++i )
	//{
	//	meshs[ i ] = m_board_vec[ i ]->GetMeshSP()->m_directx_mesh;
	//	D3DXMatrixIdentity(&trans_mtx[ i ]);
	//}

	////メッシュの結合
	//D3DXConcatenateMeshes( meshs , 6 , D3DXMESH_MANAGED , NULL , NULL , NULL , CGraphicsManager::m_pd3dDevice , &m_mesh->m_directx_mesh );
}

//ロジック
void CCube::Render( const CameraBaseWP camera )
{
	//キューブの種類設定に応じてフラグの設定
	if( m_cube_state == CCubeManager::CUBE_STATE::NON )
	{
		m_is_render = false;
	}
	else
	{
		m_is_render = true;
	}

	//ワールドマトリクス設定(ローカル座標からワールド座標へ)
	SetWldMtx();
}

//板ポリセッター(指定された板ポリのテクスチャを設定する)
void CCube::SetBoardTex( const TextureSP tex , const int num )
{
	m_board_vec[ num ]->SetTex( tex );
}
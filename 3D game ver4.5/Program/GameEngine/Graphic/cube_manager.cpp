#include "DXUT.h"
#include "cube_manager.h"

//コンストラクタ
CCubeManager::CCubeManager( const std::string file_path )
{
	//ステージ名保存
	m_stage_name = file_path;

	//CSV読み込み
	m_csv_platinum = CCsvPlatinum::Create( m_stage_name );

	//CSVからブロックを作成
	//X
	for( int x = 0 ; x < m_csv_platinum->GetMapWidth() ; x++ )
	{
		CUBE2 cube2;
		std::vector< std::vector< D3DXVECTOR3 > > pos2;
		//Y
		for( int y = 0 ; y < m_csv_platinum->GetLayerNum() ; y++ )
		{
			CUBE1 cube1;
			std::vector< D3DXVECTOR3 > pos1;
			//Z
			for( int z = 0 ; z < m_csv_platinum->GetMapDepth() ; z++ )
			{
				//ブロックなし
				if( m_csv_platinum->GetToken< int >( m_csv_platinum->GetLayerIdx( y ) + z , x ) == CUBE_STATE::NON ) 
				{
					CubeSP cube = CCube::Create( D3DXVECTOR3( 5 , 5 , 5 ) );
					cube->m_pos = D3DXVECTOR3( x * 5 , y * 5 , z * 5 );
					pos1.push_back( D3DXVECTOR3( x * 5 , y * 5 , z * 5 ) );
					cube->SetIsRender( false );
					cube->SetIsCollision( false );
					cube->SetCubeState( CUBE_STATE::NON );
					cube1.push_back( cube );
				}

				//スタートブロック
				if( m_csv_platinum->GetToken< int >( m_csv_platinum->GetLayerIdx( y ) + z , x ) == CUBE_STATE::START )
				{
					CubeSP cube = CCube::Create( D3DXVECTOR3( 5 , 5 , 5 ) , "Content/csv/CubeData01.csv" );
					cube->m_pos = D3DXVECTOR3( x * 5 , y * 5 , z * 5 );
					pos1.push_back( D3DXVECTOR3( x * 5 , y * 5 , z * 5 ) );
					m_start_pos = D3DXVECTOR3( x * 5 , y * 5 + 15 , z * 5 );
					cube->SetCubeState( CUBE_STATE::START );
					cube1.push_back( cube );
				}
				//ゴールブロック
				else if( m_csv_platinum->GetToken< int >( m_csv_platinum->GetLayerIdx( y ) + z , x ) == CUBE_STATE::GOAL )
				{
					CubeSP cube = CCube::Create( D3DXVECTOR3( 5 , 5 , 5 ) , "Content/csv/CubeData02.csv" );
					cube->m_pos = D3DXVECTOR3( x * 5 , y * 5 , z * 5 );
					pos1.push_back( D3DXVECTOR3( x * 5 , y * 5 , z * 5 ) );
					m_goal_pos = D3DXVECTOR3( x * 5 , y * 5 + 15 , z * 5 );
					cube->SetCubeState( CUBE_STATE::GOAL );
					cube1.push_back( cube );
				}
				//ノーマルブロック
				else if( m_csv_platinum->GetToken< int >( m_csv_platinum->GetLayerIdx( y ) + z , x ) == CUBE_STATE::NORMAL )
				{
					CubeSP cube = CCube::Create( D3DXVECTOR3( 5 , 5 , 5 ) , "Content/csv/CubeData03.csv" );
					cube->m_pos = D3DXVECTOR3( x * 5 , y * 5 , z * 5 );
					pos1.push_back( D3DXVECTOR3( x * 5 , y * 5 , z * 5 ) );
					cube->SetCubeState( CUBE_STATE::NORMAL );
					cube1.push_back( cube );
				}
				else
				{
					//例外
				}
			}
			cube2.push_back( cube1 );
			pos2.push_back( pos1 );
		}
		m_cubes.push_back( cube2 );
		m_cubes_pos.push_back( pos2 );
	}
}

//指定した種類のキューブがいくつあるか検索
int CCubeManager::FindSpecifiedCubeNum( const CUBE_STATE cube )
{
	int num = 0;
	//検索
	for( int x = 0 ; x < m_cubes.size() ; ++x )
	{
		for( int y = 0 ; y < m_cubes[ x ].size() ; ++y )
		{
			for( int z = 0 ; z < m_cubes[ x ][ y ].size() ; ++z )
			{
				if( m_cubes[ x ][ y ][ z ]->GetCubeState() == cube ) ++num;
			}
		}
	}
	return num;
}
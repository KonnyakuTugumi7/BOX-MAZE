#include "DXUT.h"
#include "cube_manager.h"

//コンストラクタ
CCubeManager::CCubeManager( const std::string file_path )
{
	//ステージ名保存
	m_StageName = file_path;

	//CSV読み込み
	m_CsvPlatinum = CCsvPlatinum::Create( m_StageName );

	//CSVからブロックを作成
	//X
	for( int x = 0 ; x < m_CsvPlatinum->GetMapWidth() ; x++ )
	{
		CUBE2 cube2;
		std::vector< std::vector< D3DXVECTOR3 > > pos2;
		//Y
		for( int y = 0 ; y < m_CsvPlatinum->GetLayerNum() ; y++ )
		{
			CUBE1 cube1;
			std::vector< D3DXVECTOR3 > pos1;
			//Z
			for( int z = 0 ; z < m_CsvPlatinum->GetMapDepth() ; z++ )
			{
				//ブロックなし
				if( m_CsvPlatinum->GetToken< int >( m_CsvPlatinum->GetLayerIdx( y ) + z , x ) == CUBE_STATE::NON ) 
				{
					CubeSP cube = CCube::Create( D3DXVECTOR3( 5 , 5 , 5 ) );
					cube->m_Pos = D3DXVECTOR3( x * 5 , y * 5 , z * 5 );
					pos1.push_back( D3DXVECTOR3( x * 5 , y * 5 , z * 5 ) );
					cube->SetIsRender( false );
					cube->SetIsCollision( false );
					cube->SetCubeState( CUBE_STATE::NON );
					cube1.push_back( cube );
				}

				//スタートブロック
				if( m_CsvPlatinum->GetToken< int >( m_CsvPlatinum->GetLayerIdx( y ) + z , x ) == CUBE_STATE::START )
				{
					CubeSP cube = CCube::Create( D3DXVECTOR3( 5 , 5 , 5 ) , "Content/csv/CubeData01.csv" );
					cube->m_Pos = D3DXVECTOR3( x * 5 , y * 5 , z * 5 );
					pos1.push_back( D3DXVECTOR3( x * 5 , y * 5 , z * 5 ) );
					m_StartPos = D3DXVECTOR3( x * 5 , y * 5 + 15 , z * 5 );
					cube->SetCubeState( CUBE_STATE::START );
					cube1.push_back( cube );
				}
				//ゴールブロック
				else if( m_CsvPlatinum->GetToken< int >( m_CsvPlatinum->GetLayerIdx( y ) + z , x ) == CUBE_STATE::GOAL )
				{
					CubeSP cube = CCube::Create( D3DXVECTOR3( 5 , 5 , 5 ) , "Content/csv/CubeData02.csv" );
					cube->m_Pos = D3DXVECTOR3( x * 5 , y * 5 , z * 5 );
					pos1.push_back( D3DXVECTOR3( x * 5 , y * 5 , z * 5 ) );
					m_GoalPos = D3DXVECTOR3( x * 5 , y * 5 + 15 , z * 5 );
					cube->SetCubeState( CUBE_STATE::GOAL );
					cube1.push_back( cube );
				}
				//ノーマルブロック
				else if( m_CsvPlatinum->GetToken< int >( m_CsvPlatinum->GetLayerIdx( y ) + z , x ) == CUBE_STATE::NORMAL )
				{
					CubeSP cube = CCube::Create( D3DXVECTOR3( 5 , 5 , 5 ) , "Content/csv/CubeData03.csv" );
					cube->m_Pos = D3DXVECTOR3( x * 5 , y * 5 , z * 5 );
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
		m_Cubes.push_back( cube2 );
		m_CubesPos.push_back( pos2 );
	}
}

//指定した種類のキューブがいくつあるか検索
int CCubeManager::FindSpecifiedCubeNum( const CUBE_STATE cube )
{
	int num = 0;
	//検索
	for( int x = 0 ; x < m_Cubes.size() ; ++x )
	{
		for( int y = 0 ; y < m_Cubes[ x ].size() ; ++y )
		{
			for( int z = 0 ; z < m_Cubes[ x ][ y ].size() ; ++z )
			{
				if( m_Cubes[ x ][ y ][ z ]->GetCubeState() == cube ) ++num;
			}
		}
	}
	return num;
}
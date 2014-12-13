#include "DXUT.h"
#include "cube_manager.h"

//�R���X�g���N�^
CCubeManager::CCubeManager( const std::string file_path )
{
	//�X�e�[�W���ۑ�
	m_StageName = file_path;

	//CSV�ǂݍ���
	m_CsvPlatinum = CCsvPlatinum::Create( m_StageName );

	//CSV����u���b�N���쐬
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
				//�u���b�N�Ȃ�
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

				//�X�^�[�g�u���b�N
				if( m_CsvPlatinum->GetToken< int >( m_CsvPlatinum->GetLayerIdx( y ) + z , x ) == CUBE_STATE::START )
				{
					CubeSP cube = CCube::Create( D3DXVECTOR3( 5 , 5 , 5 ) , "Content/csv/CubeData01.csv" );
					cube->m_Pos = D3DXVECTOR3( x * 5 , y * 5 , z * 5 );
					pos1.push_back( D3DXVECTOR3( x * 5 , y * 5 , z * 5 ) );
					m_StartPos = D3DXVECTOR3( x * 5 , y * 5 + 15 , z * 5 );
					cube->SetCubeState( CUBE_STATE::START );
					cube1.push_back( cube );
				}
				//�S�[���u���b�N
				else if( m_CsvPlatinum->GetToken< int >( m_CsvPlatinum->GetLayerIdx( y ) + z , x ) == CUBE_STATE::GOAL )
				{
					CubeSP cube = CCube::Create( D3DXVECTOR3( 5 , 5 , 5 ) , "Content/csv/CubeData02.csv" );
					cube->m_Pos = D3DXVECTOR3( x * 5 , y * 5 , z * 5 );
					pos1.push_back( D3DXVECTOR3( x * 5 , y * 5 , z * 5 ) );
					m_GoalPos = D3DXVECTOR3( x * 5 , y * 5 + 15 , z * 5 );
					cube->SetCubeState( CUBE_STATE::GOAL );
					cube1.push_back( cube );
				}
				//�m�[�}���u���b�N
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
					//��O
				}
			}
			cube2.push_back( cube1 );
			pos2.push_back( pos1 );
		}
		m_Cubes.push_back( cube2 );
		m_CubesPos.push_back( pos2 );
	}
}

//�w�肵����ނ̃L���[�u���������邩����
int CCubeManager::FindSpecifiedCubeNum( const CUBE_STATE cube )
{
	int num = 0;
	//����
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
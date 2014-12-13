#include "DXUT.h"
#include "cube.h"
#include "../../Library/Graphic/graphic_base.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../GameEngine/Camera/camera_base.h"
#include "../../GameEngine/Graphic/cube_manager.h"
#include "../../GameEngine/Graphic/board.h"

//����
CubeSP CCube::Create( const D3DXVECTOR3 length , const std::string file_path )
{
	//�L���[�u
	CubeSP cube = std::make_shared< CCube >();

	//������
	cube->Init( length , file_path );

	//�����_�����O���X�g�ւ̓o�^
	CGraphicsManager::RegistObj( cube , CGraphicsManager::NORMAL_3D );

	//�|���ɃL���[�u�̏���ݒ�
	auto it = cube->m_BoardVec.begin();
	while( it != cube->m_BoardVec.end() )
	{
		( *it )->SetCubeWP( cube );
		++it;
	}

	return cube;
}

//������
void CCube::Init( const D3DXVECTOR3 length , const std::string file_path )
{
	//�e�ӂ̒���
	m_Length = length;

	if( file_path != "" )
	{
		//CSV
		m_Csv = CCsv::Create( file_path );

		//�e�ʂ̔|�����쐬
		//���
		m_BoardVec.push_back( CBoard::Create( m_Length.x , m_Length.z , m_Csv->GetToken< std::string >( 0 , 0 ) ) );
		//����
		m_BoardVec.push_back( CBoard::Create( m_Length.z , m_Length.y , m_Csv->GetToken< std::string >( 1 , 0 ) ) );
		//��O
		m_BoardVec.push_back( CBoard::Create( m_Length.x , m_Length.y , m_Csv->GetToken< std::string >( 2 , 0 ) ) );
		//�E��
		m_BoardVec.push_back( CBoard::Create( m_Length.z , m_Length.y , m_Csv->GetToken< std::string >( 3 , 0 ) ) );
		//��
		m_BoardVec.push_back( CBoard::Create( m_Length.x , m_Length.y , m_Csv->GetToken< std::string >( 4 , 0 ) ) );
		//���
		m_BoardVec.push_back( CBoard::Create( m_Length.x , m_Length.z , m_Csv->GetToken< std::string >( 5 , 0 ) ) );
	}
	else
	{
		//�e�ʂ̔|�����쐬
		//���
		m_BoardVec.push_back( CBoard::Create( m_Length.x , m_Length.z ) );
		//����
		m_BoardVec.push_back( CBoard::Create( m_Length.z , m_Length.y ) );
		//��O
		m_BoardVec.push_back( CBoard::Create( m_Length.x , m_Length.y ) );
		//�E��
		m_BoardVec.push_back( CBoard::Create( m_Length.z , m_Length.y ) );
		//��
		m_BoardVec.push_back( CBoard::Create( m_Length.x , m_Length.y ) );
		//���
		m_BoardVec.push_back( CBoard::Create( m_Length.x , m_Length.z ) );
	}

	//���W�Ǝp��
	//���
	m_BoardVec[ 0 ]->m_Pos.y = ( m_Length.y / 2 );
	m_BoardVec[ 0 ]->UpdateRotMtx( 90.0f , 0.0f , 0.0f );
	//����
	m_BoardVec[ 1 ]->m_Pos.x = -( m_Length.x / 2 );
	m_BoardVec[ 1 ]->UpdateRotMtx( 0.0f , 90.0f , 0.0f );
	//��O
	m_BoardVec[ 2 ]->m_Pos.z = -( m_Length.z / 2 );
	//�E��
	m_BoardVec[ 3 ]->m_Pos.x = ( m_Length.x / 2 );
	m_BoardVec[ 3 ]->UpdateRotMtx( 0.0f , -90.0f , 0.0f );
	//��
	m_BoardVec[ 4 ]->m_Pos.z = ( m_Length.z / 2 );
	m_BoardVec[ 4 ]->UpdateRotMtx( 0.0f , 180.0f , 0.0f );
	//���
	m_BoardVec[ 5 ]->m_Pos.y = -( m_Length.y / 2 );
	m_BoardVec[ 5 ]->UpdateRotMtx( -90.0f , 0.0f , 0.0f );

	//D3DXMATRIX trans_mtx[ 6 ];
	//LPD3DXMESH meshs[ 6 ];
	//for( int i = 0 ; i < 6 ; ++i )
	//{
	//	meshs[ i ] = m_BoardVec[ i ]->GetMeshSP()->m_DirectxMesh;
	//	D3DXMatrixIdentity(&trans_mtx[ i ]);
	//}

	////���b�V���̌���
	//D3DXConcatenateMeshes( meshs , 6 , D3DXMESH_MANAGED , NULL , NULL , NULL , CGraphicsManager::m_pD3dDevice , &m_Mesh->m_DirectxMesh );
}

//���W�b�N
void CCube::Render( const CameraBaseWP camera )
{
	//�L���[�u�̎�ސݒ�ɉ����ăt���O�̐ݒ�
	if( m_CubeState == CCubeManager::CUBE_STATE::NON )
	{
		m_IsRender = false;
	}
	else
	{
		m_IsRender = true;
	}

	//���[���h�}�g���N�X�ݒ�(���[�J�����W���烏�[���h���W��)
	SetWldMtx();

	//�L���[�u�̃J���[��|���ɐݒ�
	for( int i = 0 ; i < m_BoardVec.size() ; ++i )
	{
		m_BoardVec[ i ]->m_Col = m_Col;
	}
}

//�|���Z�b�^�[(�w�肳�ꂽ�|���̃e�N�X�`����ݒ肷��)
void CCube::SetBoardTex( const TextureSP tex , const int num )
{
	m_BoardVec[ num ]->SetTex( tex );
}
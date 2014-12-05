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
	auto it = cube->m_board_vec.begin();
	while( it != cube->m_board_vec.end() )
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
	m_length = length;

	if( file_path != "" )
	{
		//CSV
		m_csv = CCsv::Create( file_path );

		//�e�ʂ̔|�����쐬
		//���
		m_board_vec.push_back( CBoard::Create( m_length.x , m_length.z , m_csv->GetToken< std::string >( 0 , 0 ) ) );
		//����
		m_board_vec.push_back( CBoard::Create( m_length.z , m_length.y , m_csv->GetToken< std::string >( 1 , 0 ) ) );
		//��O
		m_board_vec.push_back( CBoard::Create( m_length.x , m_length.y , m_csv->GetToken< std::string >( 2 , 0 ) ) );
		//�E��
		m_board_vec.push_back( CBoard::Create( m_length.z , m_length.y , m_csv->GetToken< std::string >( 3 , 0 ) ) );
		//��
		m_board_vec.push_back( CBoard::Create( m_length.x , m_length.y , m_csv->GetToken< std::string >( 4 , 0 ) ) );
		//���
		m_board_vec.push_back( CBoard::Create( m_length.x , m_length.z , m_csv->GetToken< std::string >( 5 , 0 ) ) );
	}
	else
	{
		//�e�ʂ̔|�����쐬
		//���
		m_board_vec.push_back( CBoard::Create( m_length.x , m_length.z ) );
		//����
		m_board_vec.push_back( CBoard::Create( m_length.z , m_length.y ) );
		//��O
		m_board_vec.push_back( CBoard::Create( m_length.x , m_length.y ) );
		//�E��
		m_board_vec.push_back( CBoard::Create( m_length.z , m_length.y ) );
		//��
		m_board_vec.push_back( CBoard::Create( m_length.x , m_length.y ) );
		//���
		m_board_vec.push_back( CBoard::Create( m_length.x , m_length.z ) );
	}

	//���W�Ǝp��
	//���
	m_board_vec[ 0 ]->m_pos.y = ( m_length.y / 2 );
	m_board_vec[ 0 ]->UpdateRotMtx( 90.0f , 0.0f , 0.0f );
	//����
	m_board_vec[ 1 ]->m_pos.x = -( m_length.x / 2 );
	m_board_vec[ 1 ]->UpdateRotMtx( 0.0f , 90.0f , 0.0f );
	//��O
	m_board_vec[ 2 ]->m_pos.z = -( m_length.z / 2 );
	//�E��
	m_board_vec[ 3 ]->m_pos.x = ( m_length.x / 2 );
	m_board_vec[ 3 ]->UpdateRotMtx( 0.0f , -90.0f , 0.0f );
	//��
	m_board_vec[ 4 ]->m_pos.z = ( m_length.z / 2 );
	m_board_vec[ 4 ]->UpdateRotMtx( 0.0f , 180.0f , 0.0f );
	//���
	m_board_vec[ 5 ]->m_pos.y = -( m_length.y / 2 );
	m_board_vec[ 5 ]->UpdateRotMtx( -90.0f , 0.0f , 0.0f );

	//D3DXMATRIX trans_mtx[ 6 ];
	//LPD3DXMESH meshs[ 6 ];
	//for( int i = 0 ; i < 6 ; ++i )
	//{
	//	meshs[ i ] = m_board_vec[ i ]->GetMeshSP()->m_directx_mesh;
	//	D3DXMatrixIdentity(&trans_mtx[ i ]);
	//}

	////���b�V���̌���
	//D3DXConcatenateMeshes( meshs , 6 , D3DXMESH_MANAGED , NULL , NULL , NULL , CGraphicsManager::m_pd3dDevice , &m_mesh->m_directx_mesh );
}

//���W�b�N
void CCube::Render( const CameraBaseWP camera )
{
	//�L���[�u�̎�ސݒ�ɉ����ăt���O�̐ݒ�
	if( m_cube_state == CCubeManager::CUBE_STATE::NON )
	{
		m_is_render = false;
	}
	else
	{
		m_is_render = true;
	}

	//���[���h�}�g���N�X�ݒ�(���[�J�����W���烏�[���h���W��)
	SetWldMtx();
}

//�|���Z�b�^�[(�w�肳�ꂽ�|���̃e�N�X�`����ݒ肷��)
void CCube::SetBoardTex( const TextureSP tex , const int num )
{
	m_board_vec[ num ]->SetTex( tex );
}
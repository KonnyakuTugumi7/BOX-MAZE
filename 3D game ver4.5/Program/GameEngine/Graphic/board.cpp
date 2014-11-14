#include "DXUT.h"
#include "board.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Graphic/texture.h"
#include "../../Library/Graphic/vertex.h"
#include "../../Library/Graphic/mesh.h"
#include "../../GameEngine/Graphic/cube.h"
#include "../../GameEngine/Camera/camera_base.h"

//����
BoardSP CBoard::Create( const int width , const int height , const std::string path  , const CGraphicsManager::RENDERLIST_STATE list )
{
	//�{�[�h
	BoardSP board = std::make_shared< CBoard >();

	//������
	board->Init( width , height , path );

	//�����_�����O���X�g�ւ̓o�^
	CGraphicsManager::RegistObj( board , list );

	return board;
}

//������
void CBoard::Init( const int width , const int height , const std::string path )
{
	//�r���{�[�h�t���O
	m_is_billboard = false;

	//�J�����O�ݒ�(�J�����O���Ȃ�)
	//m_cull_state = D3DCULL_NONE;

	if( path == "" )
	{
		//�F
		m_col.x = 0.0f;
		m_col.y = 0.0f;
		m_col.z = 1.0f;
		m_col.w = 0.5f;
	}

	//�|���T�C�Y
	m_board_size.x = width;
	m_board_size.y = height;
	m_board_size.z = 0;

	//�V�F�[�_
	m_sdr_state = SDR_BASE;

	//���O
	std::stringstream ss;
	ss << path << width << height;

	//�e�N�X�`���쐬
	if( path != "" ) m_texs.push_back( CTexture::Create( path ) );

	//���b�V���쐬(���O�͎g���e�N�X�`���̃p�X�ƕ��ƍ��������킹������)
	m_mesh = CMesh::CreateFromFVF( ss.str() );

	//�C���f�b�N�X�f�[�^�쐬
	WORD* pIndexs;

	m_mesh->GetMesh()->LockIndexBuffer( 0 , ( void ** )&pIndexs );
	pIndexs[ 0 ] = 0;
	pIndexs[ 1 ] = 1;
	pIndexs[ 2 ] = 2;
					 
	pIndexs[ 3 ] = 1;
	pIndexs[ 4 ] = 3;
	pIndexs[ 5 ] = 2;
	m_mesh->GetMesh()->UnlockIndexBuffer();

	//���_�f�[�^�쐬
	CVertex* pVertex;

	m_mesh->GetMesh()->LockVertexBuffer( 0 , ( void** )&pVertex );
	pVertex[ 0 ].m_pos = D3DXVECTOR3( -m_board_size.x / 2 , m_board_size.y / 2 , 0 );
	pVertex[ 1 ].m_pos = D3DXVECTOR3( m_board_size.x / 2 , m_board_size.y / 2 , 0 );
	pVertex[ 2 ].m_pos = D3DXVECTOR3( -m_board_size.x / 2 , -m_board_size.y / 2 , 0 );
	pVertex[ 3 ].m_pos = D3DXVECTOR3( m_board_size.x / 2 , -m_board_size.y / 2 , 0 );

	pVertex[ 0 ].m_nml = D3DXVECTOR3( 0 , 0 , -1 );
	pVertex[ 1 ].m_nml = D3DXVECTOR3( 0 , 0 , -1 );
	pVertex[ 2 ].m_nml = D3DXVECTOR3( 0 , 0 , -1 );
	pVertex[ 3 ].m_nml = D3DXVECTOR3( 0 , 0 , -1 );

	pVertex[ 0 ].m_uv = D3DXVECTOR2( 0.0f , 0.0f );
	pVertex[ 1 ].m_uv = D3DXVECTOR2( 1.0f , 0.0f );
	pVertex[ 2 ].m_uv = D3DXVECTOR2( 0.0f , 1.0f );
	pVertex[ 3 ].m_uv = D3DXVECTOR2( 1.0f , 1.0f );
	m_mesh->GetMesh()->UnlockIndexBuffer();
}

//�`��
void CBoard::Render( const CameraBaseWP camera )
{
	HRESULT hr;

	//���[���h�}�g���N�X�ݒ�(���[�J�����W���烏�[���h���W��)
	if( m_is_billboard == true )
	{
		SetWldMtxBillBoard( camera );
	}
	else
	{
		//�e(cube)�����݂���ꍇ�͐e�̃��[���h�}�g���b�N�X��n��
		m_cube.expired() ? SetWldMtx() : SetWldMtx( &m_cube.lock()->GetWldMtx() );
	}
	m_wld_mtx = m_wld_mtx * camera.lock()->GetViewMatrix() * camera.lock()->GetProjectionMatrix();

	//�e�����݂��Ă��e�̕`��t���O���I�t�Ȃ�ȉ��̏������s��Ȃ�
	if( m_cube.expired() == false && m_cube.lock()->GetIsRender() == false ) return;

	//�J�����O�ݒ�
	CGraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_CULLMODE , m_cull_state );

	//�V�F�[�_�ݒ�
	SetSDR();

	//----------------------------�`�揈��-------------------------------

	//�V�F�[�_�Ƀe�N�X�`����n��
	if( 0 != m_texs.size() )
	{
		CGraphicsManager::m_sdr_use->m_sdr->SetTexture( CGraphicsManager::m_sdr_use->m_sdr_tex , m_texs[ 0 ]->GetTex() );
		CGraphicsManager::m_sdr_use->m_sdr->SetVector( CGraphicsManager::m_sdr_use->m_sdr_color , &D3DXVECTOR4( 1 , 1 , 1 , 1 ) );
	}
	//�e�N�X�`����������΃J���[��n��
	else
	{
		CGraphicsManager::m_sdr_use->m_sdr->SetVector( CGraphicsManager::m_sdr_use->m_sdr_color , &D3DXVECTOR4( m_col.x , m_col.y , m_col.z , m_col.w ) );
	}

	//�V�F�[�_�̎g�p�J�n
	CGraphicsManager::m_sdr_use->Begin();

	// �V�F�[�_�̃p�X�ݒ�
	if( 0 != m_texs.size() )
	{
		if( !m_is_add_blend )
		{
			CGraphicsManager::m_sdr_use->BeginPass( 0 );
		}
		else
		{
			CGraphicsManager::m_sdr_use->BeginPass( 1 );
		}
	}
	else
	{
		CGraphicsManager::m_sdr_use->BeginPass( 2 );
	}

	if( SUCCEEDED( CGraphicsManager::m_pd3dDevice->BeginScene() ) )
	{
		if(D3D_OK != m_mesh->GetMesh()->DrawSubset( 0 ))
		{
			return;
		}
		V( CGraphicsManager::m_pd3dDevice->EndScene() );
	}

	//�p�X�I��
	CGraphicsManager::m_sdr_use->EndPass();

	//�V�F�[�_�I��
	CGraphicsManager::m_sdr_use->End();
}
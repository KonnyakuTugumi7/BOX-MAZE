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
	m_IsBillboard = false;

	//�J�����O�ݒ�(�J�����O���Ȃ�)
	//m_CullState = D3DCULL_NONE;

	if( path == "" )
	{
		//�F
		m_Col.x = 0.0f;
		m_Col.y = 0.0f;
		m_Col.z = 1.0f;
		m_Col.w = 0.5f;
	}

	//�|���T�C�Y
	m_BoardSize.x = width;
	m_BoardSize.y = height;
	m_BoardSize.z = 0;

	//�V�F�[�_
	m_sdr_state = SDR_BASE;

	//���O
	std::stringstream ss;
	ss << path << width << height;

	//�e�N�X�`���쐬
	if( path != "" ) m_Texs.push_back( CTexture::Create( path ) );

	//���b�V���쐬(���O�͎g���e�N�X�`���̃p�X�ƕ��ƍ��������킹������)
	m_Mesh = CMesh::CreateFromFVF( ss.str() );

	//�C���f�b�N�X�f�[�^�쐬
	WORD* pIndexs;

	m_Mesh->GetMesh()->LockIndexBuffer( 0 , ( void ** )&pIndexs );
	pIndexs[ 0 ] = 0;
	pIndexs[ 1 ] = 1;
	pIndexs[ 2 ] = 2;
					 
	pIndexs[ 3 ] = 1;
	pIndexs[ 4 ] = 3;
	pIndexs[ 5 ] = 2;
	m_Mesh->GetMesh()->UnlockIndexBuffer();

	//���_�f�[�^�쐬
	CVertex* pVertex;

	m_Mesh->GetMesh()->LockVertexBuffer( 0 , ( void** )&pVertex );
	pVertex[ 0 ].m_Pos = D3DXVECTOR3( -m_BoardSize.x / 2 , m_BoardSize.y / 2 , 0 );
	pVertex[ 1 ].m_Pos = D3DXVECTOR3( m_BoardSize.x / 2 , m_BoardSize.y / 2 , 0 );
	pVertex[ 2 ].m_Pos = D3DXVECTOR3( -m_BoardSize.x / 2 , -m_BoardSize.y / 2 , 0 );
	pVertex[ 3 ].m_Pos = D3DXVECTOR3( m_BoardSize.x / 2 , -m_BoardSize.y / 2 , 0 );

	pVertex[ 0 ].m_Nml = D3DXVECTOR3( 0 , 0 , -1 );
	pVertex[ 1 ].m_Nml = D3DXVECTOR3( 0 , 0 , -1 );
	pVertex[ 2 ].m_Nml = D3DXVECTOR3( 0 , 0 , -1 );
	pVertex[ 3 ].m_Nml = D3DXVECTOR3( 0 , 0 , -1 );

	pVertex[ 0 ].m_Uv = D3DXVECTOR2( 0.0f , 0.0f );
	pVertex[ 1 ].m_Uv = D3DXVECTOR2( 1.0f , 0.0f );
	pVertex[ 2 ].m_Uv = D3DXVECTOR2( 0.0f , 1.0f );
	pVertex[ 3 ].m_Uv = D3DXVECTOR2( 1.0f , 1.0f );
	m_Mesh->GetMesh()->UnlockIndexBuffer();
}

//�`��
void CBoard::Render( const CameraBaseWP camera )
{
	HRESULT hr;

	//���[���h�}�g���N�X�ݒ�(���[�J�����W���烏�[���h���W��)
	if( m_IsBillboard == true )
	{
		SetWldMtxBillBoard( camera );
	}
	else
	{
		//�e(cube)�����݂���ꍇ�͐e�̃��[���h�}�g���b�N�X��n��
		m_Cube.expired() ? SetWldMtx() : SetWldMtx( &m_Cube.lock()->GetWldMtx() );
	}
	m_WldMtx = m_WldMtx * camera.lock()->GetViewMatrix() * camera.lock()->GetProjectionMatrix();

	//�e�����݂��Ă��e�̕`��t���O���I�t�Ȃ�ȉ��̏������s��Ȃ�
	if( m_Cube.expired() == false && m_Cube.lock()->GetIsRender() == false ) return;

	//�J�����O�ݒ�
	CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_CULLMODE , m_CullState );

	//�V�F�[�_�ݒ�
	SetSDR();

	//----------------------------�`�揈��-------------------------------

	//�V�F�[�_�Ƀe�N�X�`����n��
	if( 0 != m_Texs.size() )
	{
		CGraphicsManager::m_SdrUse->m_Sdr->SetTexture( CGraphicsManager::m_SdrUse->m_SdrTex , m_Texs[ 0 ]->GetTex() );
		CGraphicsManager::m_SdrUse->m_Sdr->SetVector( CGraphicsManager::m_SdrUse->m_SdrColor , &D3DXVECTOR4( m_Col.x , m_Col.y , m_Col.z , m_Col.w ) );
	}
	//�e�N�X�`����������΃J���[�݂̂�n��
	else
	{
		CGraphicsManager::m_SdrUse->m_Sdr->SetVector( CGraphicsManager::m_SdrUse->m_SdrColor , &D3DXVECTOR4( m_Col.x , m_Col.y , m_Col.z , m_Col.w ) );
	}

	//�V�F�[�_�̎g�p�J�n
	CGraphicsManager::m_SdrUse->Begin();

	// �V�F�[�_�̃p�X�ݒ�
	if( 0 != m_Texs.size() )
	{
		if( !m_IsAddBlend )
		{
			CGraphicsManager::m_SdrUse->BeginPass( 0 );
		}
		else
		{
			CGraphicsManager::m_SdrUse->BeginPass( 1 );
		}
	}
	else
	{
		CGraphicsManager::m_SdrUse->BeginPass( 2 );
	}

	if( SUCCEEDED( CGraphicsManager::m_pD3dDevice->BeginScene() ) )
	{
		if(D3D_OK != m_Mesh->GetMesh()->DrawSubset( 0 ))
		{
			return;
		}
		V( CGraphicsManager::m_pD3dDevice->EndScene() );
	}

	//�p�X�I��
	CGraphicsManager::m_SdrUse->EndPass();

	//�V�F�[�_�I��
	CGraphicsManager::m_SdrUse->End();
}

//���[���h�}�g���b�N�X�ݒ�(billboard�p) //�J�����̕����������p
void CBoard::SetWldMtxBillBoard( const CameraBaseWP camera )
{
	//-----------------------------���[���h�}�g���N�X�ݒ�(���[�J�����W���烏�[���h���W��)-------------------------------------

	D3DXMATRIX SclMtx , RotMtx , PosMtx;

	//---------------------------�g��---------------------------
	D3DXMatrixScaling( &SclMtx , m_Scl.x , m_Scl.y , m_Scl.z );

	//--------------------------���s�ړ�--------------------------
	D3DXMatrixTranslation( &PosMtx, m_Pos.x, m_Pos.y, m_Pos.z );

	//�r���[�s����쐬
	D3DXMatrixLookAtLH( &RotMtx , &camera.lock()->GetPosition() , &m_Pos , &D3DXVECTOR3( 0 , 1 , 0 ) );

	//���s�ړ��s��𖳌�(0)��
	RotMtx._41 = 0;
	RotMtx._42 = 0;
	RotMtx._43 = 0;

	//�t�s��쐬
	D3DXMatrixInverse( &RotMtx , NULL , &RotMtx );

	//���[���h�}�g���b�N�X
	m_WldMtx =  SclMtx * RotMtx * PosMtx;
}

//�e�N�X�`���Z�b�^�[
void CBoard::SetTex( const TextureSP tex ){ m_Texs.clear();m_Texs.push_back( tex ); }
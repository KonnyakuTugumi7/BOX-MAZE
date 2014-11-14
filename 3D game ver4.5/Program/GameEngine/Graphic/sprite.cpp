#include "DXUT.h"
#include "sprite.h"
#include "../../Library/Support/common.h"
#include "../../GameEngine/Camera/camera_base.h"

//����
SpriteSP CSprite::Create( const std::string file_name , const CGraphicsManager::RENDERLIST_STATE list , const int x_num , const int y_num )
{
	//�X�v���C�g
	SpriteSP obj = std::make_shared< CSprite >();

	//������
	obj->Init( file_name , x_num , y_num );

	//�����_�����O���X�g�֓o�^
	CGraphicsManager::RegistObj( obj , list );

	return obj;
}

//������
void CSprite::Init( const std::string file_name , const int x_num , const int y_num )
{
	//���O
	m_name = file_name;

	//----------------------------------------------------------------------------
	// �t�@�C������ WCHAR �ɕϊ�
	WCHAR buff_name[256] = {0};
	CCommon::DXconvAnsiToWide( buff_name, file_name.c_str(), (file_name.length() + 1) );
	
	//----------------------------------------------------------------------------
	// �e�N�X�`���̃��[�h
	if( FAILED(D3DXCreateTextureFromFileEx( CGraphicsManager::m_pd3dDevice,
		buff_name,			// �t�@�C���p�X
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DPOOL_DEFAULT,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		D3DXCOLOR(0,0,0,0),	// �J���[�L�[�̐ݒ�( �����F )�@�� �S��0�w��ŃJ���[�L�[�Ȃ�
		&m_image_info,		// �摜���̎擾
		NULL,
		&m_texture			// �e�N�X�`���̎󂯎��
	) ) );

	//�e�N�X�`������T�[�t�F�C�X���擾
	IDirect3DSurface9 *pSurface;
	m_texture->GetSurfaceLevel( 0 , &pSurface );

	//�T�[�t�F�C�X��񂩂�摜�T�C�Y���擾
	D3DSURFACE_DESC SurfaceInfo;
	pSurface->GetDesc( &SurfaceInfo );

	//��������摜��1�̑傫��
	int x_split = m_image_info.Width / x_num;
	int y_split = m_image_info.Height / y_num;

	//�摜�𕪊����ĕ`�悷��͈͂��i�[����
	for( int y = 0 ; y < y_num ; ++y )
	{
		for( int x = 0 ; x < x_num ; ++x )
		{
			RECT rect = { x * x_split , y * y_split , ( x + 1 ) * x_split , ( y + 1 ) * y_split };
			m_rect.push_back( rect );
		}
	}

	//�摜�̃T�C�Y
	m_img_size = D3DXVECTOR3( m_image_info.Width , m_image_info.Height , 0 );

	//�摜�̒��S
	m_center = D3DXVECTOR3( m_img_size.x / x_num * 0.5f , m_img_size.y / y_num * 0.5f , 0 );

	//�g���I������̂ŃT�[�t�F�C�X���͉��
	SAFE_RELEASE( pSurface ) ;

	//�X�v���C�g�̐���
	D3DXCreateSprite( CGraphicsManager::m_pd3dDevice , &m_sprite );

	//�\�[�g�Ɏg���l
	m_camera_distance = 1.0f;
}

//�`��
void CSprite::Render( const CameraBaseWP camera )
{
	HRESULT hr;

	//���[���h�}�g���b�N�X�ݒ�
	SetWldMtx();

	//�X�v���C�g�Ƀ��[���h�}�g���N�X��ݒ�
	m_sprite->SetTransform( &m_wld_mtx );

	//�`��t���O���I�t�̎��ȉ��̏������s��Ȃ�
	if( m_is_render == false ) return;

	//���Z�u�����h�̐ݒ�
	if( m_is_add_blend )
	{
		CGraphicsManager::m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		CGraphicsManager::m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
		CGraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		CGraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
		CGraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		CGraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	}
	else
	{
		CGraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	}

	//�f�o�C�X�̕`�揀��
	if( SUCCEEDED( CGraphicsManager::m_pd3dDevice->BeginScene() ) )
	{
		//�X�v���C�g�̕`�揀��
		m_sprite->Begin( D3DXSPRITE_ALPHABLEND );

		//�X�v���C�g�̕`�施��
		m_sprite->Draw(	m_texture ,							//�`��Ɏg�p����e�N�X�`��
			&m_rect[ m_animation_frame ] ,					//�摜�̕`��͈�
			&m_center ,										//�T�[�t�F�C�X�̒��S
			&D3DXVECTOR3( 0 , 0 , 0 ) ,						//�`����W(�s��Ő��䂷��̂� 0,0,0 ��OK)
			D3DCOLOR_ARGB( ( int )( m_col.w * 255.0f ) ,	//�A���t�@�ƐF
						   ( int )( m_col.x * 255 ) , 
						   ( int )( m_col.y * 255 ) , 
						   ( int )( m_col.z * 255 ) )
			);

		//���Z�؂�ւ�
		if( m_is_add_blend )
		{
			m_sprite->Flush();
		}
		else
		{
			m_sprite->End();
		}

		//�`��I��
		V( CGraphicsManager::m_pd3dDevice->EndScene() );
	}
}
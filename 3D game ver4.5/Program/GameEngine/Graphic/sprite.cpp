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
	m_Name = file_name;

	//----------------------------------------------------------------------------
	// �t�@�C������ WCHAR �ɕϊ�
	WCHAR buff_name[256] = {0};
	CCommon::DXconvAnsiToWide( buff_name, file_name.c_str(), (file_name.length() + 1) );
	
	//----------------------------------------------------------------------------
	// �e�N�X�`���̃��[�h
	if( FAILED(D3DXCreateTextureFromFileEx( CGraphicsManager::m_pD3dDevice,
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
		&m_ImageInfo,		// �摜���̎擾
		NULL,
		&m_Texture			// �e�N�X�`���̎󂯎��
	) ) );

	//�e�N�X�`������T�[�t�F�C�X���擾
	IDirect3DSurface9 *pSurface;
	m_Texture->GetSurfaceLevel( 0 , &pSurface );

	//�T�[�t�F�C�X��񂩂�摜�T�C�Y���擾
	D3DSURFACE_DESC SurfaceInfo;
	pSurface->GetDesc( &SurfaceInfo );

	//��������摜��1�̑傫��
	int x_split = m_ImageInfo.Width / x_num;
	int y_split = m_ImageInfo.Height / y_num;

	//�摜�𕪊����ĕ`�悷��͈͂��i�[����
	for( int y = 0 ; y < y_num ; ++y )
	{
		for( int x = 0 ; x < x_num ; ++x )
		{
			RECT rect = { x * x_split , y * y_split , ( x + 1 ) * x_split , ( y + 1 ) * y_split };
			m_Rect.push_back( rect );
		}
	}

	//�摜�̃T�C�Y
	m_ImgSize = D3DXVECTOR3( m_ImageInfo.Width , m_ImageInfo.Height , 0 );

	//�摜�̒��S
	m_Center = D3DXVECTOR3( m_ImgSize.x / x_num * 0.5f , m_ImgSize.y / y_num * 0.5f , 0 );

	//�g���I������̂ŃT�[�t�F�C�X���͉��
	SAFE_RELEASE( pSurface ) ;

	//�X�v���C�g�̐���
	D3DXCreateSprite( CGraphicsManager::m_pD3dDevice , &m_Sprite );

	//�\�[�g�Ɏg���l
	m_CameraDistance = 1.0f;
}

//�`��
void CSprite::Render( const CameraBaseWP camera )
{
	HRESULT hr;

	//���[���h�}�g���b�N�X�ݒ�
	SetWldMtx();

	//�X�v���C�g�Ƀ��[���h�}�g���N�X��ݒ�
	m_Sprite->SetTransform( &m_WldMtx );

	//�`��t���O���I�t�̎��ȉ��̏������s��Ȃ�
	if( m_IsRender == false ) return;

	//���Z�u�����h�̐ݒ�
	if( m_IsAddBlend )
	{
		CGraphicsManager::m_pD3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		CGraphicsManager::m_pD3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
		CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
		CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	}
	else
	{
		CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	}

	//�f�o�C�X�̕`�揀��
	if( SUCCEEDED( CGraphicsManager::m_pD3dDevice->BeginScene() ) )
	{
		//�X�v���C�g�̕`�揀��
		m_Sprite->Begin( D3DXSPRITE_ALPHABLEND );

		//�X�v���C�g�̕`�施��
		m_Sprite->Draw(	m_Texture ,							//�`��Ɏg�p����e�N�X�`��
			&m_Rect[ m_AnimationFrame ] ,					//�摜�̕`��͈�
			&m_Center ,										//�T�[�t�F�C�X�̒��S
			&D3DXVECTOR3( 0 , 0 , 0 ) ,						//�`����W(�s��Ő��䂷��̂� 0,0,0 ��OK)
			D3DCOLOR_ARGB( ( int )( m_Col.w * 255.0f ) ,	//�A���t�@�ƐF
						   ( int )( m_Col.x * 255 ) , 
						   ( int )( m_Col.y * 255 ) , 
						   ( int )( m_Col.z * 255 ) )
			);

		//���Z�؂�ւ�
		if( m_IsAddBlend )
		{
			m_Sprite->Flush();
		}
		else
		{
			m_Sprite->End();
		}

		//�`��I��
		V( CGraphicsManager::m_pD3dDevice->EndScene() );
	}
}
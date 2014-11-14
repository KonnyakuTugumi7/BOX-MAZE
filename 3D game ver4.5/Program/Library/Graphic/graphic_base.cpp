#include "DXUT.h"
#include "graphic_base.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../GameEngine/Camera/camera_base.h"

//�`��
void CGraphicBase::Render( const CameraBaseWP camera )
{
	HRESULT hr;
	
	//���[���h�}�g���N�X�ݒ�(���[�J�����W���烏�[���h���W��)
	SetWldMtx();
	m_wld_mtx = m_wld_mtx * camera.lock()->GetViewMatrix() * camera.lock()->GetProjectionMatrix();

	//�J�����O�ݒ�
	CGraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_CULLMODE , m_cull_state );

	//�V�F�[�_�ݒ�
	SetSDR();

	if( m_is_render == false ) return;

	//----------------------------�`�揈��-------------------------------

	// 3D ���f���̃p�[�c�����[�v���ĕ`��
	for( int i = 0 ; i < m_mesh->GetMaterialNum() ; i++ )
	{
		// �i�p�[�c�ɑΉ�����e�N�X�`����ݒ�
		// �V�F�[�_�Ƀe�N�X�`����n��
		if( 0 != m_texs.size() )
		{
			if( NULL != m_texs[ i ] )
			{
				CGraphicsManager::m_sdr_use->m_sdr->SetTexture( CGraphicsManager::m_sdr_use->m_sdr_tex , m_texs[ i ]->GetTex() );
				CGraphicsManager::m_sdr_use->m_sdr->SetVector( CGraphicsManager::m_sdr_use->m_sdr_color , &D3DXVECTOR4( 1 , 1 , 1 , 1 ) );
			}
			else
			{
				CGraphicsManager::m_sdr_use->m_sdr->SetVector( CGraphicsManager::m_sdr_use->m_sdr_color , &D3DXVECTOR4( m_col.x , m_col.y , m_col.z , m_col.w ) );
			}
		}
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
			//�p�[�c�̕`��
			m_mesh->GetMesh()->DrawSubset( i ) ;
			V( CGraphicsManager::m_pd3dDevice->EndScene() );
		}

		//�p�X�I��
		CGraphicsManager::m_sdr_use->EndPass();

		//�V�F�[�_�I��
		CGraphicsManager::m_sdr_use->End();
	}
}

//�V�F�[�_�ݒ�
void CGraphicBase::SetSDR()
{
	//�V�F�[�_��؂�ւ���
	if( m_sdr_state == SDR_BASE )
	{
		CGraphicsManager::m_sdr_use = CGraphicsManager::m_sdr_base;
	}
	else if( m_sdr_state == SDR_DIR_LIGHT )
	{
		CGraphicsManager::m_sdr_use = CGraphicsManager::m_sdr_dir_light;
	}

	//�e�N�j�b�N�ݒ�
	CGraphicsManager::m_sdr_use->m_sdr->SetTechnique( CGraphicsManager::m_sdr_use->m_sdr_technique );
	
	//���[���h�s��ݒ�
	CGraphicsManager::m_sdr_use->m_sdr->SetMatrix( CGraphicsManager::m_sdr_use->m_sdr_wvmp , &m_wld_mtx );

	//�J���[�ݒ�
	CGraphicsManager::m_sdr_use->m_sdr->SetVector( CGraphicsManager::m_sdr_use->m_sdr_color , &m_col );

	//���s�����̎�
	if( m_sdr_state == SDR_DIR_LIGHT )
	{
		//���C�g�̐ݒ�
		CGraphicsManager::m_sdr_dir_light->m_sdr->SetVector( CGraphicsManager::m_sdr_dir_light->m_dir_light , &CGraphicsManager::m_sdr_dir_light->m_dir );

		//��]�s��̐ݒ�
		CGraphicsManager::m_sdr_dir_light->m_sdr->SetMatrix( CGraphicsManager::m_sdr_dir_light->m_rot_mtx , &m_rot_mtx );

		//�A���r�G���g�F�̐ݒ�
		CGraphicsManager::m_sdr_dir_light->m_sdr->SetFloat( CGraphicsManager::m_sdr_dir_light->m_ambient , 0.08f );
	}
}
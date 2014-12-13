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
	m_WldMtx = m_WldMtx * camera.lock()->GetViewMatrix() * camera.lock()->GetProjectionMatrix();

	//�J�����O�ݒ�
	CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_CULLMODE , m_CullState );

	//�V�F�[�_�ݒ�
	SetSDR();

	if( m_IsRender == false ) return;

	//----------------------------�`�揈��-------------------------------

	// 3D ���f���̃p�[�c�����[�v���ĕ`��
	for( int i = 0 ; i < m_Mesh->GetMaterialNum() ; i++ )
	{
		// �i�p�[�c�ɑΉ�����e�N�X�`����ݒ�
		// �V�F�[�_�Ƀe�N�X�`����n��
		if( 0 != m_Texs.size() )
		{
			if( NULL != m_Texs[ i ] )
			{
				CGraphicsManager::m_SdrUse->m_Sdr->SetTexture( CGraphicsManager::m_SdrUse->m_SdrTex , m_Texs[ i ]->GetTex() );
				CGraphicsManager::m_SdrUse->m_Sdr->SetVector( CGraphicsManager::m_SdrUse->m_SdrColor , &D3DXVECTOR4( m_Col.x , m_Col.y , m_Col.z , m_Col.w ) );
			}
			else
			{
				CGraphicsManager::m_SdrUse->m_Sdr->SetVector( CGraphicsManager::m_SdrUse->m_SdrColor , &D3DXVECTOR4( m_Col.x , m_Col.y , m_Col.z , m_Col.w ) );
			}
		}
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
			//�p�[�c�̕`��
			m_Mesh->GetMesh()->DrawSubset( i ) ;
			V( CGraphicsManager::m_pD3dDevice->EndScene() );
		}

		//�p�X�I��
		CGraphicsManager::m_SdrUse->EndPass();

		//�V�F�[�_�I��
		CGraphicsManager::m_SdrUse->End();
	}
}

//�V�F�[�_�ݒ�
void CGraphicBase::SetSDR()
{
	//�V�F�[�_��؂�ւ���
	if( m_sdr_state == SDR_BASE )
	{
		CGraphicsManager::m_SdrUse = CGraphicsManager::m_SdrBase;
	}
	else if( m_sdr_state == SDR_DIR_LIGHT )
	{
		CGraphicsManager::m_SdrUse = CGraphicsManager::m_SdrDirLight;
	}

	//�e�N�j�b�N�ݒ�
	CGraphicsManager::m_SdrUse->m_Sdr->SetTechnique( CGraphicsManager::m_SdrUse->m_SdrTechnique );
	
	//���[���h�s��ݒ�
	CGraphicsManager::m_SdrUse->m_Sdr->SetMatrix( CGraphicsManager::m_SdrUse->m_SdrWvmp , &m_WldMtx );

	//�J���[�ݒ�
	CGraphicsManager::m_SdrUse->m_Sdr->SetVector( CGraphicsManager::m_SdrUse->m_SdrColor , &m_Col );

	//���s�����̎�
	if( m_sdr_state == SDR_DIR_LIGHT )
	{
		//���C�g�̐ݒ�
		CGraphicsManager::m_SdrDirLight->m_Sdr->SetVector( CGraphicsManager::m_SdrDirLight->m_DirLight , &CGraphicsManager::m_SdrDirLight->m_Dir );

		//��]�s��̐ݒ�
		CGraphicsManager::m_SdrDirLight->m_Sdr->SetMatrix( CGraphicsManager::m_SdrDirLight->m_RotMtx , &m_RotMtx );

		//�A���r�G���g�F�̐ݒ�
		CGraphicsManager::m_SdrDirLight->m_Sdr->SetFloat( CGraphicsManager::m_SdrDirLight->m_Ambient , 0.08f );
	}
}
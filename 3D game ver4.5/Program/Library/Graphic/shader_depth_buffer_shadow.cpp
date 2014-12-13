#include "DXUT.h"
#include "shader_depth_buffer_shadow.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Support/common.h"

//�R���X�g���N�^
CShaderDepthBufferShadow::CShaderDepthBufferShadow( const std::string& file_pass )
{
	WCHAR wc_buff[ 255 ] = { 0 };
	CCommon::DXconvAnsiToWide( wc_buff, file_pass.c_str(), 255 );

	//�V�F�[�_�t�@�C���̃��[�h
	HRESULT hr = D3DXCreateEffectFromFile( CGraphicsManager::m_pD3dDevice , wc_buff , NULL , NULL , D3DXSHADER_DEBUG , NULL , &m_Sdr , NULL );

	if( SUCCEEDED(hr) )
	{
		//�e�N�j�b�N
		m_SdrTechnique = m_Sdr->GetTechniqueByName( "TShader" );
		//�e�N�X�`��0
		m_SdrTex = m_Sdr->GetParameterByName( NULL , "tex0" );
		//�J���[
		m_SdrColor = m_Sdr->GetParameterByName( NULL , "Color" );
		//���[���h x �r���[ x �v���W�F�N�V���� �s��
		m_SdrWvmp = m_Sdr->GetParameterByName( NULL , "WVP" );
		//���[���h x �r���[ x �v���W�F�N�V���� �s��
		m_LWVP        = m_Sdr->GetParameterByName( NULL, "m_LWVP" );
		//���[���h x �r���[ x �v���W�F�N�V���� �s�� x �e�N�X�`��
		m_LWVPT       = m_Sdr->GetParameterByName( NULL, "m_LWVPT" );
		//�e�N�X�`��1
		m_DepTex = m_Sdr->GetParameterByName( NULL , "tex1" );
		//���C�g�̕���
		m_LightDir    = m_Sdr->GetParameterByName( NULL, "m_LightDir" );
		//�A���r�G���g�F
		m_Ambient     = m_Sdr->GetParameterByName( NULL, "m_Ambient" );
		//�o�C�A�X�l
		m_Bias        = m_Sdr->GetParameterByName( NULL, "m_Bias" );
		//�e�̐F
		m_ShadowColor = m_Sdr->GetParameterByName( NULL, "m_ShadowColor" );
	}
}

//�V�F�[�_�J�n
void CShaderDepthBufferShadow::Begin()
{
	CGraphicsManager::m_pD3dDevice->GetTransform( D3DTS_VIEW , &m_MatProj );
	CGraphicsManager::m_pD3dDevice->GetTransform( D3DTS_PROJECTION, &m_MatProj );
	if( m_Sdr ) m_Sdr->Begin( NULL , 0 ); 
}

//�A���r�G���g�F�Z�b�g
//����1:�A���r�G���g�F
void CShaderDepthBufferShadow::SetAmbient( const float ambient )
{
	if( m_Sdr )
	{
		D3DXVECTOR4 A;
		A = D3DXVECTOR4( ambient, ambient, ambient, 1.0f );
		m_Sdr->SetVector( m_Ambient, &A );
	}
	else
	{
		D3DMATERIAL9 old_material;
		CGraphicsManager::m_pD3dDevice->GetMaterial( &old_material );
		old_material.Ambient.r = ambient;
		old_material.Ambient.g = ambient;
		old_material.Ambient.b = ambient;
		old_material.Ambient.a = 1.0f;
		CGraphicsManager::m_pD3dDevice->SetMaterial( &old_material );
	}
}
//����1:�A���r�G���g�F
void CShaderDepthBufferShadow::SetAmbient( const D3DXVECTOR4& ambient )
{
	if( m_Sdr )
	{
		m_Sdr->SetVector( m_Ambient, &ambient );
	}
	else
	{
		D3DMATERIAL9 old_material;
		CGraphicsManager::m_pD3dDevice->GetMaterial( &old_material );
		old_material.Ambient.r = ambient.x;
		old_material.Ambient.g = ambient.y;
		old_material.Ambient.b = ambient.z;
		old_material.Ambient.a = ambient.w;
		CGraphicsManager::m_pD3dDevice->SetMaterial( &old_material );
	}
}

//�s��ݒ�
//����1:���[���h�s�� , ����2:���C�g , ����3:���C�g�̕���
void CShaderDepthBufferShadow::SetMatrix( const D3DXMATRIX& world , const D3DXMATRIX& light , const D3DXVECTOR4& light_dir )
{
	if( m_Sdr )
	{
		D3DXMATRIX m , m1 , m2;
		D3DXVECTOR4 LightDir;
		D3DXVECTOR4 v;

		//�J������̍s��ϊ��}�g���b�N�X���Z�b�g����
		m = world * m_MatView * m_MatProj;
		m_Sdr->SetMatrix( m_SdrWvmp , &m );

		//���C�g�̕����x�N�g�����v�Z
		LightDir = light_dir;
		D3DXMatrixInverse( &m1 , NULL , &world );
		D3DXVec4Transform( &v, &LightDir , &m1 );
		//XYZ�����ɂ��Đ��K������
		D3DXVec3Normalize( (D3DXVECTOR3*)&v, (D3DXVECTOR3*)&v );
		m_Sdr->SetVector( m_LightDir , &v );

		//���C�g��̍s��ϊ��}�g���b�N�X���Z�b�g����
		m = world * light;
		m_Sdr->SetMatrix( m_LWVP , &m );

		//���C�g��̍s��ϊ��}�g���b�N�X���e�N�X�`���[���W�n�֕ϊ�
		D3DXMatrixScaling( &m1 , 0.5f , -0.5f , 1.0f );
		D3DXMatrixTranslation( &m2 , 0.5f , 0.5f , 0.0f );
		m = m * m1 * m2;
		m_Sdr->SetMatrix( m_LWVPT, &m );
	}
	else
	{
		CGraphicsManager::m_pD3dDevice->SetTransform( D3DTS_WORLD , &world );
	}
}
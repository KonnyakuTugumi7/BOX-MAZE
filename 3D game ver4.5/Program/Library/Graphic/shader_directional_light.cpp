#include "DXUT.h"
#include "shader_directional_light.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Support/common.h"

//�R���X�g���N�^
//����1:�t�@�C���p�X
CShaderDirectionalLight::CShaderDirectionalLight( const std::string& file_pass )
{
	WCHAR wc_buff[ 255 ] = { 0 };
	CCommon::DXconvAnsiToWide( wc_buff, file_pass.c_str(), 255 );

	//�V�F�[�_�t�@�C���̃��[�h
	D3DXCreateEffectFromFile( CGraphicsManager::m_pD3dDevice , wc_buff , NULL , NULL , D3DXSHADER_DEBUG , NULL , &m_Sdr , NULL );
	//�e�N�j�b�N
	m_SdrTechnique = m_Sdr->GetTechniqueByName( "TShader" );
	//�e�N�X�`��
	m_SdrTex = m_Sdr->GetParameterByName( NULL , "SrcTex" );
	//�J���[
	m_SdrColor = m_Sdr->GetParameterByName( NULL , "Color" );
	//���[���h x �r���[ x �v���W�F�N�V���� �s��
	m_SdrWvmp = m_Sdr->GetParameterByName( NULL , "WVP" );
	//��]�s��
	m_RotMtx = m_Sdr->GetParameterByName( NULL , "RM" );
	//���s�����̌���
	m_DirLight = m_Sdr->GetParameterByName( NULL , "LightDir" );
	//�A���r�G���g�F
	m_Ambient = m_Sdr->GetParameterByName( NULL , "Ambient" );

	m_Dir = D3DXVECTOR4( -1.0f , -1.0f , 1.0f , 0.0f );
}
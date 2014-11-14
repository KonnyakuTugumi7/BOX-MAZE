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
	D3DXCreateEffectFromFile( CGraphicsManager::m_pd3dDevice , wc_buff , NULL , NULL , D3DXSHADER_DEBUG , NULL , &m_sdr , NULL );
	//�e�N�j�b�N
	m_sdr_technique = m_sdr->GetTechniqueByName( "TShader" );
	//�e�N�X�`��
	m_sdr_tex = m_sdr->GetParameterByName( NULL , "SrcTex" );
	//�J���[
	m_sdr_color = m_sdr->GetParameterByName( NULL , "Color" );
	//���[���h x �r���[ x �v���W�F�N�V���� �s��
	m_sdr_wvmp = m_sdr->GetParameterByName( NULL , "WVP" );
	//��]�s��
	m_rot_mtx = m_sdr->GetParameterByName( NULL , "RM" );
	//���s�����̌���
	m_dir_light = m_sdr->GetParameterByName( NULL , "LightDir" );
	//�A���r�G���g�F
	m_ambient = m_sdr->GetParameterByName( NULL , "Ambient" );

	m_dir = D3DXVECTOR4( -1.0f , -1.0f , 1.0f , 0.0f );
}
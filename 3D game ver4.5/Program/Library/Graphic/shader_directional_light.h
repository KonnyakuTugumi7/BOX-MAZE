#pragma once

#include "DXUT.h"
#include "../../Library/Graphic/shader_base.h"
#include <string>
#include <memory>

class CShaderDirectionalLight;
typedef std::shared_ptr< CShaderDirectionalLight > ShaderDirectionalLightSP;

//���s�����V�F�[�_�N���X
class CShaderDirectionalLight : public CShaderBase{
public:
	//--------------�n���h��--------------
	//���s�����̌���
	D3DXHANDLE m_dir_light;

	//��]�s��
	D3DXHANDLE m_rot_mtx;

	//�A���r�G���g�F
	D3DXHANDLE m_ambient;
	//------------------------------------

	//���s�����̌���
	D3DXVECTOR4 m_dir;

public:
	//�R���X�g���N�^
	//����1:�t�@�C���p�X
	CShaderDirectionalLight( const std::string& file_pass );

	//�f�X�g���N�^
	~CShaderDirectionalLight(){}

	//����
	//����1:�t�@�C���p�X
	static ShaderDirectionalLightSP Create( const std::string& file_pass ){ return std::make_shared< CShaderDirectionalLight >( file_pass ); }
};
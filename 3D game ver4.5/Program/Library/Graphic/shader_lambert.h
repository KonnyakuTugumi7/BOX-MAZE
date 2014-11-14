#pragma once

#include "DXUT.h"
#include "../../Library/Graphic/shader_base.h"
#include <string>
#include <memory>

class CShaderLambert;
typedef std::shared_ptr< CShaderLambert > ShaderLambertSP;

//�����o�[�g�Ɩ��͎��������Ɉˑ����Ȃ������̓��˕����Ɩʂ̖@�������ŎZ�o�����A�e�����Z�@�E�E�E�炵��
//�����ł̓��b�V�������C�g�ʒu����Ȃ��߂āAZ�l���擾���鏈��
class CShaderLambert : public CShaderBase{
public:
	//�R���X�g���N�^
	//����1:�V�F�[�_�t�@�C���p�X
	CShaderLambert( const std::string& file_pass );

	//�f�X�g���N�^
	~CShaderLambert(){}

	//����
	//����1:�V�F�[�_�t�@�C���p�X
	static ShaderLambertSP Create( const std::string& file_pass ){ return std::make_shared< CShaderLambert >( file_pass ); }
};
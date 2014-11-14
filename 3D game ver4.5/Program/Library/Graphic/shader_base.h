#pragma once

#include "DXUT.h"
#include <string>
#include <memory>

class CShaderBase;
typedef std::shared_ptr< CShaderBase > ShaderBaseSP;

//�V�F�[�_��{�N���X
class CShaderBase{
public:
	//�V�F�[�_�{��
	LPD3DXEFFECT m_sdr;

	//-----------�n���h��-----------
	//�e�N�j�b�N
	D3DXHANDLE m_sdr_technique;

	//�e�N�X�`��
	D3DXHANDLE m_sdr_tex;

	//�J���[
	D3DXHANDLE m_sdr_color;
	//------------------------------

	//���[���h x �r���[ x �v���W�F�N�V�����s��
	D3DXHANDLE m_sdr_wvmp;

public:
	//�R���X�g���N�^
	CShaderBase(){}

	//�R���X�g���N�^
	//����1:�V�F�[�_�t�@�C���p�X
	CShaderBase( const std::string& file_pass );

	//�f�X�g���N�^
	virtual ~CShaderBase()
	{
		//�V�F�[�_���
		SAFE_RELEASE( m_sdr );
	}

	//����
	//����1:�t�@�C���p�X
	static ShaderBaseSP Create( const std::string& file_pass ){ return std::make_shared< CShaderBase >( file_pass ); }

	//�V�F�[�_�J�n
	virtual void Begin(){ if( m_sdr ) m_sdr->Begin( NULL , 0 ); }

	//�V�F�[�_�I��
	void End(){ if( m_sdr ) m_sdr->End(); }

	//�V�F�[�_�p�X�J�n
	void BeginPass( const UINT pass ){ if( m_sdr ) m_sdr->BeginPass( pass ); }

	//�V�F�[�_�p�X�I��
	void EndPass(){ if( m_sdr ) m_sdr->EndPass(); }
};
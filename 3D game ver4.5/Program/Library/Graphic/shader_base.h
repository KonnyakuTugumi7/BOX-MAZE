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
	LPD3DXEFFECT m_Sdr;

	//-----------�n���h��-----------
	//�e�N�j�b�N
	D3DXHANDLE m_SdrTechnique;

	//�e�N�X�`��
	D3DXHANDLE m_SdrTex;

	//�J���[
	D3DXHANDLE m_SdrColor;
	//------------------------------

	//���[���h x �r���[ x �v���W�F�N�V�����s��
	D3DXHANDLE m_SdrWvmp;

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
		SAFE_RELEASE( m_Sdr );
	}

	//����
	//����1:�t�@�C���p�X
	static ShaderBaseSP Create( const std::string& file_pass ){ return std::make_shared< CShaderBase >( file_pass ); }

	//�V�F�[�_�J�n
	virtual void Begin(){ if( m_Sdr ) m_Sdr->Begin( NULL , 0 ); }

	//�V�F�[�_�I��
	void End(){ if( m_Sdr ) m_Sdr->End(); }

	//�V�F�[�_�p�X�J�n
	void BeginPass( const UINT pass ){ if( m_Sdr ) m_Sdr->BeginPass( pass ); }

	//�V�F�[�_�p�X�I��
	void EndPass(){ if( m_Sdr ) m_Sdr->EndPass(); }
};
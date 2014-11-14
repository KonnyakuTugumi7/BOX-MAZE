#pragma once

#include "DXUT.h"
#include "../../Library/Graphic/shader_base.h"
#include <string>
#include <memory>

class CShaderDepthBufferShadow;
typedef std::shared_ptr< CShaderDepthBufferShadow > ShaderDepthBufferShadowSP;

class CShaderDepthBufferShadow : public CShaderBase{
public:
	//----------------------�n���h��----------------------
	//���[���h���W�n �~ �r���[(���C�g�)���W�n �~ ���ˉe���W�n
	D3DXHANDLE m_LWVP;

	//���[���h���W�n �~ �r���[(���C�g�)���W�n �~ ���ˉe���W�n �~ �e�N�X�`���[���W�n
	D3DXHANDLE m_LWVPT;

	//�f�v�X�o�b�t�@�T�[�t�F�C�X�̃e�N�X�`���[
	D3DXHANDLE m_dep_tex;

	//���C�g�̕���
	D3DXHANDLE m_LightDir;

	//�A���r�G���g�F
	D3DXHANDLE m_Ambient;

	//�o�C�A�X�l(Z�l�̔�r�ɂ��덷��␳��)
	D3DXHANDLE m_Bias;

	//�e�̐F(�e�ɂ�郁�b�V���̃J���[�̌��Z�l)
	D3DXHANDLE m_ShadowColor;
	//----------------------------------------------------

	//�r���[�s��
	D3DXMATRIX m_matView;

	//�v���W�F�N�V�����s��
	D3DXMATRIX m_matProj;

public:
	//�R���X�g���N�^
	//����1:�t�@�C���p�X
	CShaderDepthBufferShadow( const std::string& file_pass );

	//�f�X�g���N�^
	~CShaderDepthBufferShadow(){}

	//����
	//����1:�t�@�C���p�X
	static ShaderDepthBufferShadowSP Create( const std::string& file_pass ){ return std::make_shared< CShaderDepthBufferShadow >( file_pass ); }

	//�V�F�[�_�J�n
	virtual void Begin();

	//�A���r�G���g�F�Z�b�g
	//����1:�A���r�G���g�F
	void SetAmbient( const float ambient );

	//����1:�A���r�G���g�F
	void SetAmbient( const D3DXVECTOR4& ambient );

	//�s��ݒ�
	//����1:���[���h�s�� , ����2:���C�g , ����3:���C�g�̕���
	void SetMatrix( const D3DXMATRIX& world , const D3DXMATRIX& light , const D3DXVECTOR4& light_dir );

	//Z�l�̔�r�ɂ���Ĕ�������v�Z�덷���y�����邽�߂̃o�C�A�X�l
	//����1:�o�C�A�X�l
	void SetBias( const float& bias )
	{
		if( m_sdr )
		{
			m_sdr->SetFloat( m_Bias , bias );
		}
	}

	//�e�ɂ�郁�b�V���̃J���[�̌��Z�l
	//����1:�e�̐F(1.0f�Ŋ��S�ɍ��ɂȂ�)
	void SetShadowColor( const float& shadow_color )
	{
		if( m_sdr )
		{
			m_sdr->SetFloat( m_ShadowColor , shadow_color );
		}
	}
};
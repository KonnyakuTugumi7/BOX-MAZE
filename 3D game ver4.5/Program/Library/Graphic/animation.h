#pragma once

#include "DXUT.h"
#include <vector>
#include <string>
#include <memory>
#include <atlbase.h>

#pragma warning ( disable : 4389 )

class CTime;
typedef std::shared_ptr< CTime > TimeSP;

//2D�A�j���[�V�����N���X
class CAnimation{
protected:
	//���[�v�t���O
	bool m_IsLoop;

	//�`��t���O
	bool m_IsActive;

	//�A�j���t���[���ԍ�
	int m_AnimationFrame;

	//�摜�̃T�C�Y
	D3DXVECTOR3 m_ImgSize;

	//�摜�̒��S���W
	D3DXVECTOR3 m_Center;

	//�摜�𕪊����ĕ`�悷��͈͂��i�[
	std::vector< RECT > m_Rect;

	//�摜���(���⍂���Ȃ�)�̎擾�p
	D3DXIMAGE_INFO m_ImageInfo;

	//�摜�f�[�^(�e�N�X�`��)
	LPDIRECT3DTEXTURE9 m_Texture;

	//�A�j���[�V�����p�^�C�}�[
	TimeSP m_AnimationTimer;

public:
	//�R���X�g���N�^
	CAnimation();

	//�f�X�g���N�^
	virtual ~CAnimation();

	//�Đ�
	void Play();
};
#include "DXUT.h"
#include "animation.h"
#include "../../Library/Support/time.h"

//�R���X�g���N�^
CAnimation::CAnimation() : 
	m_ImgSize( 0 , 0 , 0 ) , 
	m_Center( 0 , 0 , 0 ) , 
	m_AnimationFrame( 0 ) ,
	m_IsLoop( true ) ,
	m_IsActive( true ) 
{
	m_AnimationTimer = std::make_shared< CTime >( 2 );
}

//�f�X�g���N�^
CAnimation::~CAnimation()
{
	//���
	SAFE_RELEASE( m_Texture );
	m_Rect.clear();
	m_AnimationTimer.reset();
}

//�Đ�
void CAnimation::Play()
{
	//�`��t���O���I�t�̎��ȉ��̏������s��Ȃ�
	if( m_IsActive == false ) return;

	//�����Ȃ��œǂݍ��񂾎��ȉ��̏������s��Ȃ�
	if( m_Rect.size() == 1 ) return;

	//�A�j���[�V�����J�E���g��i�߂�
	if( m_AnimationTimer->Update() == true )
	{
		//���̉摜�ɐi��(�摜�̕������܂�)
		++m_AnimationFrame;

		//�J�E���g�����Z�b�g
		m_AnimationTimer->Start();
	}

	//�P��
	if( m_IsLoop == false && m_Rect.size() == m_AnimationFrame )
	{
		//�`��t���O���I�t�ɂ���
		m_IsActive = false;

		//�A�j���[�V�����t���[�����ŏ��ɖ߂�
		m_AnimationFrame = 0;
			 
		//�J�E���g�����Z�b�g
		m_AnimationTimer->Start();
	}

	//�A�j���t���[���ԍ������[�v������
	m_AnimationFrame %= m_Rect.size();
}
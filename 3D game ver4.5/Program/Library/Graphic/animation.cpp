#include "DXUT.h"
#include "animation.h"
#include "../../Library/Support/time.h"

//�R���X�g���N�^
CAnimation::CAnimation() : 
	m_img_size( 0 , 0 , 0 ) , 
	m_center( 0 , 0 , 0 ) , 
	m_animation_frame( 0 ) ,
	m_is_loop( true ) ,
	m_is_active( true ) 
{
	m_animation_timer = std::make_shared< CTime >( 2 );
}

//�f�X�g���N�^
CAnimation::~CAnimation()
{
	//���
	SAFE_RELEASE( m_texture );
	m_rect.clear();
	m_animation_timer.reset();
}

//�Đ�
void CAnimation::Play()
{
	//�`��t���O���I�t�̎��ȉ��̏������s��Ȃ�
	if( m_is_active == false ) return;

	//�����Ȃ��œǂݍ��񂾎��ȉ��̏������s��Ȃ�
	if( m_rect.size() == 1 ) return;

	//�A�j���[�V�����J�E���g��i�߂�
	if( m_animation_timer->Update() == true )
	{
		//���̉摜�ɐi��(�摜�̕������܂�)
		++m_animation_frame;

		//�J�E���g�����Z�b�g
		m_animation_timer->Start();
	}

	//�P��
	if( m_is_loop == false && m_rect.size() == m_animation_frame )
	{
		//�`��t���O���I�t�ɂ���
		m_is_active = false;

		//�A�j���[�V�����t���[�����ŏ��ɖ߂�
		m_animation_frame = 0;
			 
		//�J�E���g�����Z�b�g
		m_animation_timer->Start();
	}

	//�A�j���t���[���ԍ������[�v������
	m_animation_frame %= m_rect.size();
}
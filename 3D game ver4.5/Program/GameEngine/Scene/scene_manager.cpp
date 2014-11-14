#include "DXUT.h"
#include "scene_manager.h"
#include "../../Library/Support/common.h"
#include "../../Library/Input/input.h"
#include "../../Library/Sound/sound.h"
#include "../../Library/Sound/sound_manager.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../GameEngine/Graphic/sprite.h"
#include "../../Game/Scene/scene_title.h"
#include "../../Game/Scene/scene_play.h"

//������
void CSceneManager::InitSceneManager()
{
	//�C���X�^���X�쐬
	m_scene_main = std::make_shared< CSceneTitle >();

	//�T�E���h�Đ�
	m_scene_main->ScenePlaySound();

	//�t�F�[�h�V�[���쐬
	m_fade = CSceneFade::CreateScene();
}

//���
void CSceneManager::DeleteSceneManager()
{
	//���
	m_scene_main.reset();
	m_fade.reset();
}

void CSceneManager::SetNextScene( SceneBaseSP ( *next )() )
{
	//�V�[���̂�\��
	m_scene_next = next;

	//�t�F�[�h�A�E�g
	m_fade->SetFadeState( m_fade->FADE_OUT );
}


//���̃V�[���Ɉړ�
void CSceneManager::ChangeScene()
{
	//�T�E���h��~
	m_scene_main->SceneStopSound();

	//�V�[���ړ�
	m_scene_main = m_scene_next();

	//�V�[���\������Z�b�g
	m_scene_next = NULL;

	//�T�E���h�Đ�
	m_scene_main->ScenePlaySound();
}

//���W�b�N
void CSceneManager::FrameMove( const float elapsed_time )
{
	//�L�[�{�[�h�X�V
	K_B->Update();

	//�}�E�X�X�V
    MOUSE->Update();

	//�L�[�{�[�h&�}�E�X���b�N
	if( m_fade->GetFadeState() == m_fade->FADE_IN || m_fade->GetFadeState() == m_fade->FADE_OUT )
	{
		K_B->SetIsActive( false );
		MOUSE->SetIsActive( false );
	}
	//�A�����b�N
	else
	{
		K_B->SetIsActive( true );
		MOUSE->SetIsActive( true );
	}

	//�T�E���h�}�l�[�W���̍X�V
	CSoundManager::Updata();

	//�O���t�B�b�N�}�l�[�W���̍X�V
	CGraphicsManager::Update();

	//���C�����W�b�N
	m_scene_main->SceneFrameMove( elapsed_time );

	//�t�F�[�h���W�b�N
	m_fade->SceneFrameMove( elapsed_time );

	//�V�[���`�F���W
	//���̃V�[�����\�񂳂�Ă��������t�F�[�h�C���̎�
	if( m_scene_next != NULL && m_fade->GetFadeState() == m_fade->FADE_IN )
	{
		ChangeScene();
	}
}

//�`��
void CSceneManager::FrameRender( const float elapsed_time )
{
	//���C���`��
	m_scene_main->SceneFrameRender( elapsed_time );

	//�t�F�[�h�`��
	m_fade->SceneFrameRender( elapsed_time );
}
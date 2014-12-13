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
	m_SceneMain = std::make_shared< CSceneTitle >();

	//�T�E���h�Đ�
	m_SceneMain->ScenePlaySound();

	//�t�F�[�h�V�[���쐬
	m_Fade = CSceneFade::CreateScene();
}

//���
void CSceneManager::DeleteSceneManager()
{
	//���
	m_SceneMain.reset();
	m_Fade.reset();
}

void CSceneManager::SetNextScene( SceneBaseSP ( *next )() )
{
	//�V�[���̂�\��
	m_SceneNext = next;

	//�t�F�[�h�A�E�g
	m_Fade->SetFadeState( m_Fade->FADE_OUT );
}


//���̃V�[���Ɉړ�
void CSceneManager::ChangeScene()
{
	//�T�E���h��~
	m_SceneMain->SceneStopSound();

	//�V�[���ړ�
	m_SceneMain = m_SceneNext();

	//�V�[���\������Z�b�g
	m_SceneNext = NULL;

	//�T�E���h�Đ�
	m_SceneMain->ScenePlaySound();
}

//���W�b�N
void CSceneManager::FrameMove( const float elapsed_time )
{
	//�L�[�{�[�h�X�V
	K_B->Update();

	//�}�E�X�X�V
    MOUSE->Update();

	//�L�[�{�[�h&�}�E�X���b�N
	if( m_Fade->GetFadeState() == m_Fade->FADE_IN || m_Fade->GetFadeState() == m_Fade->FADE_OUT )
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
	m_SceneMain->SceneFrameMove( elapsed_time );

	//�t�F�[�h���W�b�N
	m_Fade->SceneFrameMove( elapsed_time );

	//�V�[���`�F���W
	//���̃V�[�����\�񂳂�Ă��������t�F�[�h�C���̎�
	if( m_SceneNext != NULL && m_Fade->GetFadeState() == m_Fade->FADE_IN )
	{
		ChangeScene();
	}
}

//�`��
void CSceneManager::FrameRender( const float elapsed_time )
{
	//���C���`��
	m_SceneMain->SceneFrameRender( elapsed_time );

	//�t�F�[�h�`��
	m_Fade->SceneFrameRender( elapsed_time );
}
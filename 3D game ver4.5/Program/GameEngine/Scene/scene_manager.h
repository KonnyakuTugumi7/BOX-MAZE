#pragma once

#include "../../GameEngine/Scene/scene_base.h"
#include "../../Game/Scene/scene_fade.h"
#include <memory>

#pragma warning( disable : 4244 )

class CSceneManager{
private:
	//�t�F�[�h�V�[��
	SceneFadeSP m_fade;

	//���݂̃V�[���̃|�C���^
	SceneBaseSP m_scene_main;

	//���̃V�[����CreateScene���ĂԂƂ��Ɏg���֐��|�C���^(�t�F�[�h�A�E�g���I������^�C�~���O�ŌĂяo�����߂Ɋ֐��|�C���^���g�p)
	SceneBaseSP ( *m_scene_next )();

private:
	//�R���X�g���N�^
	CSceneManager(){}

	//�f�X�g���N�^
	~CSceneManager(){}

public:	
	//�C���X�^���X�擾
	static CSceneManager* GetInstance()
	{
		static CSceneManager instance;
		return &instance;
	}

	//������
	void InitSceneManager();
	
	//���
	void DeleteSceneManager();

	//���̃V�[����\��
	void SetNextScene( SceneBaseSP ( *next )() );

	//���̃V�[���̂Ɉړ�
	void ChangeScene();

	//���W�b�N
	//����1:�o�ߎ���
	void FrameMove( const float elapsed_time );

	//�`��
	//����1:�o�ߎ���
	void FrameRender( const float elapsed_time );
};
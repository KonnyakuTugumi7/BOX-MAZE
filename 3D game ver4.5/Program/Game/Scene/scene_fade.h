#pragma once

#include "DXUT.h"
#include "../../Library/Support/csv.h"
#include "../../GameEngine/Scene/scene_base.h"
#include "../../GameEngine/Graphic/sprite.h"
#include <map>
#include <list>
#include <time.h>
#include <memory>

class CSceneFade;
typedef std::shared_ptr< CSceneFade > SceneFadeSP;

class CSceneFade : public CSceneBase{
private:
	//�t�F�[�h�p�摜
	SpriteSP m_fade;

	//�t�F�[�h�Ǘ�
	enum FADE_MODE;
	FADE_MODE m_fade_mode;

public:
	//�t�F�[�h�Ǘ�
	enum FADE_MODE
	{
		//�t�F�[�h���Ă��Ȃ�(�ʏ�̏��)
		FADE_NON = 0 , 

		//�t�F�[�h�C��
		FADE_IN = -1 , 

		//�t�F�[�h�A�E�g
		FADE_OUT = 1
	};

public:
	//�C���X�^���X�쐬
	static SceneFadeSP CreateScene(){ return std::make_shared< CSceneFade >(); }

	//�R���X�g���N�^
	CSceneFade()
	{
		//������
		SceneInit();
	}

	//�f�X�g���N�^
	~CSceneFade()
	{
		//���
		SceneDelete();
	}
	
	//������
	void SceneInit();

	//���
	void SceneDelete();

	//���W�b�N
	//����1:�o�ߎ���
	void SceneFrameMove( const float elapsed_time );

	//�`��
	//����1:�o�ߎ���
	void SceneFrameRender( const float elapsed_time );

	//�T�E���h�Đ�
	void ScenePlaySound(){}

	//�T�E���h��~
	void SceneStopSound(){}

	//�t�F�[�h��ԃQ�b�^�[
	FADE_MODE GetFadeState(){ return m_fade_mode; }

	//�t�F�[�h��ԃZ�b�^�[
	void SetFadeState( const FADE_MODE mode ){ m_fade_mode = mode; }
};

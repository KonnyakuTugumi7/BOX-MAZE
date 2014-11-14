#pragma once

#include "DXUT.h"
#include "../../Library/Support/csv.h"
#include "../../GameEngine/Scene/scene_base.h"
#include "../../GameEngine/Graphic/sprite.h"
#include "../../GameEngine/Graphic/board.h"
#include <string>
#include <memory>

#pragma warning( disable : 4244 )

class CSceneTitle;
typedef std::shared_ptr< CSceneTitle > SceneTitleSP;

class CSceneTitle : public CSceneBase{
private:
	//�^�C�g���Ǘ��ԍ�
	enum SCENE_NUM{
		PLAY = 0 ,
		EDIT ,
		EXIT ,
		SCENE_MAX ,
		SCENE_MIN = -1
	};

	//�^�C�g���Ǘ��ԍ�
	enum STAGE_SELECT_NUM{
		STAGE01 = 0 ,
		STAGE02 ,
		TITLE ,
		STAGE_MAX ,
		STAGE_MIN = -1
	};

	//�X�e�[�W�I����ʂ��ǂ���
	bool m_is_stage_select;

	//�^�C�g���Ǘ��ԍ�
	int m_scene_num;

	//�^�C�g���摜
	SpriteSP m_back;// , m_logo[ 4 ];
	//BoardSP m_board;

	//�^�C�g�����S
	FontSP m_logo[ 3 ];

public:
	//�C���X�^���X�쐬
	static SceneBaseSP CreateScene(){ return std::make_shared< CSceneTitle >(); }

	//�R���X�g���N�^
	CSceneTitle()
	{
		//������
		SceneInit();
	}

	//�f�X�g���N�^
	~CSceneTitle()
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
	void ScenePlaySound();

	//�T�E���h��~
	void SceneStopSound();

	//�v���C���[�̓���
	void TitleInput();
	void StageSelectInput();

	//�^�C�g����ʕ`��
	void TitleRender();

	//�X�e�[�W�Z���N�g��ʕ`��
	void StageSelectRender();
};

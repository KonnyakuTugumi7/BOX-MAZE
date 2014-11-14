#pragma once

#include "DXUT.h"
#include "../../Library/Support/csv.h"
#include "../../GameEngine/Scene/scene_base.h"
#include "../../GameEngine/Camera/camera_base.h"
#include "../../GameEngine/Graphic/xfile.h"
#include "../../GameEngine/Graphic/cube_manager.h"
#include "../../Game/Object/user_interface.h"
#include <map>
#include <list>
#include <time.h>
#include <memory>

class CScenePlay;
typedef std::shared_ptr< CScenePlay > ScenePlaySP;

class CScenePlay : public CSceneBase{
private:
	//�L���[�u�}�l�[�W��
	CubeManagerSP m_cube_manager;

	//���[�U�[�C���^�[�t�F�[�X
	UserInterfaceSP m_ui;

public:
	//�C���X�^���X�쐬
	static SceneBaseSP CreateScene(){ return std::make_shared< CScenePlay >(); }

	//�R���X�g���N�^
	CScenePlay()
	{
		//������
		SceneInit();
	}

	//�f�X�g���N�^
	~CScenePlay()
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

	//����
	void PlayInput();

	//�J����(���@���X�^�[�g�L���[�u��)
	void CameraToStartCube();
};

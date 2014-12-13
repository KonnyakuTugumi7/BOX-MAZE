#pragma once

#include "DXUT.h"
#include "../../Library/Support/csv.h"
#include "../../GameEngine/Graphic/sprite.h"
#include "../../GameEngine/Scene/scene_base.h"
#include "../../GameEngine/Camera/camera_base.h"
#include "../../Game/Camera/view_camera.h"
#include "../../GameEngine/Graphic/cube_manager.h"
#include "../../GameEngine/Graphic/cube.h"
#include <map>
#include <list>
#include <time.h>
#include <memory>

class CSceneEdit;
typedef std::shared_ptr< CSceneEdit > SceneEditSP;

class CSceneEdit : public CSceneBase{
private:
	//�L���[�u�}�l�[�W��
	CubeManagerSP m_CubeManager;

	//�I���L���[�u
	CubeSP m_SelectCube;

	//��ʒ����̏\���J�[�\��
	BoardSP m_CenterCarsor;

	//�t�H���g
	FontSP m_Font;
	std::vector< std::wstring > m_Logo;

	//�J����
	ViewCameraSP m_ViewCamera;

	//�I���L���[�u�Ǘ��i���o�[
	int m_SelectCubeState;

	////�^�[�Q�b�g�ړ����x
	//float m_MoveSpeed;

	//�^�[�Q�b�g���W
	D3DXVECTOR3 m_Pos;

public:
	//�C���X�^���X�쐬
	static SceneBaseSP CreateScene(){ return std::make_shared< CSceneEdit >(); }

	//�R���X�g���N�^
	CSceneEdit()
	{
		//������
		SceneInit();
	}

	//�f�X�g���N�^
	~CSceneEdit()
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

	//�����蔻��
	void SceneCollision();

	//�����o��
	void SceneExport();
};

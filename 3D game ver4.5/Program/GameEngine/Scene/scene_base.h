#pragma once

#include "../../GameEngine/Camera/camera_base.h"
#include "../../Library/Graphic/font.h"
#include <memory>

class CSceneBase;
typedef std::shared_ptr< CSceneBase > SceneBaseSP;

//�V�[����{�N���X
class CSceneBase{
protected:
	//�T�E���hID
	int m_sound_id;

	//CSV
	CsvSP m_csv;

	//�t�H���g
	FontSP m_font;

	//�J�����C���X�^���X
	CameraBaseSP m_camera;

public:
	//�R���X�g���N�^
	CSceneBase() : m_sound_id( 0 )
	{}

	//���z�f�X�g���N�^
	virtual ~CSceneBase()
	{
		m_csv.reset();
		m_camera.reset();
	}

	//������
	virtual void SceneInit() = 0;

	//���
	virtual void SceneDelete() = 0;

	//���W�b�N
	//����1:�o�ߎ���
	virtual void SceneFrameMove( const float elapsed_time ) = 0;

	//�`��
	//����1:�o�ߎ���
	virtual void SceneFrameRender( const float elapsed_time ) = 0;

	//�T�E���h�Đ�
	virtual void ScenePlaySound() = 0;

	//�T�E���h��~
	virtual void SceneStopSound() = 0;
};

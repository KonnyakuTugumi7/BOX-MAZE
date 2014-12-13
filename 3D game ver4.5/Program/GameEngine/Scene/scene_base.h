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
	int m_SoundId;

	//CSV
	CsvSP m_Csv;

	//�J�����C���X�^���X
	CameraBaseSP m_Camera;

public:
	//�R���X�g���N�^
	CSceneBase() : m_SoundId( 0 )
	{}

	//���z�f�X�g���N�^
	virtual ~CSceneBase()
	{
		m_Csv.reset();
		m_Camera.reset();
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

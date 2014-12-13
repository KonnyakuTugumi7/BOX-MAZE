#pragma once

#include "DXUT.h"
#include "../../GameEngine/Camera/camera_base.h"
#include <memory>

class CViewCamera;
typedef std::shared_ptr< CViewCamera > ViewCameraSP;

//�t�H���g�Ȃǂ��B�e����J�����N���X
class CViewCamera : public CCameraBase{
private:
	//z�l
	float m_Distance;

public:
	//�R���X�g���N�^
    CViewCamera();

	//�f�X�g���N�^
	~CViewCamera(){};

	//����
	static ViewCameraSP Create(){ return std::make_shared< CViewCamera >(); }

	//�����_����ړ�
	//����1:�J�������W , ����2:�����_ , ����3:�����
    void LookAt( const D3DXVECTOR3& eye , const D3DXVECTOR3& target , const D3DXVECTOR3& up );

	//���ߓ��e
	//����1:��p , ����2:�A�X�y�N�g�� , ����3:�ߐڌ��E(Z) , ����4:���u���E(Z)
    void Perspective( const float fov , const float aspect , const float znear , const float zfar );
};
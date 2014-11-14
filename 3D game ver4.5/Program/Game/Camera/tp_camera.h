#pragma once

#include "../../Library/Support/csv.h"
#include "../../GameEngine/Camera/camera_base.h"

#include <d3dx9.h>
#include <memory>

class CTPCamera;
typedef std::shared_ptr< CTPCamera > TPCameraSP;

//�O�l�̎��_�J�����N���X
class CTPCamera : public CCameraBase{
private:
	//�o�x
    float m_longitudeDegrees;

	//�ܓx
    float m_latitudeDegrees;

	//��]���x
	float m_rot_speed;

	//�J�����ƒ����_�̃I�t�Z�b�g(�^�[�Q�b�g����J�����ւ̃x�N�g��)
    D3DXVECTOR3 m_offset;

public:
	//�R���X�g���N�^
    CTPCamera();

	//�f�X�g���N�^
	~CTPCamera(){};

	//����
	static TPCameraSP Create(){ return std::make_shared< CTPCamera >(); }

	//�����_
	//����1:�����_
    void LookAt( const D3DXVECTOR3& target );

	//�����_����ړ�
	//����1:�J�������W , ����2:�����_ , ����3:�����
    void LookAt( const D3DXVECTOR3& eye , const D3DXVECTOR3& target , const D3DXVECTOR3& up );

	//���ߓ��e
	//����1:��p(x) , ����2:�A�X�y�N�g�� , ����3:�ߐڌ��E(Z) , ����4:���u���E(Z)
    void Perspective( const float fovx , const float aspect , const float znear , const float zfar );

	//��]
	//����1:�o�x , ����2:�ܓx , ����3:hoge
    void Rotate( float longitudeDegrees , float latitudeDegrees , float hoge );

	//�J����������
	void Init(){}

	//�J�����X�V
	//����1:�o�ߎ���
    void UpdateCamera( const float elapsedTimeSec );
};
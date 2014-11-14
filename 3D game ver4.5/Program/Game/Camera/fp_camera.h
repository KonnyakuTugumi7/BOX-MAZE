#pragma once

#include "../../Library/Support/csv.h"
#include "../../Library/Input/input.h"
#include "../../GameEngine/Camera/camera_base.h"

#include <d3dx9.h>
#include <memory>

#pragma warning ( disable : 4240 ) //��W���̊g���@�\���g�p����Ă��܂��B

class CFPCamera;
typedef std::shared_ptr< CFPCamera > FPCameraSP;

//��l�̎��_�J�����N���X
class CFPCamera : public CCameraBase{
private:
	//�X���x����
    float m_accumPitchDegrees;
    
	//��]���x
    float m_rotationSpeed;

	//�����x
    D3DXVECTOR3 m_acceleration;

	//���̑��x
    D3DXVECTOR3 m_currentVelocity;

	//���x
    D3DXVECTOR3 m_velocity;

	//�ő���E
	D3DXVECTOR3 m_boundsmax;

	//�ŏ����E
	D3DXVECTOR3 m_boundsmin;

private:
	//�ړ�����
	//����1:����
	void GetMovementDirection( D3DXVECTOR3& direction );

	//��l�̉�]
	//����1:y���ŉ�]������� , ����2:x���ŉ�]�������
    void RotateFirstPerson( float headingDegrees , float pitchDegrees );

	//���x�X�V
	//����1:�ړ����� , ����2:�o�ߎ���
    void UpdateVelocity( const D3DXVECTOR3& direction , const float elapsedTimeSec );

	//�r���[�s��X�V
    void UpdateViewMatrix();

	//���W�X�V
	//����1:���� , ����2:�o�ߎ���
	void UpdatePosition( const D3DXVECTOR3& direction , const float elapsedTimeSec );

	//�J�����ړ�
	//����1:�ړ�����(x) , ����2:�ړ�����(y) , ����3:�ړ�����(z)
    void Move( const float dx , const float dy , const float dz );

	//�J�����ړ�
	//����1:�ړ����� , ����2:�ړ���
    void Move( const D3DXVECTOR3& direction , const D3DXVECTOR3& amount );

	//�����̐���
	void PerformCameraCollisionDetection();

	//�W�����v
	//����1:�o�ߎ���
	void Jump( const float elapsedTimeSec );

	//���ߓ��e
	//����1:��p(x) , ����2:�A�X�y�N�g�� , ����3:�ߐڌ��E(Z) , ����4:���u���E(Z)
   void Perspective( const float fovx , const float aspect , const float znear , const float zfar );

	//��]
	//����1:y���ŉ�]������� , ����2:x���ŉ�]������� , ����3:z���ŉ�]�������
    void Rotate( float headingDegrees , float pitchDegrees , float rollDegrees );

public:
	//�R���X�g���N�^
    CFPCamera();

	//�f�X�g���N�^
	~CFPCamera(){};

	//����
	static FPCameraSP Create(){ return std::make_shared< CFPCamera >(); }

	//������
	void Init();
    
	//�J�����X�V
	//����1:�o�ߎ���
	void UpdateCamera( const float elapsedTimeSec );

    //----------------------------------�Q�b�^�[----------------------------------

    inline const D3DXVECTOR3 &GetAcceleration() const { return m_acceleration; }
    inline const D3DXVECTOR3 &GetCurrentVelocity() const { return m_currentVelocity; }
    inline const D3DXVECTOR3 &GetVelocity() const { return m_velocity; }
	inline const float GetRotationSpeed() const { return m_rotationSpeed; }
    
    //----------------------------------�Z�b�^�[----------------------------------

	inline void SetPosition( const D3DXVECTOR3& eye )
	{
		m_eye = eye;

		//�r���[�s����X�V
		UpdateViewMatrix();
	}
	inline void SetPosition( const float x , const float y , const float z )
	{
		m_eye.x = x;
		m_eye.y = y;
		m_eye.z = z;

		//�r���[�s����X�V
		UpdateViewMatrix();
	}
	inline void SetOrientation( const D3DXQUATERNION& orientation )
	{
		D3DXMATRIX m;

		//�N�H�[�^�j�I�������]�s����쐬
		D3DXMatrixRotationQuaternion( &m , &orientation );

		//�X���x����
		m_accumPitchDegrees = D3DXToDegree( asinf( m( 1 , 2 ) ) );

		//�V�������ʂ��Z�b�g
		m_orientation = orientation;

		LookAt( m_eye , m_eye + m_viewDir , WORLD_YAXIS );

		//�r���[�s����X�V
		UpdateViewMatrix();
	}
};

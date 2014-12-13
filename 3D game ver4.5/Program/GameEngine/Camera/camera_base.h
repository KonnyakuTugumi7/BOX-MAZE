#pragma once

#include "../../Library/Support/csv.h"

#include <d3dx9.h>
#include <memory>

#pragma warning ( disable : 4240 ) //��W���̊g���@�\���g�p����Ă��܂��B
#pragma warning ( disable : 4238 ) //��W���̊g���@�\���g�p����Ă��܂��B: �E�Ӓl�̃N���X�����Ӓl�Ɏg�p����܂��B

////������N���X
//class CViewFrustum{
//public:
//	//������̊e��
//	D3DXPLANE m_left;
//	D3DXPLANE m_right;
//	D3DXPLANE m_top;
//	D3DXPLANE m_bottom;
//	D3DXPLANE m_near;
//	D3DXPLANE m_far;
//
//public:
//	//�R���X�g���N�^
//	CViewFrustum(){}
//
//	//�f�X�g���N�^
//	~CViewFrustum(){}
//
//	//�R�s�[�R���X�g���N�^
//	CViewFrustum( const CViewFrustum& frustum )
//	{
//		m_left = frustum.m_left;
//		m_right = frustum.m_right;
//		m_top = frustum.m_top;
//		m_bottom = frustum.m_bottom;
//		m_near = frustum.m_near;
//		m_far = frustum.m_far;
//	}
//
//	// = ���Z�q�̃I�[�o�[���[�h
//	inline CViewFrustum& operator = (const CViewFrustum& frustum)
//	{
//		m_left = frustum.m_left;
//		m_right = frustum.m_right;
//		m_top = frustum.m_top;
//		m_bottom = frustum.m_bottom;
//		m_near = frustum.m_near;
//		m_far = frustum.m_far;
//		return ( *this );
//	}
//};

class CCameraBase;
typedef std::shared_ptr< CCameraBase > CameraBaseSP;
typedef std::weak_ptr< CCameraBase > CameraBaseWP;

//�J�����N���X
class CCameraBase{
protected:
	//-----------------�ÓI�����o�ϐ�-----------------
	//���[���hY��
    static const D3DXVECTOR3 WORLD_YAXIS;

public:
	//���W
    D3DXVECTOR3 m_Eye;

	//�O�t���[���̍��W
	D3DXVECTOR3 m_PrevEye;

protected:
	//�d�͂������邩�ǂ���
	bool m_IsGravity;

	//�n��(���̂̏��)�ڒn�t���O
	bool m_IsOnGround;

	//Y���ړ���
	float m_JumpMovement;

	//AABB�̊e���̒���(�����蔻��)
	D3DXVECTOR3 m_Length;

	//�����_(look)
    D3DXVECTOR3 m_Target;
    
	//X��
    D3DXVECTOR3 m_XAxis;

	//Y��
    D3DXVECTOR3 m_YAxis;

	//Z��
    D3DXVECTOR3 m_ZAxis;

	//�������
    D3DXVECTOR3 m_ViewDir;

	//����
    D3DXQUATERNION m_Orientation;
	
	//�r���[�s��
    D3DXMATRIX m_ViewMatrix;

	//�v���W�F�N�V����(���e)�s��
    D3DXMATRIX m_ProjMatrix;

	//CSV
	CsvSP m_Csv;

public:
	//�R���X�g���N�^
    CCameraBase();

	//�f�X�g���N�^
	virtual ~CCameraBase(){ m_Csv.reset(); };

	//�����_�ړ�
	//����1:�����_
	virtual void LookAt( const D3DXVECTOR3& target ){};

	//�����_�ړ�
	//����1:�J�������W , ����2:�����_ , ����3:�����
	virtual void LookAt( const D3DXVECTOR3& eye , const D3DXVECTOR3& target , const D3DXVECTOR3& up ){};

	//���ߓ��e
	//����1:��p(x) , ����2:�A�X�y�N�g�� , ����3:�ߐڌ��E(Z) , ����4:���u���E(Z)
	virtual void Perspective( const float fovx , const float aspect , const float znear , const float zfar ){};

	//��]
	//����1:y���ŉ�]������� , ����2:x���ŉ�]������� , ����3:z���ŉ�]�������
	virtual void Rotate( float headingDegrees , float pitchDegrees , float rollDegrees ){};
    
	//�J�����X�V
	//����1:�o�ߎ���
	virtual void UpdateCamera( const float elapsedTimeSec ){};

	//�J����������
	virtual void Init(){};

    //----------------------------------�Q�b�^�[----------------------------------

    inline const D3DXVECTOR3 &GetPosition() const { return m_Eye; }
	inline const D3DXVECTOR3 GetSize() const { return m_Length; }
	inline const D3DXVECTOR3 GetViewDir() const { return m_ViewDir; }
	inline const D3DXVECTOR3 GetViewTarget() const { return m_Target; }
	inline const D3DXMATRIX &GetViewMatrix() const { return m_ViewMatrix; }
	inline const D3DXMATRIX &GetProjectionMatrix() const { return m_ProjMatrix; }
	inline const float GetJumpMovement() const { return m_JumpMovement; }
	//CViewFrustum GetViewFrustum();

	//----------------------------------�Z�b�^�[----------------------------------

	inline void SetIsGravity( const bool flg ){ m_IsGravity = flg; }
	inline void SetIsOnGround( const bool flg ){ m_IsOnGround = flg; }
	inline void SetJumpMovement( const float jump_movement ){ m_JumpMovement = jump_movement; }
};

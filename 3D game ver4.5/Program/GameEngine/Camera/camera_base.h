#pragma once

#include "../../Library/Support/csv.h"

#include <d3dx9.h>
#include <memory>

#pragma warning ( disable : 4240 ) //��W���̊g���@�\���g�p����Ă��܂��B
#pragma warning ( disable : 4238 ) //��W���̊g���@�\���g�p����Ă��܂��B: �E�Ӓl�̃N���X�����Ӓl�Ɏg�p����܂��B

//������N���X
class CViewFrustum{
public:
	//������̊e��
	D3DXPLANE m_left;
	D3DXPLANE m_right;
	D3DXPLANE m_top;
	D3DXPLANE m_bottom;
	D3DXPLANE m_near;
	D3DXPLANE m_far;

public:
	//�R���X�g���N�^
	CViewFrustum(){}

	//�f�X�g���N�^
	~CViewFrustum(){}

	//�R�s�[�R���X�g���N�^
	CViewFrustum( const CViewFrustum& frustum )
	{
		m_left = frustum.m_left;
		m_right = frustum.m_right;
		m_top = frustum.m_top;
		m_bottom = frustum.m_bottom;
		m_near = frustum.m_near;
		m_far = frustum.m_far;
	}

	// = ���Z�q�̃I�[�o�[���[�h
	inline CViewFrustum& operator = (const CViewFrustum& frustum)
	{
		m_left = frustum.m_left;
		m_right = frustum.m_right;
		m_top = frustum.m_top;
		m_bottom = frustum.m_bottom;
		m_near = frustum.m_near;
		m_far = frustum.m_far;
		return ( *this );
	}
};

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
    D3DXVECTOR3 m_eye;

	//�O�t���[���̍��W
	D3DXVECTOR3 m_prev_eye;

protected:
	//�d�͂������邩�ǂ���
	bool m_is_gravity;

	//�n��(���̂̏��)�ڒn�t���O
	bool m_is_on_ground;

	//Y���ړ���
	float m_jump_movement;

	//AABB�̊e���̒���(�����蔻��)
	D3DXVECTOR3 m_length;

	//�����_(look)
    D3DXVECTOR3 m_target;
    
	//X��
    D3DXVECTOR3 m_xAxis;

	//Y��
    D3DXVECTOR3 m_yAxis;

	//Z��
    D3DXVECTOR3 m_zAxis;

	//�������
    D3DXVECTOR3 m_viewDir;

	//����
    D3DXQUATERNION m_orientation;
	
	//�r���[�s��
    D3DXMATRIX m_viewMatrix;

	//�v���W�F�N�V����(���e)�s��
    D3DXMATRIX m_projMatrix;

	//CSV
	CsvSP m_csv;

public:
	//�R���X�g���N�^
    CCameraBase();

	//�f�X�g���N�^
	virtual ~CCameraBase(){ m_csv.reset(); };

	//�����_�ړ�
	//����1:�����_
	virtual void LookAt( const D3DXVECTOR3& target ){};

	//�����_�ړ�
	//����1:�J�������W , ����2:�����_ , ����3:�����
	virtual void LookAt( const D3DXVECTOR3& eye , const D3DXVECTOR3& target , const D3DXVECTOR3& up ){};

	//���ߓ��e
	//����1:��p(x) , ����2:�A�X�y�N�g�� , ����3:�ߐڌ��E(Z) , ����4:���u���E(Z)
	virtual void Perspective( const float fovx , const float aspect , const float znear , const float zfar ) = 0;

	//��]
	//����1:y���ŉ�]������� , ����2:x���ŉ�]������� , ����3:z���ŉ�]�������
	virtual void Rotate( float headingDegrees , float pitchDegrees , float rollDegrees ) = 0;
    
	//�J�����X�V
	//����1:�o�ߎ���
	virtual void UpdateCamera( const float elapsedTimeSec ) = 0;

	//�J����������
	virtual void Init() = 0;

    //----------------------------------�Q�b�^�[----------------------------------

    inline const D3DXVECTOR3 &GetPosition() const { return m_eye; }
	inline const D3DXVECTOR3 GetSize() const { return m_length; }
	inline const D3DXVECTOR3 GetViewDir() const { return m_viewDir; }
	inline const D3DXVECTOR3 GetViewTarget() const { return m_target; }
	inline const D3DXMATRIX &GetViewMatrix() const { return m_viewMatrix; }
	inline const D3DXMATRIX &GetProjectionMatrix() const { return m_projMatrix; }
	inline const float GetJumpMovement() const { return m_jump_movement; }
	CViewFrustum GetViewFrustum();

	//----------------------------------�Z�b�^�[----------------------------------

	inline void SetIsGravity( const bool flg ){ m_is_gravity = flg; }
	inline void SetIsOnGround( const bool flg ){ m_is_on_ground = flg; }
	inline void SetJumpMovement( const float jump_movement ){ m_jump_movement = jump_movement; }
};

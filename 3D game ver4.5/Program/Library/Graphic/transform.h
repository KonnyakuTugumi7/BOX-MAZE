#pragma once

#include "DXUT.h"
#include <memory>

#pragma warning ( disable : 4238)	//��W���̊g���@�\���g�p����Ă��܂� : �E�Ӓl�̃N���X�����Ӓl�Ɏg�p����܂�

//�g�����X�t�H�[���N���X
class CTransform{
protected:
	//��]�^�C�v
	enum{
		//XYZ
		ROT_TYPE_XYZ , 
		
		//�s��
		ROT_TYPE_MATRIX , 

		//�N�H�[�^�j�I��
		ROT_TYPE_QUATERNION
	};

	//��]�^�C�v
	int m_rot_type;

	//���a //�n���̃��f���̔��a�̓K���Ȓl(0.5f)
	float m_rad;
	
	//�p��
	//����� VEC3 �����p����
	//�x�N�g���A�s��A�N�H�[�^�j�I���̂ǂ�ł�
	//����ł���悤�ɂ��ׂ�
	D3DXVECTOR3 m_rot;
		
	//��]�s��(x)
	D3DXMATRIX m_rot_x;
	
	//��]�s��(y)
	D3DXMATRIX m_rot_y;

	//��]�s��(z)
	D3DXMATRIX m_rot_z;

	//�N�H�[�^�j�I��
	D3DXQUATERNION m_rot_qtn;

	//���[���h�}�g���b�N�X(���[���h��Ԃ���Ƃ������̎p��)
	D3DXMATRIX m_wld_mtx;

public:
	//���W
	D3DXVECTOR3 m_pos;

	//�O�̃t���[���̍��W
	D3DXVECTOR3 m_prev_pos;

	//�X�P�[��
	D3DXVECTOR3 m_scl;

	//��]�s��
	D3DXMATRIX m_rot_mtx;

public:
	//�R���X�g���N�^
	CTransform();

	//�f�X�g���N�^
	virtual ~CTransform(){}

	//���[���h�}�g���b�N�X�Q�b�^�[
	D3DXMATRIX GetWldMtx(){ return m_wld_mtx; }

	//���[���h�}�g���b�N�X��ݒ�
	//����1:�e�̃��[���h�}�g���b�N�X
	void SetWldMtx( D3DXMATRIX* parent_mtx = NULL );

	//����1:��]��(x) , ����2:��]��(y) , ����3:��]��(z)
	void UpdateRotMtx( const float& rot_x , const float& rot_y , const float& rot_z );

	//����1:��]��
	void UpdateRotMtx( const D3DXVECTOR3& rot );
};
#pragma once

#pragma warning( disable : 4715 ) //�l��Ԃ��Ȃ��R���g���[�� �p�X������܂��B
#pragma warning( disable : 4238 ) //��W���̊g���@�\���g�p����Ă��܂� : �E�Ӓl�̃N���X�����Ӓl�Ɏg�p����܂��B
#pragma warning( disable : 4482 ) //��W���̊g���@�\���g�p����Ă��܂�: enum 'CCollisionManager::HIT_DIRECTION' �����薼�Ŏg�p����܂��B
#pragma warning( disable : 4239 ) //��W���̊g���@�\���g�p����Ă��܂� : 'default argument' : 'D3DXVECTOR3' ���� 'D3DXVECTOR3 &' �ւ̕ϊ��ł��B
#pragma warning( disable : 4244 ) //'����' : 'const int' ���� 'FLOAT' �ւ̕ϊ��ł��B�f�[�^��������\��������܂��B

//�N���X�̑O���錾
//������N���X
class CViewFrustum;

//�����蔻��N���X
class CCollisionManager{
public:
	//AABB��
	static enum HIT_DIRECTION
	{
		NONE = 0,
		LEFT = 1,
		RIGHT = 2,
		FRONT = 3,
		BACK = 4,
		TOP = 5,
		BOTTOM = 6,
		EXCEPTION = 7
	};

private:
	//�R���X�g���N�^
	CCollisionManager(){}

	//�f�X�g���N�^
	~CCollisionManager(){}

public:
	//AABB��AABB�̓����蔻��Ɠ��������ʂ����o���␳����
	//����1:A�̒��S , ����2:A�̑O�t���[���̒��S , ����3:A�̊e�ӂ̒��� , ����4:B�̒��S , ����5:B�̑O�t���[���̒��S , ����6:B�̊e�ӂ̒��� , ����7:�␳������W
	//�߂�l:����������
	static HIT_DIRECTION CollisionAABBAABB( const D3DXVECTOR3 posA , const D3DXVECTOR3 prev_posA , const D3DXVECTOR3 lengthA , const D3DXVECTOR3 posB , const D3DXVECTOR3 prev_posB , const D3DXVECTOR3 lengthB , D3DXVECTOR3& pos = D3DXVECTOR3( 0 , 0 , 0 ) );

	//AABB�ƌ����Ƃ̌�������
	//����1:�����̎n�_ , ����2:�����̕��� , ����3:AABB�̍��W , ����4:AABB�̊e�ӂ̒��� , ����5:AABB�̃��[���h�ϊ��s�� , ����6:�ՓˊԊu , ����7:�Փˈʒu
	static bool CollisionRayAABB( const D3DXVECTOR3& pos , const D3DXVECTOR3& dir , const D3DXVECTOR3 posAABB , const D3DXVECTOR3 lengthAABB , const D3DXMATRIX& mat , float& t , D3DXVECTOR3& colPos = D3DXVECTOR3( 0 , 0 , 0 ) );

	//AABB�Ɠ_�Ƃ̌�������
	//����1:AABB�̍��W , ����2:AABB�̊e�ӂ̒��� , ����3:�_�̍��W
	static bool CollisionPointAABB( const D3DXVECTOR3& pos , const D3DXVECTOR3& length , const D3DXVECTOR3& point );

	//�X�N���[�����W�̎w��Ń��C���쐬����
	//����1:���C��̍��W(�v�Z���ʂ��i�[) , ����2:���C�̕���(�v�Z���ʂ��i�[����) ����3:�X�N���[��X���W , ����4:�X�N���[��Y���W , ����5:�J�����̃r���[�s�� , ����6:�J�����̃v���W�F�N�V�����s��
	static void ScreenToRay( D3DXVECTOR3& pos , D3DXVECTOR3& dir , const int sx , const int sy , const D3DXMATRIX& view , const D3DXMATRIX& prj );

	//2�_�Ԃ̋������Z�o
	//����1:�n�_ , ����2:�I�_
	static float CalcDistance( const D3DXVECTOR3& v_s , const D3DXVECTOR3& v_e );
};
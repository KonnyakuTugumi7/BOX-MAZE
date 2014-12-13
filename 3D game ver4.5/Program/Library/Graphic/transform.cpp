#include "DXUT.h"
#include "transform.h"

//�R���X�g���N�^
CTransform::CTransform() : 
	m_RotType( ROT_TYPE_MATRIX ) ,
	m_Pos( 0 , 0 , 0 ) ,
	m_PrevPos( 0 , 0 , 0 ) ,
	m_Scl( 1 , 1 , 1 ) ,
	m_Rot( 0 , 0 , 0 ) ,
	m_Rad( 0.5f )
{
	//�s��̏�����
	D3DXMatrixIdentity( &m_RotMtx );
	D3DXMatrixIdentity( &m_RotX );
	D3DXMatrixIdentity( &m_RotY );
	D3DXMatrixIdentity( &m_RotZ );
	D3DXMatrixIdentity( &m_WldMtx );

	//�N�H�[�^�j�I���̏�����
	D3DXQuaternionIdentity( &m_RotQtn );
}

//���[���h�}�g���b�N�X��ݒ�
void CTransform::SetWldMtx( D3DXMATRIX* parent_mtx )
{
	//���W��ۑ�
	m_PrevPos = m_Pos;

	//-----------------------------���[���h�}�g���N�X�ݒ�(���[�J�����W���烏�[���h���W��)-------------------------------------

	D3DXMATRIX SclMtx , RotMtx , PosMtx;

	//--------------------------��](��y,x,z�̏���)--------------------------
	if( CTransform::ROT_TYPE_XYZ == m_RotType )
	{
		D3DXMatrixRotationYawPitchRoll( &RotMtx, m_Rot.y, m_Rot.x, m_Rot.z );
	}
	else if( CTransform::ROT_TYPE_MATRIX == m_RotType )
	{
		m_RotMtx = m_RotX * m_RotY * m_RotZ; 
		RotMtx = m_RotMtx;
	}
	else if( CTransform::ROT_TYPE_QUATERNION == m_RotType )
	{
		//�N�H�[�^�j�I������}�g���N�X�𐶐�����
		D3DXMatrixRotationQuaternion( &RotMtx, &m_RotQtn );
	}

	//---------------------------�g��---------------------------
	D3DXMatrixScaling( &SclMtx , m_Scl.x , m_Scl.y , m_Scl.z );

	//--------------------------���s�ړ�--------------------------
	D3DXMatrixTranslation( &PosMtx, m_Pos.x, m_Pos.y, m_Pos.z );

	//�e�̃��[���h�}�g���b�N�X��NULL����Ȃ��Ȃ�
	//�e������̂Őe�̎p�����l�����������̎p����ݒ�
	if( parent_mtx )
	{
		m_WldMtx = SclMtx * RotMtx * PosMtx * ( *parent_mtx );
	}
	else
	{
		m_WldMtx =  SclMtx * RotMtx * PosMtx;
	}
}

//��]�s��X�V
void CTransform::UpdateRotMtx( const float& rot_x , const float& rot_y , const float& rot_z )
{
	//---------------�e��������e��]�s����v�Z---------------
	D3DXMatrixRotationX( &m_RotX , D3DXToRadian( rot_x ) );
	D3DXMatrixRotationY( &m_RotY , D3DXToRadian( rot_y ) );
	D3DXMatrixRotationZ( &m_RotZ , D3DXToRadian( rot_z ) );

	m_RotMtx = m_RotX * m_RotY * m_RotZ;
}

//��]�s��X�V
void CTransform::UpdateRotMtx( const D3DXVECTOR3& rot )
{
	//---------------�e��������e��]�s����v�Z---------------
	D3DXMatrixRotationX( &m_RotX, D3DXToRadian( rot.x ) );
	D3DXMatrixRotationY( &m_RotY, D3DXToRadian( rot.y ) );
	D3DXMatrixRotationZ( &m_RotZ, D3DXToRadian( rot.z ) );

	m_RotMtx = m_RotX * m_RotY * m_RotZ;
}
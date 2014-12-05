#include "DXUT.h"
#include "transform.h"

//�R���X�g���N�^
CTransform::CTransform() : 
	m_rot_type( ROT_TYPE_MATRIX ) ,
	m_pos( 0 , 0 , 0 ) ,
	m_prev_pos( 0 , 0 , 0 ) ,
	m_scl( 1 , 1 , 1 ) ,
	m_rot( 0 , 0 , 0 ) ,
	m_rad( 0.5f )
{
	//�s��̏�����
	D3DXMatrixIdentity( &m_rot_mtx );
	D3DXMatrixIdentity( &m_rot_x );
	D3DXMatrixIdentity( &m_rot_y );
	D3DXMatrixIdentity( &m_rot_z );
	D3DXMatrixIdentity( &m_wld_mtx );

	//�N�H�[�^�j�I���̏�����
	D3DXQuaternionIdentity( &m_rot_qtn );
}

//���[���h�}�g���b�N�X��ݒ�
void CTransform::SetWldMtx( D3DXMATRIX* parent_mtx )
{
	//���W��ۑ�
	m_prev_pos = m_pos;

	//-----------------------------���[���h�}�g���N�X�ݒ�(���[�J�����W���烏�[���h���W��)-------------------------------------

	D3DXMATRIX SclMtx , RotMtx , PosMtx;

	//--------------------------��](��y,x,z�̏���)--------------------------
	if( CTransform::ROT_TYPE_XYZ == m_rot_type )
	{
		D3DXMatrixRotationYawPitchRoll( &RotMtx, m_rot.y, m_rot.x, m_rot.z );
	}
	else if( CTransform::ROT_TYPE_MATRIX == m_rot_type )
	{
		m_rot_mtx = m_rot_x * m_rot_y * m_rot_z; 
		RotMtx = m_rot_mtx;
	}
	else if( CTransform::ROT_TYPE_QUATERNION == m_rot_type )
	{
		//�N�H�[�^�j�I������}�g���N�X�𐶐�����
		D3DXMatrixRotationQuaternion( &RotMtx, &m_rot_qtn );
	}

	//---------------------------�g��---------------------------
	D3DXMatrixScaling( &SclMtx , m_scl.x , m_scl.y , m_scl.z );

	//--------------------------���s�ړ�--------------------------
	D3DXMatrixTranslation( &PosMtx, m_pos.x, m_pos.y, m_pos.z );

	//�e�̃��[���h�}�g���b�N�X��NULL����Ȃ��Ȃ�
	//�e������̂Őe�̎p�����l�����������̎p����ݒ�
	if( parent_mtx )
	{
		m_wld_mtx = SclMtx * RotMtx * PosMtx * ( *parent_mtx );
	}
	else
	{
		m_wld_mtx =  SclMtx * RotMtx * PosMtx;
	}
}

//��]�s��X�V
void CTransform::UpdateRotMtx( const float& rot_x , const float& rot_y , const float& rot_z )
{
	//---------------�e��������e��]�s����v�Z---------------
	D3DXMatrixRotationX( &m_rot_x , D3DXToRadian( rot_x ) );
	D3DXMatrixRotationY( &m_rot_y , D3DXToRadian( rot_y ) );
	D3DXMatrixRotationZ( &m_rot_z , D3DXToRadian( rot_z ) );

	m_rot_mtx = m_rot_x * m_rot_y * m_rot_z;
}

//��]�s��X�V
void CTransform::UpdateRotMtx( const D3DXVECTOR3& rot )
{
	//---------------�e��������e��]�s����v�Z---------------
	D3DXMatrixRotationX( &m_rot_x, D3DXToRadian( rot.x ) );
	D3DXMatrixRotationY( &m_rot_y, D3DXToRadian( rot.y ) );
	D3DXMatrixRotationZ( &m_rot_z, D3DXToRadian( rot.z ) );

	m_rot_mtx = m_rot_x * m_rot_y * m_rot_z;
}
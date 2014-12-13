#include "DXUT.h"
#include "camera_base.h"
#include "../../Library/Input/input.h"

//Camera�N���X�̐ÓI�����o�ϐ��̎���
const D3DXVECTOR3 CCameraBase::WORLD_YAXIS( 0.0f , 1.0f , 0.0f );

//�R���X�g���N�^
CCameraBase::CCameraBase()
{
	//�d�̓t���O
	m_IsGravity = false;

	//�ڒn�t���O
	m_IsOnGround = false;

	//Y���ړ���
	m_JumpMovement = 0.0f;

	//AABB�̊e���̒���
	m_Length = D3DXVECTOR3( 3.0f , 3.0f , 3.0f );

	//���W
    m_Eye = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//�O�t���[�����W
	m_PrevEye = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//�����_
    m_Target = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//X���W
	m_XAxis = D3DXVECTOR3( 1.0f , 0.0f , 0.0f );
    
	//Y���W
	m_YAxis = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );
    
	//Z���W
	m_ZAxis = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
    
	//�������
	m_ViewDir = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//����
    D3DXQuaternionIdentity( &m_Orientation );

	//�r���[�s��
	D3DXMatrixIdentity( &m_ViewMatrix );

	//�v���W�F�N�V����(���e)�s��
	D3DXMatrixIdentity( &m_ProjMatrix );
}

////������Q�b�^�[
//CViewFrustum CCameraBase::GetViewFrustum()
//{
//	//�����䐶��
//	D3DXVECTOR3 near_tl( -1.0f , 1.0f , 0.0f );
//	D3DXVECTOR3 near_tr( 1.0f , 1.0f , 0.0f );
//	D3DXVECTOR3 near_bl( -1.0f , -1.0f , 0.0f );
//	D3DXVECTOR3 near_br( 1.0f , -1.0f , 0.0f );
//	D3DXVECTOR3 far_tl( -1.0f ,  1.0f , 1.0f );
//	D3DXVECTOR3 far_tr( 1.0f , 1.0f , 1.0f );
//	D3DXVECTOR3 far_bl( -1.0f , -1.0f , 1.0f );
//	D3DXVECTOR3 far_br( 1.0f , -1.0f , 1.0f );
//
//	D3DXMATRIX inv_view_proj;
//	D3DXMatrixInverse( &inv_view_proj , NULL , &( GetViewMatrix() * GetProjectionMatrix() ) );
//	D3DXVec3TransformCoord( &near_tl , &near_tl , &inv_view_proj );
//	D3DXVec3TransformCoord( &near_tr , &near_tr , &inv_view_proj );
//	D3DXVec3TransformCoord( &near_bl , &near_bl , &inv_view_proj );
//	D3DXVec3TransformCoord( &near_br , &near_br , &inv_view_proj );
//	D3DXVec3TransformCoord( &far_tl , & far_tl , &inv_view_proj );
//	D3DXVec3TransformCoord( &far_tr , & far_tr , &inv_view_proj );
//	D3DXVec3TransformCoord( &far_bl , & far_bl , &inv_view_proj );
//	D3DXVec3TransformCoord( &far_br , & far_br , &inv_view_proj );
//
//	// �����䕽�ʐ���
//	CViewFrustum frustum;
//	D3DXPlaneFromPoints( &frustum.m_left , & far_bl , &far_tl , &near_tl );
//	D3DXPlaneFromPoints( &frustum.m_right , &near_tr , &far_tr , &far_br );
//	D3DXPlaneFromPoints( &frustum.m_top , &near_tr , &near_tl , &far_tl );
//	D3DXPlaneFromPoints( &frustum.m_bottom , &far_bl , &near_bl , &near_br );
//	D3DXPlaneFromPoints( &frustum.m_near , &near_bl , &near_tl , &near_tr );
//	D3DXPlaneFromPoints( &frustum.m_far , &far_tr , & far_tl , &far_bl );
//
//	// ���K��
//	D3DXPlaneNormalize( &frustum.m_left , &frustum.m_left );
//	D3DXPlaneNormalize( &frustum.m_right , &frustum.m_right );
//	D3DXPlaneNormalize( &frustum.m_top , &frustum.m_top );
//	D3DXPlaneNormalize( &frustum.m_bottom , &frustum.m_bottom );
//	D3DXPlaneNormalize( &frustum.m_near , &frustum.m_near );
//	D3DXPlaneNormalize( &frustum.m_far , &frustum.m_far );
//
//	return frustum;
//}
#include "DXUT.h"
#include "view_camera.h"
#include "../../Library/Graphic/graphics_manager.h"

//�R���X�g���N�^
CViewCamera::CViewCamera() : m_Distance( 1.0f )
{
	Perspective( /*80.0f*/D3DXToRadian( 45.0f ) , 0.0f , 0.0f , 0.0f );
	LookAt( D3DXVECTOR3( 0 , 0 , 0 ) , D3DXVECTOR3( 0 , 0 , 0 ) , D3DXVECTOR3( 0 , 1 , 0 ) );
}

//�����_����ړ�
void CViewCamera::LookAt( const D3DXVECTOR3& eye , const D3DXVECTOR3& target , const D3DXVECTOR3& up )
{
	//�J�������W�ƃ^�[�Q�b�g���Z�b�g
    m_Eye = eye;
	m_Eye.z += m_Distance;
    m_Target = target;

	//�r���[�s��X�V
	D3DXMatrixLookAtLH( &m_ViewMatrix , &m_Eye , &m_Target , &up );

    D3DXQuaternionRotationMatrix( &m_Orientation , &m_ViewMatrix );
}

//���ߓ��e
void CViewCamera::Perspective( const float fov , const float aspect , const float znear , const float zfar )
{
	//��p����A�X�y�N�g����l�������E�B���h�E���œ��e����Z�������Z�o����
	m_Distance = ( CGraphicsManager::m_WindowHeight * 0.5f ) * -tan( ( D3DX_PI - fov ) * 0.5f);

	//�v���W�F�N�V�����s����X�V����
	D3DXMatrixPerspectiveFovLH( &m_ProjMatrix , fov , CGraphicsManager::m_WindowWidth / CGraphicsManager::m_WindowHeight , 1 , abs( m_Distance * 2.0f ) );
	//CGraphicsManager::m_pD3dDevice->SetTransform( D3DTS_PROJECTION , &m_ProjMatrix );
}
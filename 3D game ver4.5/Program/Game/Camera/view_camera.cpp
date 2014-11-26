#include "DXUT.h"
#include "view_camera.h"
#include "../../Library/Graphic/graphics_manager.h"

//�R���X�g���N�^
CViewCamera::CViewCamera() : m_distance( 1.0f )
{
	Perspective( /*80.0f*/D3DXToRadian( 45.0f ) , 0.0f , 0.0f , 0.0f );
	LookAt( D3DXVECTOR3( 0 , 0 , 0 ) , D3DXVECTOR3( 0 , 0 , 0 ) , D3DXVECTOR3( 0 , 1 , 0 ) );
}

//�����_����ړ�
void CViewCamera::LookAt( const D3DXVECTOR3& eye , const D3DXVECTOR3& target , const D3DXVECTOR3& up )
{
	//�J�������W�ƃ^�[�Q�b�g���Z�b�g
    m_eye = eye;
	m_eye.z += m_distance;
    m_target = target;

	//�r���[�s��X�V
	D3DXMatrixLookAtLH( &m_viewMatrix , &m_eye , &m_target , &up );

    D3DXQuaternionRotationMatrix( &m_orientation , &m_viewMatrix );
}

//���ߓ��e
void CViewCamera::Perspective( const float fov , const float aspect , const float znear , const float zfar )
{
	//��p����A�X�y�N�g����l�������E�B���h�E���œ��e����Z�������Z�o����
	m_distance = ( CGraphicsManager::m_window_height * 0.5f ) * -tan( ( D3DX_PI - fov ) * 0.5f);

	//�v���W�F�N�V�����s����X�V����
	D3DXMatrixPerspectiveFovLH( &m_projMatrix , fov , CGraphicsManager::m_window_width / CGraphicsManager::m_window_height , 1 , abs( m_distance * 2.0f ) );
	//CGraphicsManager::m_pd3dDevice->SetTransform( D3DTS_PROJECTION , &m_projMatrix );
}
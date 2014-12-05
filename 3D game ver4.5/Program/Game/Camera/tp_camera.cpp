#include "DXUT.h"
#include <cmath>
#include "tp_camera.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"

//�R���X�g���N�^
CTPCamera::CTPCamera()
{
	//CSV
	m_csv = CCsv::Create( "Content/csv/TPCameraData.csv" );

	//�o�x
    m_longitudeDegrees = 0.0f;

	//�ܓx
    m_latitudeDegrees = 0.0f;

	//�J�����ƒ����_�̃I�t�Z�b�g
    m_offset = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//��]���x
	m_rot_speed = m_csv->GetToken< float >( 5 , 0 );

	//�ˉe�ϊ�
	Perspective( m_csv->GetToken< float >( 0 , 0 ) , 1.0f , m_csv->GetToken< float >( 1 , 0 ) , m_csv->GetToken< float >( 2 , 0 ) );

	//�����_����ړ�
	LookAt( D3DXVECTOR3( 0.0f , /*m_csv->GetToken< float >( 3 , 0 )*/0 , m_csv->GetToken< float >( 3 , 1 ) ) , D3DXVECTOR3( 0.0f , /*m_csv->GetToken< float >( 4 , 0 )*/0 , 0 ) , D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );
}

//�����_
void CTPCamera::LookAt( const D3DXVECTOR3& target )
{
    m_target = target;
}

//�����_����ړ�
void CTPCamera::LookAt( const D3DXVECTOR3& eye , const D3DXVECTOR3& target , const D3DXVECTOR3& up )
{
	//�J�������W�ƃ^�[�Q�b�g���Z�b�g
    m_eye = eye;
    m_target = target;

    m_zAxis = target - eye;
    m_offset.x = -m_zAxis.x;
    m_offset.y = -m_zAxis.y;
    m_offset.z = -m_zAxis.z;
    D3DXVec3Normalize( &m_zAxis , &m_zAxis );

	//x�������̃x�N�g�������߂�
    D3DXVec3Cross( &m_xAxis, &up , &m_zAxis );
    D3DXVec3Normalize( &m_xAxis , &m_xAxis );

	//y�������̃x�N�g�������߂�
    D3DXVec3Cross( &m_yAxis , &m_zAxis , &m_xAxis );
    D3DXVec3Normalize( &m_yAxis , &m_yAxis );
    D3DXVec3Normalize( &m_xAxis , &m_xAxis );

    D3DXMatrixIdentity( &m_viewMatrix );

	////�r���[�s��X�V
 //   m_viewMatrix( 0 , 0 ) = m_xAxis.x;
 //   m_viewMatrix( 1 , 0 ) = m_xAxis.y;
 //   m_viewMatrix( 2 , 0 ) = m_xAxis.z;
 //   m_viewMatrix( 3 , 0 ) = -D3DXVec3Dot( &m_xAxis , &eye );

 //   m_viewMatrix( 0 , 1 ) = m_yAxis.x;
 //   m_viewMatrix( 1 , 1 ) = m_yAxis.y;
 //   m_viewMatrix( 2 , 1 ) = m_yAxis.z;
 //   m_viewMatrix( 3 , 1 ) = -D3DXVec3Dot( &m_yAxis, &eye );

 //   m_viewMatrix( 0 , 2 ) = m_zAxis.x;
 //   m_viewMatrix( 1 , 2 ) = m_zAxis.y;
 //   m_viewMatrix( 2 , 2 ) = m_zAxis.z;
 //   m_viewMatrix( 3 , 2 ) = -D3DXVec3Dot( &m_zAxis , &eye );

	//���������ݒ�
	m_viewDir = m_zAxis;

	//�r���[�s��X�V
	D3DXMatrixLookAtLH( &m_viewMatrix , &m_eye , &m_target , &up );

    D3DXQuaternionRotationMatrix( &m_orientation , &m_viewMatrix );
}

//���ߓ��e
void CTPCamera::Perspective( const float fovx , const float aspect , const float znear , const float zfar )
{
    float e = 1.0f / tanf( D3DXToRadian( fovx ) / 2.0f );
    float aspectInv = 1.0f / aspect;
    float fovy = 2.0f * atanf( aspectInv / e );
    //float xScale = 1.0f / tanf( 0.5f * fovy );
    //float yScale = xScale / aspectInv;

	//�ˉe�ϊ�
	D3DXMatrixPerspectiveFovLH( &m_projMatrix , fovy , aspect , znear , zfar );
	CGraphicsManager::m_pd3dDevice->SetTransform( D3DTS_PROJECTION , &m_projMatrix );

	////�ˉe�ϊ�
 //   m_projMatrix( 0 , 0 ) = xScale;
 //   m_projMatrix( 1 , 0 ) = 0.0f;
 //   m_projMatrix( 2 , 0 ) = 0.0f;
 //   m_projMatrix( 3 , 0 ) = 0.0f;
	//			        
 //   m_projMatrix( 0 , 1 ) = 0.0f;
 //   m_projMatrix( 1 , 1 ) = yScale;
 //   m_projMatrix( 2 , 1 ) = 0.0f;
 //   m_projMatrix( 3 , 1 ) = 0.0f;
	//			        
 //   m_projMatrix( 0 , 2 ) = 0.0f;
 //   m_projMatrix( 1 , 2 ) = 0.0f;
 //   m_projMatrix( 2 , 2 ) = zfar / ( zfar - znear );
 //   m_projMatrix( 3 , 2 ) = -znear * zfar / ( zfar - znear );
	//			        
 //   m_projMatrix( 0 , 3 ) = 0.0f;
 //   m_projMatrix( 1 , 3 ) = 0.0f;
 //   m_projMatrix( 2 , 3 ) = 1.0f;
 //   m_projMatrix( 3 , 3 ) = 0.0f;
}

//��]
void CTPCamera::Rotate( float longitudeDegrees , float latitudeDegrees , float hoge )
{
    m_latitudeDegrees = latitudeDegrees;
    m_longitudeDegrees = longitudeDegrees;
}

//�J�����X�V
void CTPCamera::UpdateCamera( const float elapsedTimeSec )
{
	//�o�ߎ��Ԃ��l�����ĉ�]������
    float latitudeElapsed = m_latitudeDegrees * m_rot_speed * elapsedTimeSec;
    float longitudeElapsed = m_longitudeDegrees * m_rot_speed * elapsedTimeSec;

	//��]
    D3DXQUATERNION rot;
    
	//���[���͖���
    D3DXQuaternionRotationYawPitchRoll( &rot , D3DXToRadian( longitudeElapsed ) , D3DXToRadian( latitudeElapsed ) , 0.0f );

	//���݂̃J�����̉�]�Ɋ�Â��ăI�t�Z�b�g�x�N�g������]������
	D3DXVECTOR3 new_eye = CCommon::QuaternionTransformCoord( m_offset , rot ) + m_target;

	//�r���[�s����č\��
    LookAt( new_eye , m_target , WORLD_YAXIS );
}
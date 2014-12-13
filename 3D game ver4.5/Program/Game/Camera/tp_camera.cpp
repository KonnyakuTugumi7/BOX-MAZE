#include "DXUT.h"
#include <cmath>
#include "tp_camera.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"

//�R���X�g���N�^
CTPCamera::CTPCamera()
{
	//CSV
	m_Csv = CCsv::Create( "Content/csv/TPCameraData.csv" );

	//�o�x
    m_LongitudeDegrees = 0.0f;

	//�ܓx
    m_LatitudeDegrees = 0.0f;

	//�J�����ƒ����_�̃I�t�Z�b�g
    m_Offset = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//��]���x
	m_RotSpeed = m_Csv->GetToken< float >( 5 , 0 );

	//�ˉe�ϊ�
	Perspective( m_Csv->GetToken< float >( 0 , 0 ) , 1.0f , m_Csv->GetToken< float >( 1 , 0 ) , m_Csv->GetToken< float >( 2 , 0 ) );

	//�����_����ړ�
	LookAt( D3DXVECTOR3( 0.0f , /*m_Csv->GetToken< float >( 3 , 0 )*/0 , m_Csv->GetToken< float >( 3 , 1 ) ) , D3DXVECTOR3( 0.0f , /*m_Csv->GetToken< float >( 4 , 0 )*/0 , 0 ) , D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );
}

//�����_
void CTPCamera::LookAt( const D3DXVECTOR3& target )
{
    m_Target = target;
}

//�����_����ړ�
void CTPCamera::LookAt( const D3DXVECTOR3& eye , const D3DXVECTOR3& target , const D3DXVECTOR3& up )
{
	//�J�������W�ƃ^�[�Q�b�g���Z�b�g
    m_Eye = eye;
    m_Target = target;

    m_ZAxis = target - eye;
    m_Offset.x = -m_ZAxis.x;
    m_Offset.y = -m_ZAxis.y;
    m_Offset.z = -m_ZAxis.z;
    D3DXVec3Normalize( &m_ZAxis , &m_ZAxis );

	//x�������̃x�N�g�������߂�
    D3DXVec3Cross( &m_XAxis, &up , &m_ZAxis );
    D3DXVec3Normalize( &m_XAxis , &m_XAxis );

	//y�������̃x�N�g�������߂�
    D3DXVec3Cross( &m_YAxis , &m_ZAxis , &m_XAxis );
    D3DXVec3Normalize( &m_YAxis , &m_YAxis );
    D3DXVec3Normalize( &m_XAxis , &m_XAxis );

    D3DXMatrixIdentity( &m_ViewMatrix );

	////�r���[�s��X�V
 //   m_ViewMatrix( 0 , 0 ) = m_XAxis.x;
 //   m_ViewMatrix( 1 , 0 ) = m_XAxis.y;
 //   m_ViewMatrix( 2 , 0 ) = m_XAxis.z;
 //   m_ViewMatrix( 3 , 0 ) = -D3DXVec3Dot( &m_XAxis , &eye );

 //   m_ViewMatrix( 0 , 1 ) = m_YAxis.x;
 //   m_ViewMatrix( 1 , 1 ) = m_YAxis.y;
 //   m_ViewMatrix( 2 , 1 ) = m_YAxis.z;
 //   m_ViewMatrix( 3 , 1 ) = -D3DXVec3Dot( &m_YAxis, &eye );

 //   m_ViewMatrix( 0 , 2 ) = m_ZAxis.x;
 //   m_ViewMatrix( 1 , 2 ) = m_ZAxis.y;
 //   m_ViewMatrix( 2 , 2 ) = m_ZAxis.z;
 //   m_ViewMatrix( 3 , 2 ) = -D3DXVec3Dot( &m_ZAxis , &eye );

	//���������ݒ�
	m_ViewDir = m_ZAxis;

	//�r���[�s��X�V
	D3DXMatrixLookAtLH( &m_ViewMatrix , &m_Eye , &m_Target , &up );

    D3DXQuaternionRotationMatrix( &m_Orientation , &m_ViewMatrix );
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
	D3DXMatrixPerspectiveFovLH( &m_ProjMatrix , fovy , aspect , znear , zfar );
	CGraphicsManager::m_pD3dDevice->SetTransform( D3DTS_PROJECTION , &m_ProjMatrix );

	////�ˉe�ϊ�
 //   m_ProjMatrix( 0 , 0 ) = xScale;
 //   m_ProjMatrix( 1 , 0 ) = 0.0f;
 //   m_ProjMatrix( 2 , 0 ) = 0.0f;
 //   m_ProjMatrix( 3 , 0 ) = 0.0f;
	//			        
 //   m_ProjMatrix( 0 , 1 ) = 0.0f;
 //   m_ProjMatrix( 1 , 1 ) = yScale;
 //   m_ProjMatrix( 2 , 1 ) = 0.0f;
 //   m_ProjMatrix( 3 , 1 ) = 0.0f;
	//			        
 //   m_ProjMatrix( 0 , 2 ) = 0.0f;
 //   m_ProjMatrix( 1 , 2 ) = 0.0f;
 //   m_ProjMatrix( 2 , 2 ) = zfar / ( zfar - znear );
 //   m_ProjMatrix( 3 , 2 ) = -znear * zfar / ( zfar - znear );
	//			        
 //   m_ProjMatrix( 0 , 3 ) = 0.0f;
 //   m_ProjMatrix( 1 , 3 ) = 0.0f;
 //   m_ProjMatrix( 2 , 3 ) = 1.0f;
 //   m_ProjMatrix( 3 , 3 ) = 0.0f;
}

//��]
void CTPCamera::Rotate( float longitudeDegrees , float latitudeDegrees , float hoge )
{
    m_LatitudeDegrees = latitudeDegrees;
    m_LongitudeDegrees = longitudeDegrees;
}

//�J�����X�V
void CTPCamera::UpdateCamera( const float elapsedTimeSec )
{
	//�o�ߎ��Ԃ��l�����ĉ�]������
    float latitudeElapsed = m_LatitudeDegrees * m_RotSpeed * elapsedTimeSec;
    float longitudeElapsed = m_LongitudeDegrees * m_RotSpeed * elapsedTimeSec;

	//��]
    D3DXQUATERNION rot;
    
	//���[���͖���
    D3DXQuaternionRotationYawPitchRoll( &rot , D3DXToRadian( longitudeElapsed ) , D3DXToRadian( latitudeElapsed ) , 0.0f );

	//���݂̃J�����̉�]�Ɋ�Â��ăI�t�Z�b�g�x�N�g������]������
	D3DXVECTOR3 new_eye = CCommon::QuaternionTransformCoord( m_Offset , rot ) + m_Target;

	//�r���[�s����č\��
    LookAt( new_eye , m_Target , WORLD_YAXIS );
}
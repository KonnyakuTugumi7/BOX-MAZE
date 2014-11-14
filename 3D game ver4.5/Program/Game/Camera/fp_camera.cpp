#include "DXUT.h"
#include <cmath>
#include "fp_camera.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"

//�R���X�g���N�^
CFPCamera::CFPCamera()
{
	//������
	Init();
}

//������
void CFPCamera::Init()
{
	//CSV
	m_csv = CCsv::Create( "Content/csv/FPCameraData.csv" );

	//Y���ړ���
	m_jump_movement = 0.0f;
    
	//�X���x����
    m_accumPitchDegrees = 0.0f;
	
	//��]���x
	m_rotationSpeed = m_csv->GetToken< float >( 0 , 0 );

	//�����ʒu
    SetPosition( 0 , m_csv->GetToken< float >( 1 , 0 ) , 0 );

	//X���W
	m_xAxis = D3DXVECTOR3( 1.0f , 0.0f , 0.0f );
    
	//Y���W
	m_yAxis = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );
    
	//Z���W
	m_zAxis = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
    
	//�������
	m_viewDir = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
    
	//����
	m_currentVelocity = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//�ˉe�ϊ�	 //��p(x)		//�A�X�y�N�g�� //�ߐڌ��E //���u���E
	Perspective( m_csv->GetToken< float >( 2 , 0 ) , 1.0f , m_csv->GetToken< float >( 3 , 0 ) , m_csv->GetToken< float >( 4 , 0 ) );

	//���������x
	m_acceleration = D3DXVECTOR3( m_csv->GetToken< float >( 5 , 0 ) , m_csv->GetToken< float >( 5 , 1 ) , m_csv->GetToken< float >( 5 , 2 ) );

	//�������x
	m_velocity = D3DXVECTOR3( m_csv->GetToken< float >( 6 , 0 ) , m_csv->GetToken< float >( 6 , 1 ) , m_csv->GetToken< float >( 6 , 2 ) );

	//�ő���E
	m_boundsmax.x = m_csv->GetToken< float >( 7 , 0 );
	m_boundsmax.y = m_csv->GetToken< float >( 7 , 1 );
	m_boundsmax.z = m_csv->GetToken< float >( 7 , 2 );

	//�ŏ����E
	m_boundsmin.x = m_csv->GetToken< float >( 8 , 0 );
	m_boundsmin.y = m_csv->GetToken< float >( 8 , 1 );
	m_boundsmin.z = m_csv->GetToken< float >( 8 , 2 );

	//����	
    D3DXQuaternionIdentity( &m_orientation );

	//�d�̓t���O
	m_is_gravity = true;
}

//�J�����X�V
void CFPCamera::UpdateCamera( const float elapsedTimeSec )
{
	//�O�̃t���[���̍��W��ۑ�
	m_prev_eye = m_eye;

	//����
    D3DXVECTOR3 direction;
	//�ړ�����
	GetMovementDirection( direction );

	//x���ŉ�]�������
	float pitch = MOUSE->yPosRelative() * GetRotationSpeed();
	//y���ŉ�]�������
	float heading = MOUSE->xPosRelative() * GetRotationSpeed();
	//��]
	Rotate( heading , pitch , 0.0f );

	//���W�X�V
	UpdatePosition( direction , elapsedTimeSec );

	//�����̐���
	PerformCameraCollisionDetection();

	//�n�ʐڒn�t���O���I�t��
	m_is_on_ground = false;
}

//�ړ�����
void CFPCamera::GetMovementDirection( D3DXVECTOR3& direction )
{
	//-------------�ړ��L�[�t���O-------------
	//�O
	static bool moveForwardsPressed = false;
	//���
	static bool moveBackwardsPressed = false;
	//�E
	static bool moveRightPressed = false;
	//��
	static bool moveLeftPressed = false;
	//��
	static bool moveUpPressed = false;
	//��
	static bool moveDownPressed = false;

	//������ۑ�
	D3DXVECTOR3 velocity = GetCurrentVelocity();

	//����������
	direction.x = direction.y = direction.z = 0.0f;

	//------------------------------------�ړ�------------------------------------
	//�O
	if( K_B->KeyDown( K_B->KEY_UP ) || K_B->KeyDown( K_B->KEY_W ) )
	{
		if( !moveForwardsPressed )
		{
			moveForwardsPressed = true;
			//z���ȊO�̗����ݒ�
			m_currentVelocity = D3DXVECTOR3( velocity.x , velocity.y , 0.0f );
		}

		//�O��(z��)
		direction.z += 1.0f;
	}
	else
	{
		moveForwardsPressed = false;
	}

	//���
	if( K_B->KeyDown( K_B->KEY_DOWN ) || K_B->KeyDown( K_B->KEY_S ) )
	{
		if( !moveBackwardsPressed )
		{
			moveBackwardsPressed = true;
			//z���ȊO�̗����ݒ�
			m_currentVelocity = D3DXVECTOR3( velocity.x , velocity.y , 0.0f );
		}

		//����(z��)
		direction.z -= 1.0f;
	}
	else
	{
		moveBackwardsPressed = false;
	}

	//�E
	if( K_B->KeyDown( K_B->KEY_RIGHT ) || K_B->KeyDown( K_B->KEY_D ) )
	{
		if( !moveRightPressed )
		{
			moveRightPressed = true;
			//x���ȊO�̗����ݒ�
			m_currentVelocity = D3DXVECTOR3( 0.0f , velocity.y , velocity.z );
		}

		//�E��(x��)
		direction.x += 1.0f;
	}
	else
	{
		moveRightPressed = false;
	}

	//��
	if( K_B->KeyDown( K_B->KEY_LEFT ) || K_B->KeyDown( K_B->KEY_A ) )
	{
		if(!moveLeftPressed)
		{
			moveLeftPressed = true;
			//x���ȊO�̗����ݒ�
			m_currentVelocity = D3DXVECTOR3( 0.0f , velocity.y , velocity.z );
		}

		//����(x��)
		direction.x -= 1.0f;
	}
	else
	{
		moveLeftPressed = false;
	}

	//�d�͂��������Ă����ԂȂ�ȉ��̏������s��Ȃ�
	if( m_is_gravity == true ) return;

	//��
	if( K_B->KeyDown( K_B->KEY_E ) )
	{
		if( !moveUpPressed )
		{
			moveUpPressed = true;
			//y���ȊO�̗����ݒ�
			m_currentVelocity = D3DXVECTOR3( velocity.x , 0.0f , velocity.z );
		}

		//���(y��)
		direction.y += 1.0f;
	}
	else
	{
		moveUpPressed = false;
	}

	//��
	if( K_B->KeyDown( K_B->KEY_Q ) )
	{
		if( !moveDownPressed )
		{
			moveDownPressed = true;
			//y���ȊO�̗����ݒ�
			m_currentVelocity = D3DXVECTOR3( velocity.x , 0.0f , velocity.z );
		}

		//����(y��)
		direction.y -= 1.0f;
	}
	else
	{
		moveDownPressed = false;
	}
}

//��]
void CFPCamera::Rotate( float headingDegrees , float pitchDegrees , float rollDegrees )
{
	//�}�E�X���i�񂾕��ɉ�]����悤�ɂ���
    pitchDegrees = -pitchDegrees;
    headingDegrees = -headingDegrees;

	//��l�̉�]
	RotateFirstPerson( headingDegrees , pitchDegrees );

    //�r���[�s��X�V
	UpdateViewMatrix();
}

//��l�̉�]
void CFPCamera::RotateFirstPerson( float headingDegrees , float pitchDegrees )
{
	//�X���x����
    m_accumPitchDegrees += pitchDegrees;

	//��]����
    if( m_accumPitchDegrees > 90.0f )
	{
        pitchDegrees = 90.0f - ( m_accumPitchDegrees - pitchDegrees );
        m_accumPitchDegrees = 90.0f;
    }

	//��]����
    if( m_accumPitchDegrees < -90.0f )
	{
        pitchDegrees = -90.0f - ( m_accumPitchDegrees - pitchDegrees );
        m_accumPitchDegrees = -90.0f;
    }

	//���W�A���֕ϊ�
    float heading = D3DXToRadian( headingDegrees );
    float pitch = D3DXToRadian( pitchDegrees );

    D3DXQUATERNION rot;

	//���[���hy������ɃJ��������]
    if( heading != 0.0f )
	{
        D3DXQuaternionRotationAxis( &rot , &WORLD_YAXIS , heading );
        D3DXQuaternionMultiply( &m_orientation , &rot , &m_orientation );
    }

    //���[�J��x���̎���ɃJ��������]
    if( pitch != 0.0f )
	{
		D3DXQuaternionRotationAxis( &rot , &D3DXVECTOR3( 1.0f , 0.0f , 0.0f ) , pitch );
        D3DXQuaternionMultiply( &m_orientation , &m_orientation , &rot );
    }
}

//�r���[�s��X�V
void CFPCamera::UpdateViewMatrix()
{
	//�N�H�[�^�j�I���̐��K��
    D3DXQuaternionNormalize( &m_orientation , &m_orientation );
	//�N�H�[�^�j�I�������]�s����쐬
    D3DXMatrixRotationQuaternion( &m_viewMatrix , &m_orientation );

    m_xAxis = D3DXVECTOR3( m_viewMatrix( 0 , 0 ) , m_viewMatrix( 1 , 0 ) , m_viewMatrix( 2 , 0 ) );
    m_yAxis = D3DXVECTOR3( m_viewMatrix( 0 , 1 ) , m_viewMatrix( 1 , 1 ) , m_viewMatrix( 2 , 1 ) );
    m_zAxis = D3DXVECTOR3( m_viewMatrix( 0 , 2 ) , m_viewMatrix( 1 , 2 ) , m_viewMatrix( 2 , 2 ) );
    m_viewDir = m_zAxis;

							   //2�̃x�N�g���̓���
    //m_viewMatrix( 3 , 0 ) = -D3DXVec3Dot( &m_xAxis , &m_eye );
    //m_viewMatrix( 3 , 1 ) = -D3DXVec3Dot( &m_yAxis , &m_eye );
    //m_viewMatrix( 3 , 2 ) = -D3DXVec3Dot( &m_zAxis , &m_eye );

	//�����_
	m_target = m_eye + m_viewDir;//D3DXVECTOR3( m_xAxis.x , m_yAxis.y , m_zAxis.z );

	//�r���[�s��쐬(�J�����̔z�u)
	D3DXMatrixLookAtLH( &m_viewMatrix , &m_eye , &m_target , &WORLD_YAXIS );
	
	//DirectX�ɐݒ�(�ϊ��𔽉f����)
	CCommon::m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_viewMatrix );
}

//�ʒu�X�V
void CFPCamera::UpdatePosition( const D3DXVECTOR3& direction , const float elapsedTimeSec )
{
    //�����x�N�g���͔͈͂�1����-1

	//�]�ڗ�
    D3DXVECTOR3 displacement( 0 , 0 , 0 );

	if( m_is_gravity == true )
	{
		//�W�����v
		Jump( elapsedTimeSec );
	}

	//������0�Ŗ������̂݃J�������ړ�����
    if( D3DXVec3LengthSq( &m_currentVelocity ) != 0.0f )
	//if( m_currentVelocity )
	{
		//�]�ڗ�
		displacement.x = m_currentVelocity.x * m_acceleration.x * elapsedTimeSec;
		displacement.y = m_currentVelocity.y * m_acceleration.y * elapsedTimeSec;
		displacement.z = m_currentVelocity.z * m_acceleration.z * elapsedTimeSec;

		//�J�������Ӑ}���Ȃ����������Ȃ��悤�ɖh��(�e���ɉ����Ĉړ�����)
		if( direction.x == 0.0f && fabsf( m_currentVelocity.x ) < 1e-6f ) displacement.x = 0.0f;

		if( direction.y == 0.0f && fabsf( m_currentVelocity.y ) < 1e-6f ) displacement.y = 0.0f;

        if( direction.z == 0.0f && fabsf( m_currentVelocity.z ) < 1e-6f ) displacement.z = 0.0f;

		Move( displacement.x , displacement.y , displacement.z );
    }

	//���x�X�V
	UpdateVelocity( direction , elapsedTimeSec );
}

//�W�����v
void CFPCamera::Jump( const float elapsedTimeSec )
{
	//�W�����v�J�n
	if( m_is_on_ground == true && K_B->KeyPressed( K_B->KEY_SPACE ) )
	{
		//�n�ʐڒn�t���O���I�t��
		m_is_on_ground = false;

		//�W�����v�͂�ݒ�
		m_jump_movement = 0.5f;
	}

	//�d��
	if( m_is_on_ground == false )
	{
		m_jump_movement -= 0.02f;
		//m_jump_movement -= elapsedTimeSec * 0.98f;
		m_eye.y += m_jump_movement;
	}

	//y�ړ��ʂ̐���
	if( m_jump_movement < -1.0f ) m_jump_movement = -1.0f;
}

//�J�����ړ������������g��
void CFPCamera::Move( const float dx , const float dy , const float dz )
{
	//�J�������W
	D3DXVECTOR3 eye = m_eye;

	//�O��
	D3DXVECTOR3 forwards;

	//�ړ��������v�Z
	//�J������X���ƃ��[���hY��(m_yAxis����Ȃ�)�̊O�ςŐi�ޕ���(�O��)�̃x�N�g�����o��
	D3DXVec3Cross( &forwards , &m_xAxis , &WORLD_YAXIS );

	//�x�N�g���̐��K��
	D3DXVec3Normalize( &forwards , &forwards );

	//dx�ɂ���č��E�ɃJ�������ړ�
	eye += m_xAxis * dx;

	//dy�ɂ���ď㉺�ɃJ�������ړ�
	eye += WORLD_YAXIS * dy;

	//dz�ɂ���đO��ɃJ�������ړ�
	eye += forwards * dz;

	//���W��ݒ�(m_eye�ƃr���[�}�g���b�N�X�X�V)
	SetPosition( eye );
}

//�J�����ړ�
void CFPCamera::Move( const D3DXVECTOR3& direction , const D3DXVECTOR3& amount )
{
	//���[���h���W�Ŏw�肳�ꂽ�����Ɏw�肳�ꂽ�ʂ����ړ�������
    m_eye.x += direction.x * amount.x;
    m_eye.y += direction.y * amount.y;
    m_eye.z += direction.z * amount.z;

	//�r���[�s����X�V
    UpdateViewMatrix();
}

//���x�X�V
void CFPCamera::UpdateVelocity( const D3DXVECTOR3& direction , float elapsedTimeSec )
{
	//�����͈̔͂�1����-1

	//x��
    if( direction.x != 0.0f )
	{
        m_currentVelocity.x += direction.x * m_acceleration.x * elapsedTimeSec;

		//����
        if( m_currentVelocity.x > m_velocity.x )
		{
			m_currentVelocity.x = m_velocity.x;
		}
		else if( m_currentVelocity.x < -m_velocity.x )
		{
            m_currentVelocity.x = -m_velocity.x;
		}
    }
	else
	{
        //����
        if( m_currentVelocity.x > 0.0f )
		{
            if( ( m_currentVelocity.x -= m_acceleration.x * elapsedTimeSec ) < 0.0f ) m_currentVelocity.x = 0.0f;
		}
		else
		{
            if( ( m_currentVelocity.x += m_acceleration.x * elapsedTimeSec ) > 0.0f ) m_currentVelocity.x = 0.0f;
        }
    }

	//y��
    if( direction.y != 0.0f )
	{
        m_currentVelocity.y += direction.y * m_acceleration.y * elapsedTimeSec;
        
		//����
        if( m_currentVelocity.y > m_velocity.y )
		{
			m_currentVelocity.y = m_velocity.y;
		}
		else if( m_currentVelocity.y < -m_velocity.y )
		{
            m_currentVelocity.y = -m_velocity.y;
		}
    }
	else
	{
        //����
        if( m_currentVelocity.y > 0.0f )
		{
            if( ( m_currentVelocity.y -= m_acceleration.y * elapsedTimeSec ) < 0.0f ) m_currentVelocity.y = 0.0f;
        }
		else
		{
            if( ( m_currentVelocity.y += m_acceleration.y * elapsedTimeSec ) > 0.0f ) m_currentVelocity.y = 0.0f;
        }
    }

	//z��
    if( direction.z != 0.0f )
	{
        m_currentVelocity.z += direction.z * m_acceleration.z * elapsedTimeSec * 2;

		//����
        if( m_currentVelocity.z > m_velocity.z )
		{
            m_currentVelocity.z = m_velocity.z;
		}
		else if( m_currentVelocity.z < -m_velocity.z )
		{
            m_currentVelocity.z = -m_velocity.z;
		}
    }
	else
	{
        //����
        if( m_currentVelocity.z > 0.0f )
		{
            if( ( m_currentVelocity.z -= m_acceleration.z * elapsedTimeSec ) < 0.0f ) m_currentVelocity.z = 0.0f;
        }
		else
		{
            if( ( m_currentVelocity.z += m_acceleration.z * elapsedTimeSec ) > 0.0f ) m_currentVelocity.z = 0.0f;
        }
    }
}

//�����̐���
void CFPCamera::PerformCameraCollisionDetection()
{
	//���݂̍��W
    const D3DXVECTOR3& pos = GetPosition();
	//�V�������W
    D3DXVECTOR3 newPos( pos );

	//------------���E�̒l�𒴂��Ȃ��悤�ɕ␳------------
    if( pos.x > m_boundsmax.x ) newPos.x = m_boundsmax.x;

    if( pos.x < m_boundsmin.x ) newPos.x = m_boundsmin.x;

    if( pos.y > m_boundsmax.y ) newPos.y = m_boundsmax.y;

    if( pos.y < m_boundsmin.y ) newPos.y = m_boundsmin.y;

    if( pos.z > m_boundsmax.z ) newPos.z = m_boundsmax.z;

    if( pos.z < m_boundsmin.z ) newPos.z = m_boundsmin.z;

	//���W��ݒ�
    SetPosition( newPos );
}

//���ߓ��e
void CFPCamera::Perspective( const float fovx , const float aspect , const float znear , const float zfar )
{
	//�ˉe�ϊ�(�J�����̃p�����[�^�ݒ�)			//����p //�A�X�y�N�g�� //�ŋߓ_ //�ŉ��_
	D3DXMatrixPerspectiveFovLH( &m_projMatrix , fovx , aspect , znear , zfar );
	
	//DirectX�ɐݒ�(�ϊ��𔽉f����)
	CGraphicsManager::m_pd3dDevice->SetTransform( D3DTS_PROJECTION , &m_projMatrix );
}
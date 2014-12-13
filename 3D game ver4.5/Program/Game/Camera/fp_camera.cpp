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
	m_Csv = CCsv::Create( "Content/csv/FPCameraData.csv" );

	//Y���ړ���
	m_JumpMovement = 0.0f;
    
	//�X���x����
    m_AccumPitchDegrees = 0.0f;
	
	//��]���x
	m_RotationSpeed = m_Csv->GetToken< float >( 0 , 0 );

	//�����ʒu
    SetPosition( 0 , m_Csv->GetToken< float >( 1 , 0 ) , 0 );

	//X���W
	m_XAxis = D3DXVECTOR3( 1.0f , 0.0f , 0.0f );
    
	//Y���W
	m_YAxis = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );
    
	//Z���W
	m_ZAxis = D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
    
	//�������
	m_ViewDir = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
    
	//����
	m_CurrentVelocity = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//�ˉe�ϊ�	 //��p(x)		//�A�X�y�N�g�� //�ߐڌ��E //���u���E
	Perspective( m_Csv->GetToken< float >( 2 , 0 ) , 1.0f , m_Csv->GetToken< float >( 3 , 0 ) , m_Csv->GetToken< float >( 4 , 0 ) );

	//���������x
	m_Acceleration = D3DXVECTOR3( m_Csv->GetToken< float >( 5 , 0 ) , m_Csv->GetToken< float >( 5 , 1 ) , m_Csv->GetToken< float >( 5 , 2 ) );

	//�������x
	m_Velocity = D3DXVECTOR3( m_Csv->GetToken< float >( 6 , 0 ) , m_Csv->GetToken< float >( 6 , 1 ) , m_Csv->GetToken< float >( 6 , 2 ) );

	//�ő���E
	m_BoundsMax.x = m_Csv->GetToken< float >( 7 , 0 );
	m_BoundsMax.y = m_Csv->GetToken< float >( 7 , 1 );
	m_BoundsMax.z = m_Csv->GetToken< float >( 7 , 2 );

	//�ŏ����E
	m_BoundsMin.x = m_Csv->GetToken< float >( 8 , 0 );
	m_BoundsMin.y = m_Csv->GetToken< float >( 8 , 1 );
	m_BoundsMin.z = m_Csv->GetToken< float >( 8 , 2 );

	//����	
    D3DXQuaternionIdentity( &m_Orientation );

	//�d�̓t���O
	m_IsGravity = true;
}

//�J�����X�V
void CFPCamera::UpdateCamera( const float elapsedTimeSec )
{
	//�O�̃t���[���̍��W��ۑ�
	m_PrevEye = m_Eye;

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
	m_IsOnGround = false;
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
			m_CurrentVelocity = D3DXVECTOR3( velocity.x , velocity.y , 0.0f );
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
			m_CurrentVelocity = D3DXVECTOR3( velocity.x , velocity.y , 0.0f );
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
			m_CurrentVelocity = D3DXVECTOR3( 0.0f , velocity.y , velocity.z );
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
			m_CurrentVelocity = D3DXVECTOR3( 0.0f , velocity.y , velocity.z );
		}

		//����(x��)
		direction.x -= 1.0f;
	}
	else
	{
		moveLeftPressed = false;
	}

	//�d�͂��������Ă����ԂȂ�ȉ��̏������s��Ȃ�
	if( m_IsGravity == true ) return;

	//��
	if( K_B->KeyDown( K_B->KEY_E ) )
	{
		if( !moveUpPressed )
		{
			moveUpPressed = true;
			//y���ȊO�̗����ݒ�
			m_CurrentVelocity = D3DXVECTOR3( velocity.x , 0.0f , velocity.z );
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
			m_CurrentVelocity = D3DXVECTOR3( velocity.x , 0.0f , velocity.z );
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
    m_AccumPitchDegrees += pitchDegrees;

	//��]����
    if( m_AccumPitchDegrees > 90.0f )
	{
        pitchDegrees = 90.0f - ( m_AccumPitchDegrees - pitchDegrees );
        m_AccumPitchDegrees = 90.0f;
    }

	//��]����
    if( m_AccumPitchDegrees < -90.0f )
	{
        pitchDegrees = -90.0f - ( m_AccumPitchDegrees - pitchDegrees );
        m_AccumPitchDegrees = -90.0f;
    }

	//���W�A���֕ϊ�
    float heading = D3DXToRadian( headingDegrees );
    float pitch = D3DXToRadian( pitchDegrees );

    D3DXQUATERNION rot;

	//���[���hy������ɃJ��������]
    if( heading != 0.0f )
	{
        D3DXQuaternionRotationAxis( &rot , &WORLD_YAXIS , heading );
        D3DXQuaternionMultiply( &m_Orientation , &rot , &m_Orientation );
    }

    //���[�J��x���̎���ɃJ��������]
    if( pitch != 0.0f )
	{
		D3DXQuaternionRotationAxis( &rot , &D3DXVECTOR3( 1.0f , 0.0f , 0.0f ) , pitch );
        D3DXQuaternionMultiply( &m_Orientation , &m_Orientation , &rot );
    }
}

//�r���[�s��X�V
void CFPCamera::UpdateViewMatrix()
{
	//�N�H�[�^�j�I���̐��K��
    D3DXQuaternionNormalize( &m_Orientation , &m_Orientation );
	//�N�H�[�^�j�I�������]�s����쐬
    D3DXMatrixRotationQuaternion( &m_ViewMatrix , &m_Orientation );

    m_XAxis = D3DXVECTOR3( m_ViewMatrix( 0 , 0 ) , m_ViewMatrix( 1 , 0 ) , m_ViewMatrix( 2 , 0 ) );
    m_YAxis = D3DXVECTOR3( m_ViewMatrix( 0 , 1 ) , m_ViewMatrix( 1 , 1 ) , m_ViewMatrix( 2 , 1 ) );
    m_ZAxis = D3DXVECTOR3( m_ViewMatrix( 0 , 2 ) , m_ViewMatrix( 1 , 2 ) , m_ViewMatrix( 2 , 2 ) );
    m_ViewDir = m_ZAxis;

							   //2�̃x�N�g���̓���
    //m_ViewMatrix( 3 , 0 ) = -D3DXVec3Dot( &m_XAxis , &m_Eye );
    //m_ViewMatrix( 3 , 1 ) = -D3DXVec3Dot( &m_YAxis , &m_Eye );
    //m_ViewMatrix( 3 , 2 ) = -D3DXVec3Dot( &m_ZAxis , &m_Eye );

	//�����_
	m_Target = m_Eye + m_ViewDir;//D3DXVECTOR3( m_XAxis.x , m_YAxis.y , m_ZAxis.z );

	//�r���[�s��쐬(�J�����̔z�u)
	D3DXMatrixLookAtLH( &m_ViewMatrix , &m_Eye , &m_Target , &WORLD_YAXIS );
	
	//DirectX�ɐݒ�(�ϊ��𔽉f����)
	CGraphicsManager::m_pD3dDevice->SetTransform( D3DTS_VIEW, &m_ViewMatrix );
}

//�ʒu�X�V
void CFPCamera::UpdatePosition( const D3DXVECTOR3& direction , const float elapsedTimeSec )
{
    //�����x�N�g���͔͈͂�1����-1

	//�]�ڗ�
    D3DXVECTOR3 displacement( 0 , 0 , 0 );

	if( m_IsGravity == true )
	{
		//�W�����v
		Jump( elapsedTimeSec );
	}

	//������0�Ŗ������̂݃J�������ړ�����
    if( D3DXVec3LengthSq( &m_CurrentVelocity ) != 0.0f )
	//if( m_CurrentVelocity )
	{
		//�]�ڗ�
		displacement.x = m_CurrentVelocity.x * m_Acceleration.x * elapsedTimeSec;
		displacement.y = m_CurrentVelocity.y * m_Acceleration.y * elapsedTimeSec;
		displacement.z = m_CurrentVelocity.z * m_Acceleration.z * elapsedTimeSec;

		//�J�������Ӑ}���Ȃ����������Ȃ��悤�ɖh��(�e���ɉ����Ĉړ�����)
		if( direction.x == 0.0f && fabsf( m_CurrentVelocity.x ) < 1e-6f ) displacement.x = 0.0f;

		if( direction.y == 0.0f && fabsf( m_CurrentVelocity.y ) < 1e-6f ) displacement.y = 0.0f;

        if( direction.z == 0.0f && fabsf( m_CurrentVelocity.z ) < 1e-6f ) displacement.z = 0.0f;

		Move( displacement.x , displacement.y , displacement.z );
    }

	//���x�X�V
	UpdateVelocity( direction , elapsedTimeSec );
}

//�W�����v
void CFPCamera::Jump( const float elapsedTimeSec )
{
	//�W�����v�J�n
	if( m_IsOnGround == true && K_B->KeyPressed( K_B->KEY_SPACE ) )
	{
		//�n�ʐڒn�t���O���I�t��
		m_IsOnGround = false;

		//�W�����v�͂�ݒ�
		m_JumpMovement = 0.5f;
	}

	//�d��
	if( m_IsOnGround == false )
	{
		m_JumpMovement -= 0.02f;
		//m_JumpMovement -= elapsedTimeSec * 0.98f;
		m_Eye.y += m_JumpMovement;
	}

	//y�ړ��ʂ̐���
	if( m_JumpMovement < -1.0f ) m_JumpMovement = -1.0f;
}

//�J�����ړ������������g��
void CFPCamera::Move( const float dx , const float dy , const float dz )
{
	//�J�������W
	D3DXVECTOR3 eye = m_Eye;

	//�O��
	D3DXVECTOR3 forwards;

	//�ړ��������v�Z
	//�J������X���ƃ��[���hY��(m_YAxis����Ȃ�)�̊O�ςŐi�ޕ���(�O��)�̃x�N�g�����o��
	D3DXVec3Cross( &forwards , &m_XAxis , &WORLD_YAXIS );

	//�x�N�g���̐��K��
	D3DXVec3Normalize( &forwards , &forwards );

	//dx�ɂ���č��E�ɃJ�������ړ�
	eye += m_XAxis * dx;

	//dy�ɂ���ď㉺�ɃJ�������ړ�
	eye += WORLD_YAXIS * dy;

	//dz�ɂ���đO��ɃJ�������ړ�
	eye += forwards * dz;

	//���W��ݒ�(m_Eye�ƃr���[�}�g���b�N�X�X�V)
	SetPosition( eye );
}

//�J�����ړ�
void CFPCamera::Move( const D3DXVECTOR3& direction , const D3DXVECTOR3& amount )
{
	//���[���h���W�Ŏw�肳�ꂽ�����Ɏw�肳�ꂽ�ʂ����ړ�������
    m_Eye.x += direction.x * amount.x;
    m_Eye.y += direction.y * amount.y;
    m_Eye.z += direction.z * amount.z;

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
        m_CurrentVelocity.x += direction.x * m_Acceleration.x * elapsedTimeSec;

		//����
        if( m_CurrentVelocity.x > m_Velocity.x )
		{
			m_CurrentVelocity.x = m_Velocity.x;
		}
		else if( m_CurrentVelocity.x < -m_Velocity.x )
		{
            m_CurrentVelocity.x = -m_Velocity.x;
		}
    }
	else
	{
        //����
        if( m_CurrentVelocity.x > 0.0f )
		{
            if( ( m_CurrentVelocity.x -= m_Acceleration.x * elapsedTimeSec ) < 0.0f ) m_CurrentVelocity.x = 0.0f;
		}
		else
		{
            if( ( m_CurrentVelocity.x += m_Acceleration.x * elapsedTimeSec ) > 0.0f ) m_CurrentVelocity.x = 0.0f;
        }
    }

	//y��
    if( direction.y != 0.0f )
	{
        m_CurrentVelocity.y += direction.y * m_Acceleration.y * elapsedTimeSec;
        
		//����
        if( m_CurrentVelocity.y > m_Velocity.y )
		{
			m_CurrentVelocity.y = m_Velocity.y;
		}
		else if( m_CurrentVelocity.y < -m_Velocity.y )
		{
            m_CurrentVelocity.y = -m_Velocity.y;
		}
    }
	else
	{
        //����
        if( m_CurrentVelocity.y > 0.0f )
		{
            if( ( m_CurrentVelocity.y -= m_Acceleration.y * elapsedTimeSec ) < 0.0f ) m_CurrentVelocity.y = 0.0f;
        }
		else
		{
            if( ( m_CurrentVelocity.y += m_Acceleration.y * elapsedTimeSec ) > 0.0f ) m_CurrentVelocity.y = 0.0f;
        }
    }

	//z��
    if( direction.z != 0.0f )
	{
        m_CurrentVelocity.z += direction.z * m_Acceleration.z * elapsedTimeSec * 2;

		//����
        if( m_CurrentVelocity.z > m_Velocity.z )
		{
            m_CurrentVelocity.z = m_Velocity.z;
		}
		else if( m_CurrentVelocity.z < -m_Velocity.z )
		{
            m_CurrentVelocity.z = -m_Velocity.z;
		}
    }
	else
	{
        //����
        if( m_CurrentVelocity.z > 0.0f )
		{
            if( ( m_CurrentVelocity.z -= m_Acceleration.z * elapsedTimeSec ) < 0.0f ) m_CurrentVelocity.z = 0.0f;
        }
		else
		{
            if( ( m_CurrentVelocity.z += m_Acceleration.z * elapsedTimeSec ) > 0.0f ) m_CurrentVelocity.z = 0.0f;
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
    if( pos.x > m_BoundsMax.x ) newPos.x = m_BoundsMax.x;

    if( pos.x < m_BoundsMin.x ) newPos.x = m_BoundsMin.x;

    if( pos.y > m_BoundsMax.y ) newPos.y = m_BoundsMax.y;

    if( pos.y < m_BoundsMin.y ) newPos.y = m_BoundsMin.y;

    if( pos.z > m_BoundsMax.z ) newPos.z = m_BoundsMax.z;

    if( pos.z < m_BoundsMin.z ) newPos.z = m_BoundsMin.z;

	//���W��ݒ�
    SetPosition( newPos );
}

//���ߓ��e
void CFPCamera::Perspective( const float fovx , const float aspect , const float znear , const float zfar )
{
	//�ˉe�ϊ�(�J�����̃p�����[�^�ݒ�)			//����p //�A�X�y�N�g�� //�ŋߓ_ //�ŉ��_
	D3DXMatrixPerspectiveFovLH( &m_ProjMatrix , fovx , aspect , znear , zfar );
	
	//DirectX�ɐݒ�(�ϊ��𔽉f����)
	CGraphicsManager::m_pD3dDevice->SetTransform( D3DTS_PROJECTION , &m_ProjMatrix );
}
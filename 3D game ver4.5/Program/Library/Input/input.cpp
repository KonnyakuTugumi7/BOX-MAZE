#include "DXUT.h"
#include "input.h"

//-----------------------------------------------------------------------------
// �L�[�{�[�h
//-----------------------------------------------------------------------------

//�C���X�^���X�쐬&�擾
CKeyboard* CKeyboard::GetInstance()
{
	static CKeyboard instance;

	return &instance;
}

//�R���X�g���N�^
CKeyboard::CKeyboard()
{
    m_pDirectInput = 0;
    m_pDevice = 0;
	m_is_active = true;
    m_pCurrKeyStates = m_keyStates[ 0 ];
    m_pPrevKeyStates = m_keyStates[ 1 ];
    m_lastChar = 0;
	Create();
}

//�f�X�g���N�^
CKeyboard::~CKeyboard()
{
	//���
    Destroy();
}

//����
bool CKeyboard::Create()
{
    HINSTANCE hInstance = GetModuleHandle( 0 );
	//���݃��[�U�[����Ƃ��Ă���E�B���h�E�̃n���h����Ԃ�
    HWND hWnd = GetForegroundWindow();
	
	//�f�o�C�X�I�u�W�F�N�g���쐬
    if( FAILED( DirectInput8Create( hInstance , DIRECTINPUT_VERSION , IID_IDirectInput8 , reinterpret_cast< void** >( &m_pDirectInput ) , 0 ) ) ) return false;

	//�f�o�C�X���쐬
	if( FAILED( m_pDirectInput->CreateDevice( GUID_SysKeyboard , &m_pDevice , 0 ) ) ) return false;

	//�f�[�^�`����ݒ�
    if( FAILED( m_pDevice->SetDataFormat( &c_dfDIKeyboard ) ) ) return false;

	//�������[�h�̐ݒ�(�t�H�A�O���E���h&��r�����[�h)
    if( FAILED( m_pDevice->SetCooperativeLevel( hWnd , DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) ) return false;

	//���͐���J�n(�f�o�C�X�ւ̃A�N�Z�X�����擾)
    if( FAILED( m_pDevice->Acquire() ) ) return false;

	//�L�[�X�e�[�^�X�̏�����
    memset( m_keyStates , 0 , sizeof( m_keyStates ) );
	
	return true;
}

//���
void CKeyboard::Destroy()
{
    if( m_pDevice )
	{
        m_pDevice->Unacquire();
        m_pDevice->Release();
        m_pDevice = 0;
    }

    if( m_pDirectInput )
	{
        m_pDirectInput->Release();
        m_pDirectInput = 0;
    }
}

//�n���h�����b�Z�[�W
void CKeyboard::HandleMsg( const HWND hWnd , const UINT msg , const WPARAM wParam , const LPARAM lParam )
{
    switch ( msg )
	{
    case WM_CHAR:
        m_lastChar = static_cast< char >( wParam );
        break;

    default:
        break;
    }
}

//�X�V
void CKeyboard::Update()
{
    if( !m_pDirectInput || !m_pDevice )
	{
        if( !Create() )
		{
            Destroy();
            return;
        }
    }

	//�L�[�X�e�[�^�X�̍X�V
    HRESULT hr = 0;
    unsigned char *pTemp = m_pPrevKeyStates;
    m_pPrevKeyStates = m_pCurrKeyStates;
    m_pCurrKeyStates = pTemp;
    
	//�f�o�C�X���X�g�΍�
    while ( true )
	{
		//�f�[�^���擾(256�͎擾����f�[�^�̃T�C�Y)
        hr = m_pDevice->GetDeviceState( 256 , m_pCurrKeyStates );
        
        if( FAILED( hr ) )
		{
			//���̓f�o�C�X�����X�g������
            if( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED )
			{
				//���͐���J�n(�f�o�C�X�ւ̃A�N�Z�X�����擾)
                if( FAILED( m_pDevice->Acquire() ) ) return;
            }
        }
		else
		{
            break;
        }
    }
}

//-----------------------------------------------------------------------------
// �}�E�X
//-----------------------------------------------------------------------------

//----------CMouse�N���X�̐ÓI�ϐ��̎���----------

const float CMouse::WEIGHT_MODIFIER = 0.2f;
const int CMouse::HISTORY_BUFFER_SIZE = 10;

//------------------------------------------------

//�C���X�^���X�쐬&�擾
CMouse* CMouse::GetInstance()
{
	static CMouse instance;

	return &instance;
}

//�R���X�g���N�^
CMouse::CMouse()
{
    m_pDevice = 0;
    m_pCurrMouseState = &m_mouseStates[ 0 ];
    m_pPrevMouseState = &m_mouseStates[ 1 ];

	m_is_active = true;
    
    m_deltaMouseX = 0.0f;
    m_deltaMouseY = 0.0f;
    m_deltaMouseWheel = 0.0f;
    
    m_weightModifier = WEIGHT_MODIFIER;
    m_enableFiltering = true;

    m_historyBufferSize = HISTORY_BUFFER_SIZE;
    m_historyBuffer.resize( m_historyBufferSize );

	Create();

	RECT carsor_limit;
	//�E�B���h�E�����擾
	GetWindowRect( GetForegroundWindow() , &carsor_limit );
	carsor_limit.left += 10;
	carsor_limit.right -= 10;
	carsor_limit.top += 30;
	carsor_limit.bottom -= 10;
	//�J�[�\���̈ړ��͈͂�����
	ClipCursor( &carsor_limit );
}

//�f�X�g���N�^
CMouse::~CMouse()
{
	//���
    Destroy();
}

//����
bool CMouse::Create()
{
    HINSTANCE hInstance = GetModuleHandle( 0 );
	//���݃��[�U�[����Ƃ��Ă���E�B���h�E�̃n���h����Ԃ�
    HWND hWnd = GetForegroundWindow();

	//�f�o�C�X�I�u�W�F�N�g���쐬
    if( FAILED( DirectInput8Create( hInstance , DIRECTINPUT_VERSION , IID_IDirectInput8 , reinterpret_cast< void** >( &m_pDirectInput ) , 0 ) ) ) return false;

	//�f�o�C�X���쐬
    if( FAILED( m_pDirectInput->CreateDevice( GUID_SysMouse , &m_pDevice , 0 ) ) ) return false;

	//�f�[�^�`����ݒ�
    if( FAILED( m_pDevice->SetDataFormat( &c_dfDIMouse ) ) ) return false;

	//�������[�h�̐ݒ�(�t�H�A�O���E���h&��r�����[�h)
    if( FAILED( m_pDevice->SetCooperativeLevel( hWnd , DISCL_FOREGROUND | DISCL_EXCLUSIVE ) ) ) return false;//DISCL_NONEXCLUSIVE

	//���͐���J�n(�f�o�C�X�ւ̃A�N�Z�X�����擾)
    if( FAILED( m_pDevice->Acquire() ) ) return false;

	//�}�E�X�X�e�[�^�X��������
    memset( m_mouseStates , 0 , sizeof( m_mouseStates ) );
    memset( &m_historyBuffer[ 0 ] , 0 , m_historyBuffer.size() );

    m_mouseIndex = 0;
    m_mouseMovement[ 0 ].first = m_mouseMovement[ 0 ].second = 0.0f;
    m_mouseMovement[ 1 ].first = m_mouseMovement[ 2 ].second = 0.0f;

    return true;
}

//���
void CMouse::Destroy()
{
    if( m_pDevice )
	{
        m_pDevice->Unacquire();
        m_pDevice->Release();
        m_pDevice = 0;
    }

    if( m_pDirectInput )
	{
        m_pDirectInput->Release();
        m_pDirectInput = 0;
    }
}

//�X�V
void CMouse::Update()
{
    if( !m_pDirectInput || !m_pDevice )
	{
        if( !Create() )
		{
            Destroy();
            return;
        }
    }

	//�A�N�e�B�u�łȂ��Ȃ�ȉ��̏������s��Ȃ�
	if( m_is_active == false ) return;

	//�}�E�X�X�e�[�^�X�̍X�V
    HRESULT hr = 0;
    DIMOUSESTATE *pTemp = m_pPrevMouseState;
    m_pPrevMouseState = m_pCurrMouseState;
    m_pCurrMouseState = pTemp;

	//RECT carsor_limit;
	////�E�B���h�E�����擾
	//GetWindowRect( GetForegroundWindow() , &carsor_limit );
	//carsor_limit.left = 1;
	////�J�[�\���̈ړ��͈͂�����
	//ClipCursor( &carsor_limit );

	//�}�E�X�J�[�\���̍��W
	POINT point;
	GetCursorPos( &point );
	ScreenToClient( GetForegroundWindow() , &point );
	float mouth_x = ( float )point.x;
	float mouth_y = ( float )point.y;
	m_pos.x = mouth_x;
	m_pos.y = mouth_y;

	//�f�o�C�X���X�g�΍�
    while( true )
	{
		//�f�[�^���擾(256�͎擾����f�[�^�̃T�C�Y)
        hr = m_pDevice->GetDeviceState( sizeof( DIMOUSESTATE ) , m_pCurrMouseState );

        if( FAILED( hr ) )
		{
			//���̓f�o�C�X�����X�g������
            if( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED )
			{
				//���͐���J�n(�f�o�C�X�ւ̃A�N�Z�X�����擾)
                if( FAILED( m_pDevice->Acquire() ) ) return;
            }
        }
		else
		{
            break;
        }
    }

    if( m_enableFiltering )
	{
        PerformMouseFiltering( static_cast< float >( m_pCurrMouseState->lX ) , static_cast< float >( m_pCurrMouseState->lY ) );

        PerformMouseSmoothing( m_deltaMouseX , m_deltaMouseY );
    }
	else
	{
        m_deltaMouseX = static_cast< float >( m_pCurrMouseState->lX );
        m_deltaMouseY = static_cast< float >( m_pCurrMouseState->lY );
    }

	//�O�Ƀz�C�[��
    if( m_pCurrMouseState->lZ > 0 )
	{
		m_deltaMouseWheel = 1.0f;
	}
	//���Ƀz�C�[��
    else if( m_pCurrMouseState->lZ < 0 ) 
	{
		m_deltaMouseWheel = -1.0f;
	}
	//�z�C�[�����Ă��Ȃ�
	else
	{
		m_deltaMouseWheel = 0.0f;
	}
}

//�k���k������
void CMouse::PerformMouseSmoothing( const float x , const float y )
{
    m_mouseMovement[ m_mouseIndex ].first = x;
    m_mouseMovement[ m_mouseIndex ].second = y;

    m_deltaMouseX = ( m_mouseMovement[ 0 ].first + m_mouseMovement[ 1 ].first ) * 0.5f;
    m_deltaMouseY = ( m_mouseMovement[ 0 ].second + m_mouseMovement[ 1 ].second ) * 0.5f;

    m_mouseIndex ^= 1;
    m_mouseMovement[ m_mouseIndex ].first = 0.0f;
    m_mouseMovement[ m_mouseIndex ].second = 0.0f;
}

//����
void CMouse::PerformMouseFiltering( const float x , const float y )
{
    //�O�̃t���[������̃}�E�X�̓����Ɋ�Â��đ��ΓI�ȃ}�E�X�̓����𐧌�

    for( int i = m_historyBufferSize - 1 ; i > 0 ; --i )
	{
        m_historyBuffer[ i ].first = m_historyBuffer[ i - 1 ].first;
        m_historyBuffer[ i ].second = m_historyBuffer[ i - 1 ].second;
    }

    m_historyBuffer[ 0 ].first = x;
    m_historyBuffer[ 0 ].second = y;

    float averageX = 0.0f;
    float averageY = 0.0f;
    float averageTotal = 0.0f;
    float currentWeight = 1.0f;

    for( int i = 0 ; i < m_historyBufferSize ; ++i )
	{
        averageX += m_historyBuffer[ i ].first * currentWeight;
        averageY += m_historyBuffer[ i ].second * currentWeight;
        averageTotal += 1.0f * currentWeight;
        currentWeight *= m_weightModifier;
    }

    m_deltaMouseX = averageX / averageTotal;
    m_deltaMouseY = averageY / averageTotal;
}

//�}�E�X�J�[�\���̕\���A��\��
bool CMouse::CursorAppearance( const bool flg )
{
	Destroy();

	Create();

	//���݃��[�U�[����Ƃ��Ă���E�B���h�E�̃n���h����Ԃ�
	HWND hWnd = GetForegroundWindow();

	if( flg == true )
	{
		//�������[�h�̐ݒ�(�t�H�A�O���E���h&��r�����[�h)
		if( FAILED( m_pDevice->SetCooperativeLevel( hWnd , DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) ) return false;
	}
	else
	{
		//�������[�h�̐ݒ�(�t�H�A�O���E���h&�r�����[�h)
		if( FAILED( m_pDevice->SetCooperativeLevel( hWnd , DISCL_FOREGROUND | DISCL_EXCLUSIVE ) ) ) return false;
	}

	return true;
}
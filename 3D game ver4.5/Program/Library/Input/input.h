#pragma once

#define INITGUID
#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <dinput.h>
#include <utility>
#include <vector>
#include <memory>

#pragma comment( lib , "dinput8.lib" )

#pragma warning ( disable : 4127 ) //���������萔�ł��B

//�C���X�^���X���쐬&�擾(�����̂�define�Œ�`)
#define K_B CKeyboard::GetInstance()

//�L�[�{�[�h�N���X
class CKeyboard{
private:
	//�f�o�C�X
	IDirectInput8* m_pDirectInput;

	//�f�o�C�X
    IDirectInputDevice8* m_pDevice;

	//�A�N�e�B�u�t���O
	bool m_is_active;

	//���݂̃L�[�̏��
    unsigned char* m_pCurrKeyStates;

	//1�O�̃L�[�̏��
    unsigned char* m_pPrevKeyStates;

	//�L�[�̏��
    unsigned char m_keyStates[ 2 ][ 256 ];

	//�L�[��Ԃ̍Ō�
    char m_lastChar;

public:
	//�L�[���
    enum Key
    {
        KEY_0               = 0x0B,
        KEY_1               = 0x02,
        KEY_2               = 0x03,
        KEY_3               = 0x04,
        KEY_4               = 0x05,
        KEY_5               = 0x06,
        KEY_6               = 0x07,
        KEY_7               = 0x08,
        KEY_8               = 0x09,
        KEY_9               = 0x0A,

        KEY_A               = 0x1E,
        KEY_B               = 0x30,
        KEY_C               = 0x2E,
        KEY_D               = 0x20,
        KEY_E               = 0x12,
        KEY_F               = 0x21,
        KEY_G               = 0x22,
        KEY_H               = 0x23,
        KEY_I               = 0x17,
        KEY_J               = 0x24,
        KEY_K               = 0x25,
        KEY_L               = 0x26,
        KEY_M               = 0x32,
        KEY_N               = 0x31,
        KEY_O               = 0x18,
        KEY_P               = 0x19,
        KEY_Q               = 0x10,
        KEY_R               = 0x13,
        KEY_S               = 0x1F,
        KEY_T               = 0x14,
        KEY_U               = 0x16,
        KEY_V               = 0x2F,
        KEY_W               = 0x11,
        KEY_X               = 0x2D,
        KEY_Y               = 0x15,
        KEY_Z               = 0x2C,
        
        KEY_F1              = 0x3B,
        KEY_F2              = 0x3C,
        KEY_F3              = 0x3D,
        KEY_F4              = 0x3E,
        KEY_F5              = 0x3F,
        KEY_F6              = 0x40,
        KEY_F7              = 0x41,
        KEY_F8              = 0x42,
        KEY_F9              = 0x43,
        KEY_F10             = 0x44,
        KEY_F11             = 0x57,
        KEY_F12             = 0x58,

        KEY_NUMPAD_0        = 0x52,
        KEY_NUMPAD_1        = 0x4F,
        KEY_NUMPAD_2        = 0x50,
        KEY_NUMPAD_3        = 0x51,
        KEY_NUMPAD_4        = 0x4B,
        KEY_NUMPAD_5        = 0x4C,
        KEY_NUMPAD_6        = 0x4D,
        KEY_NUMPAD_7        = 0x47,
        KEY_NUMPAD_8        = 0x48,
        KEY_NUMPAD_9        = 0x49,
        KEY_NUMPAD_ADD      = 0x4E,
        KEY_NUMPAD_DIVIDE   = 0xB5,
        KEY_NUMPAD_ENTER    = 0x9C,
        KEY_NUMPAD_MINUS    = 0x4A,
        KEY_NUMPAD_MULTIPLY = 0x37,
        KEY_NUMPAD_NUMLOCK  = 0x45,
        KEY_NUMPAD_PERIOD   = 0x53,

        KEY_ADD             = 0x0D,
        KEY_APOSTROPHE      = 0x28,
        KEY_APPS            = 0xDD,
        KEY_BACKSLASH       = 0x2B,
        KEY_BACKSPACE       = 0x0E,
        KEY_CAPSLOCK        = 0x3A,
        KEY_COMMA           = 0x33,
        KEY_ENTER           = 0x1C,
        KEY_ESCAPE          = 0x01,
        KEY_MINUS           = 0x0C,
        KEY_PAUSE           = 0xC5,
        KEY_PERIOD          = 0x34,
        KEY_PRINTSCREEN     = 0xB7,
        KEY_SCROLLLOCK      = 0x46,
        KEY_SEMICOLON       = 0x27,
        KEY_SLASH           = 0x35,
        KEY_SPACE           = 0x39,
        KEY_TAB             = 0x0F,
        KEY_TILDE           = 0x29,

        KEY_LALT            = 0x38,
        KEY_LBRACKET        = 0x1A,
        KEY_LCONTROL        = 0x1D,
        KEY_LSHIFT          = 0x2A,
        KEY_LWIN            = 0xDB,
        KEY_RALT            = 0xB8,
        KEY_RBRACKET        = 0x1B,
        KEY_RCONTROL        = 0x9D,
        KEY_RSHIFT          = 0x36,
        KEY_RWIN            = 0xDC,

        KEY_DOWN            = 0xD0,
        KEY_LEFT            = 0xCB,
        KEY_RIGHT           = 0xCD,
        KEY_UP              = 0xC8,

        KEY_DELETE          = 0xD3,
        KEY_END             = 0xCF,
        KEY_HOME            = 0xC7,
        KEY_INSERT          = 0xD2,
        KEY_PAGEDOWN        = 0xD1,
        KEY_PAGEUP          = 0xC9,

        KEY_CALCULATOR      = 0xA1,
        KEY_MAIL            = 0xEC,
        KEY_MEDIASELECT     = 0xED,
        KEY_MEDIASTOP       = 0xA4,
        KEY_MUTE            = 0xA0,
        KEY_MYCOMPUTER      = 0xEB,
        KEY_NEXTTRACK       = 0x99,
        KEY_PLAYPAUSE       = 0xA2,
        KEY_POWER           = 0xDE,
        KEY_PREVTRACK       = 0x90,
        KEY_SLEEP           = 0xDF,
        KEY_VOLUMEDOWN      = 0xAE,
        KEY_VOLUMEUP        = 0xB0,
        KEY_WAKE            = 0xE3,
        KEY_WEBBACK         = 0xEA,
        KEY_WEBFAVORITES    = 0xE6,
        KEY_WEBFORWARD      = 0xE9,
        KEY_WEBHOME         = 0xB2,
        KEY_WEBREFRESH      = 0xE7,
        KEY_WEBSEARCH       = 0xE5,
        KEY_WEBSTOP         = 0xE8
    };

private:
	//�R���X�g���N�^
    CKeyboard();

	//�f�X�g���N�^
    ~CKeyboard();

	//�R�s�[�R���X�g���N�^
	CKeyboard( const CKeyboard& obj );

	// = ���Z�q�̃I�[�o�[���[�h
	CKeyboard operator = ( const CKeyboard& obj );

	//����
    bool Create();

	//�j��
    void Destroy();

public:
	//�C���X�^���X
    static CKeyboard* GetInstance();

	//�n���h�����b�Z�[�W
    void HandleMsg( const HWND hWnd , const UINT msg , const WPARAM wParam , const LPARAM lParam );

	//�X�V
    void Update();

	//�ŏ�ʂP�r�b�g�Ɂu������Ă��邩�v�ǂ����̃f�[�^���i�[����Ă��邱�Ƃɒ���  
    //�ŉ��ʂP�r�b�g�ɂ́u�g�O���i�I���E�I�t�̏�ԁj�v���i�[����Ă���  

	//�w�肵���L�[�������������ǂ������擾
    inline bool KeyDown( const Key key ) const 
	{ 
		if( m_is_active == false ) return false;
		return ( m_pCurrKeyStates[ key ] & 0x80 ) ? true : false; 
	}

	//�w�肵���L�[���オ���Ă��邩�ǂ������擾
    inline bool KeyUp( const Key key ) const
	{
		if( m_is_active == false ) return true;
		return ( m_pCurrKeyStates[ key ] & 0x80 ) ? false : true; 
	}

	//�w�肵���L�[��������Ă��邩�ǂ������擾
    inline bool KeyPressed( const Key key ) const
	{
		if( m_is_active == false ) return false;
		return ( ( m_pCurrKeyStates[ key ] & 0x80 ) && !( m_pPrevKeyStates[ key ] & 0x80 ) ) ? true : false; 
	}

	//�ŏI������擾
    inline char GetLastChar() const { return m_lastChar; }

	//�A�N�e�B�u�t���O�Z�b�^�[
	inline void SetIsActive( const bool active ){ m_is_active = active; }

};

class CMouse;
typedef std::shared_ptr< CMouse > MouseSP;

//�C���X�^���X���쐬&�擾(�����̂�define�Œ�`)
#define MOUSE CMouse::GetInstance()

//�}�E�X�N���X
class CMouse{
private:
	//�o�b�t�@�T�C�Y
    static const int HISTORY_BUFFER_SIZE;

	//�ύX�q
	static const float WEIGHT_MODIFIER;

	//�f�o�C�X
    IDirectInput8* m_pDirectInput;

	//�f�o�C�X
    IDirectInputDevice8* m_pDevice;

	//���݂̃}�E�X�̏��
    DIMOUSESTATE* m_pCurrMouseState;

	//��O�̂̃}�E�X�̏��
    DIMOUSESTATE* m_pPrevMouseState;

	//�}�E�X�̏��
    DIMOUSESTATE m_mouseStates[ 2 ];

	//�A�N�e�B�u�t���O
	bool m_is_active;

	bool m_enableFiltering;

	int m_historyBufferSize;

    int m_mouseIndex;

	//X��(���ΓI��)
    float m_deltaMouseX;

	//Y��(���ΓI��)
    float m_deltaMouseY;

	//Z��(�z�C�[��)
    float m_deltaMouseWheel;

    float m_weightModifier;

	//�}�E�X�J�[�\�����W
	D3DXVECTOR2 m_pos;

    std::vector< std::pair< float , float > > m_historyBuffer;

    std::pair< float , float > m_mouseMovement[ 2 ];

public:
	//�}�E�X���
    enum MouseButton
    {
        BUTTON_LEFT   = 0x00,
        BUTTON_RIGHT  = 0x01,
        BUTTON_MIDDLE = 0x02
    };

private:
	//�R���X�g���N�^
    CMouse();

	//�f�X�g���N�^
    ~CMouse();

	//�R�s�[�R���X�g���N�^
	CMouse( const CMouse& obj );

	// = ���Z�q�̃I�[�o�[���[�h
	CMouse operator = ( const CMouse& obj );

	//����
    bool Create();

	//�j��
    void Destroy();

public:
	//�C���X�^���X
    static CMouse* GetInstance();

	//�X�V
	void Update();

	//�k���k������
    void PerformMouseSmoothing( const float x , const float y );

	//����
    void PerformMouseFiltering( const float x , const float y );

	//�}�E�X�J�[�\���̕\���A��\��
	bool CursorAppearance( const bool flg );

	//�ŏ�ʂP�r�b�g�Ɂu������Ă��邩�v�ǂ����̃f�[�^���i�[����Ă��邱�Ƃɒ���
    //�ŉ��ʂP�r�b�g�ɂ́u�g�O���i�I���E�I�t�̏�ԁj�v���i�[����Ă���  
    
	//�{�^�����������Ă��邩
    inline bool ButtonDown( const MouseButton button ) const 
	{
		if( m_is_active == false ) return false;
		return ( m_pCurrMouseState->rgbButtons[ button ] & 0x80 ) ? true : false;
	}

	//�{�^����������Ă��邩(�O�񗣂��Ă��Č��݉����Ă���)
    inline bool ButtonPressed( const MouseButton button ) const
	{
		if( m_is_active == false ) return false;
		return ( ( m_pCurrMouseState->rgbButtons[ button ] & 0x80 ) && !( m_pPrevMouseState->rgbButtons[ button ] & 0x80 ) ) ? true : false; 
	}

	//�{�^�����オ���Ă��邩
    inline bool ButtonUp( const MouseButton button ) const
	{
		if( m_is_active == false ) return true;
		return ( m_pCurrMouseState->rgbButtons[ button ] & 0x80) ? false : true; 
	}

	inline void DisplayCursor( const bool display )
	{ 
		if( display == true )
		{
			//�\��
			while(1)
			{
				if( ShowCursor( true ) > 0 ) break;
			}
		}
		else
		{
			//��\��
			while(1)
			{
				if( ShowCursor( false ) < 0 ) break;
			}
		}
	}


	//-------------------------�Q�b�^�[-------------------------

    inline bool IsMouseSmoothing() const { return m_enableFiltering; }

    inline float xPosRelative() const { return m_deltaMouseX; }

    inline float yPosRelative() const { return m_deltaMouseY; }

    inline float WeightModifier() const { return m_weightModifier; }

    inline float WheelPos() const { return m_deltaMouseWheel; }

	inline D3DXVECTOR2 GetPos() const { return m_pos; }

	//--------------------------------------�Z�b�^�[--------------------------------------

	//����Z�b�g
    inline void SetWeightModifier( const float weightModifier ){ m_weightModifier = weightModifier; }
    
	//�k���k������
	inline void SmoothMouse( const bool smooth ){ m_enableFiltering = smooth; }

	//�A�N�e�B�u�t���O�Z�b�^�[
	inline void SetIsActive( const bool active ){ m_is_active = active; }
};

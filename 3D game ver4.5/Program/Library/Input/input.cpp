#include "DXUT.h"
#include "input.h"

//-----------------------------------------------------------------------------
// キーボード
//-----------------------------------------------------------------------------

//インスタンス作成&取得
CKeyboard* CKeyboard::GetInstance()
{
	static CKeyboard instance;

	return &instance;
}

//コンストラクタ
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

//デストラクタ
CKeyboard::~CKeyboard()
{
	//解放
    Destroy();
}

//生成
bool CKeyboard::Create()
{
    HINSTANCE hInstance = GetModuleHandle( 0 );
	//現在ユーザーが作業しているウィンドウのハンドルを返す
    HWND hWnd = GetForegroundWindow();
	
	//デバイスオブジェクトを作成
    if( FAILED( DirectInput8Create( hInstance , DIRECTINPUT_VERSION , IID_IDirectInput8 , reinterpret_cast< void** >( &m_pDirectInput ) , 0 ) ) ) return false;

	//デバイスを作成
	if( FAILED( m_pDirectInput->CreateDevice( GUID_SysKeyboard , &m_pDevice , 0 ) ) ) return false;

	//データ形式を設定
    if( FAILED( m_pDevice->SetDataFormat( &c_dfDIKeyboard ) ) ) return false;

	//協調モードの設定(フォアグラウンド&非排他モード)
    if( FAILED( m_pDevice->SetCooperativeLevel( hWnd , DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) ) return false;

	//入力制御開始(デバイスへのアクセス権を取得)
    if( FAILED( m_pDevice->Acquire() ) ) return false;

	//キーステータスの初期化
    memset( m_keyStates , 0 , sizeof( m_keyStates ) );
	
	return true;
}

//解放
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

//ハンドルメッセージ
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

//更新
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

	//キーステータスの更新
    HRESULT hr = 0;
    unsigned char *pTemp = m_pPrevKeyStates;
    m_pPrevKeyStates = m_pCurrKeyStates;
    m_pCurrKeyStates = pTemp;
    
	//デバイスロスト対策
    while ( true )
	{
		//データを取得(256は取得するデータのサイズ)
        hr = m_pDevice->GetDeviceState( 256 , m_pCurrKeyStates );
        
        if( FAILED( hr ) )
		{
			//入力デバイスがロストした時
            if( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED )
			{
				//入力制御開始(デバイスへのアクセス権を取得)
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
// マウス
//-----------------------------------------------------------------------------

//----------CMouseクラスの静的変数の実態----------

const float CMouse::WEIGHT_MODIFIER = 0.2f;
const int CMouse::HISTORY_BUFFER_SIZE = 10;

//------------------------------------------------

//インスタンス作成&取得
CMouse* CMouse::GetInstance()
{
	static CMouse instance;

	return &instance;
}

//コンストラクタ
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
	//ウィンドウ情報を取得
	GetWindowRect( GetForegroundWindow() , &carsor_limit );
	carsor_limit.left += 10;
	carsor_limit.right -= 10;
	carsor_limit.top += 30;
	carsor_limit.bottom -= 10;
	//カーソルの移動範囲を限定
	ClipCursor( &carsor_limit );
}

//デストラクタ
CMouse::~CMouse()
{
	//解放
    Destroy();
}

//生成
bool CMouse::Create()
{
    HINSTANCE hInstance = GetModuleHandle( 0 );
	//現在ユーザーが作業しているウィンドウのハンドルを返す
    HWND hWnd = GetForegroundWindow();

	//デバイスオブジェクトを作成
    if( FAILED( DirectInput8Create( hInstance , DIRECTINPUT_VERSION , IID_IDirectInput8 , reinterpret_cast< void** >( &m_pDirectInput ) , 0 ) ) ) return false;

	//デバイスを作成
    if( FAILED( m_pDirectInput->CreateDevice( GUID_SysMouse , &m_pDevice , 0 ) ) ) return false;

	//データ形式を設定
    if( FAILED( m_pDevice->SetDataFormat( &c_dfDIMouse ) ) ) return false;

	//協調モードの設定(フォアグラウンド&非排他モード)
    if( FAILED( m_pDevice->SetCooperativeLevel( hWnd , DISCL_FOREGROUND | DISCL_EXCLUSIVE ) ) ) return false;//DISCL_NONEXCLUSIVE

	//入力制御開始(デバイスへのアクセス権を取得)
    if( FAILED( m_pDevice->Acquire() ) ) return false;

	//マウスステータスを初期化
    memset( m_mouseStates , 0 , sizeof( m_mouseStates ) );
    memset( &m_historyBuffer[ 0 ] , 0 , m_historyBuffer.size() );

    m_mouseIndex = 0;
    m_mouseMovement[ 0 ].first = m_mouseMovement[ 0 ].second = 0.0f;
    m_mouseMovement[ 1 ].first = m_mouseMovement[ 2 ].second = 0.0f;

    return true;
}

//解放
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

//更新
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

	//アクティブでないなら以下の処理を行わない
	if( m_is_active == false ) return;

	//マウスステータスの更新
    HRESULT hr = 0;
    DIMOUSESTATE *pTemp = m_pPrevMouseState;
    m_pPrevMouseState = m_pCurrMouseState;
    m_pCurrMouseState = pTemp;

	//RECT carsor_limit;
	////ウィンドウ情報を取得
	//GetWindowRect( GetForegroundWindow() , &carsor_limit );
	//carsor_limit.left = 1;
	////カーソルの移動範囲を限定
	//ClipCursor( &carsor_limit );

	//マウスカーソルの座標
	POINT point;
	GetCursorPos( &point );
	ScreenToClient( GetForegroundWindow() , &point );
	float mouth_x = ( float )point.x;
	float mouth_y = ( float )point.y;
	m_pos.x = mouth_x;
	m_pos.y = mouth_y;

	//デバイスロスト対策
    while( true )
	{
		//データを取得(256は取得するデータのサイズ)
        hr = m_pDevice->GetDeviceState( sizeof( DIMOUSESTATE ) , m_pCurrMouseState );

        if( FAILED( hr ) )
		{
			//入力デバイスがロストした時
            if( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED )
			{
				//入力制御開始(デバイスへのアクセス権を取得)
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

	//前にホイール
    if( m_pCurrMouseState->lZ > 0 )
	{
		m_deltaMouseWheel = 1.0f;
	}
	//後ろにホイール
    else if( m_pCurrMouseState->lZ < 0 ) 
	{
		m_deltaMouseWheel = -1.0f;
	}
	//ホイールしていない
	else
	{
		m_deltaMouseWheel = 0.0f;
	}
}

//ヌルヌル動作
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

//制御
void CMouse::PerformMouseFiltering( const float x , const float y )
{
    //前のフレームからのマウスの動きに基づいて相対的なマウスの動きを制御

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

//マウスカーソルの表示、非表示
bool CMouse::CursorAppearance( const bool flg )
{
	Destroy();

	Create();

	//現在ユーザーが作業しているウィンドウのハンドルを返す
	HWND hWnd = GetForegroundWindow();

	if( flg == true )
	{
		//協調モードの設定(フォアグラウンド&非排他モード)
		if( FAILED( m_pDevice->SetCooperativeLevel( hWnd , DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) ) return false;
	}
	else
	{
		//協調モードの設定(フォアグラウンド&排他モード)
		if( FAILED( m_pDevice->SetCooperativeLevel( hWnd , DISCL_FOREGROUND | DISCL_EXCLUSIVE ) ) ) return false;
	}

	return true;
}
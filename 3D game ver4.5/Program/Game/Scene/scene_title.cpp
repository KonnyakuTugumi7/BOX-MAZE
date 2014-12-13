#include "DXUT.h"
#include "scene_title.h"
#include "../../Library/Support/common.h"
#include "../../Library/Input/input.h"
#include "../../Library/Sound/sound.h"
#include "../../Library/Sound/sound_manager.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../GameEngine/Scene/scene_manager.h"
#include "../../GameEngine/Camera/camera_base.h"
#include "../../Game/Scene/scene_play.h"
#include "../../Game/Scene/scene_edit.h"
#include "../../Game/Camera/tp_camera.h"
#include "../../Game/Camera/view_camera.h"

//初期化
void CSceneTitle::SceneInit()
{
	//BGMロード
	CSoundManager::Load( "Content/sound/title.wav" );
	CSoundManager::Load( "Content/sound/cursor.wav" );
	CSoundManager::Load( "Content/sound/fade.wav" );
	//サウンドID
	m_SoundId = 0;

	//CSV
	m_Csv = CCsv::Create( "Content/csv/TitleData.csv" );

	//カメラ
	m_Camera = CViewCamera::Create();

	//------------------------------画像------------------------------

	//背景
	m_Back = CBoard::Create( CGraphicsManager::m_WindowWidth , CGraphicsManager::m_WindowHeight , "Content/graphics/title.jpg" , CGraphicsManager::BACK_2D );

	//シーンナンバー
	m_SceneNum = SCENE_NUM::PLAY;

	//マウスカーソルを表示
	//MOUSE->DisplayCursor( false ); //コメントアウトしないとカメラの動きがおかしくなる;;

	//ステージ選択画面かどうか
	m_IsStageSelect = false;

	for( int i = 0 ; i < m_Csv->GetNumLine() ; ++i )
	{
		WCHAR wc_buff[ 255 ] = { 0 };
		CCommon::DXconvAnsiToWide( wc_buff , m_Csv->GetToken< std::string >( i , 0 ).c_str() , 255 );
		m_Logo.push_back( wc_buff );
	}
	m_Csv = CCsv::Create( "Content/csv/TitleSelectData.csv" );
	for( int i = 0 ; i < m_Csv->GetNumLine() ; ++i )
	{
		WCHAR wc_buff[ 255 ] = { 0 };
		CCommon::DXconvAnsiToWide( wc_buff , m_Csv->GetToken< std::string >( i , 0 ).c_str() , 255 );
		m_SelectLogo.push_back( wc_buff );
	}

	//フォントを生成
	m_Font = CFont::Create();
	if (m_Font)
	{
		m_Font->SetSize( 80.0f , 50.0f );
		m_Font->SetAlign( CFont::CENTER );
		for( int i = 0 ; i < m_Logo.size() ; ++i ) m_Font->CreateTexture( 128 , 512 , ( i == 0 ) ? FontType::MS_GOTHIC : FontType::MS_MINCHOU , m_Logo[ i ] );
		for( int i = 0 ; i < m_SelectLogo.size() ; ++i ) m_Font->CreateTexture( 128 , 512 , ( i == 0 ) ? FontType::MS_GOTHIC : FontType::MS_MINCHOU , m_SelectLogo[ i ] );
	}
}

//解放処理
void CSceneTitle::SceneDelete()
{
	//解放
	m_Back.reset();
	m_Font.reset();
}

//ロジック処理
void CSceneTitle::SceneFrameMove( const float elapsed_time )
{
	//--------------------------------------選択移動--------------------------------------
	if( K_B->KeyPressed( K_B->KEY_UP ) )
	{
		CSoundManager::Play( "Content/sound/cursor.wav" , CSoundManager::MODE_ONCE );
		m_SceneNum -= 1;
	}
		
	if( K_B->KeyPressed( K_B->KEY_DOWN ) )
	{
		CSoundManager::Play( "Content/sound/cursor.wav" , CSoundManager::MODE_ONCE );
		m_SceneNum += 1;
	}

	//メニュー画面ごとの処理
	if( m_IsStageSelect == false )
	{
		TitleInput();
	}
	else
	{
		StageSelectInput();
	}
}

//描画処理
void CSceneTitle::SceneFrameRender( const float elapsed_time )
{
	HRESULT hr;

	//画面のクリア																	  //画面の色
	V( CGraphicsManager::m_pD3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 ) );

	//描画
	CGraphicsManager::SysRender( m_Camera );

	D3DXVECTOR4 color(Color::WHITE, 1.0f);

	//メニュー画面ごとの処理
	if( m_IsStageSelect == false )
	{
		for( int i = 0 ; i < m_Logo.size() ; ++i )
		{
			( m_SceneNum == i - 1 ) ? color = D3DXVECTOR4( Color::YELLOW , 1.0f ) : color = D3DXVECTOR4( Color::RED , 1.0f );
			( i == 0 ) ? m_Font->SetSize( 100.0f , 70.0f ) : m_Font->SetSize( 80.0f , 50.0f );
			m_Font->Draw( m_Camera , m_Logo[ i ] , D3DXVECTOR2( 400 , ( i * 120 ) + 100 ) , color );
		}
	}
	else
	{
		for( int i = 0 ; i < m_SelectLogo.size() ; ++i )
		{
			( m_SceneNum == i - 1 ) ? color = D3DXVECTOR4( Color::YELLOW , 1.0f ) : color = D3DXVECTOR4( Color::RED , 1.0f );
			( i == 0 ) ? m_Font->SetSize( 100.0f , 70.0f ) : m_Font->SetSize( 80.0f , 50.0f );
			m_Font->Draw( m_Camera , m_SelectLogo[ i ] , D3DXVECTOR2( 400 , ( i * 120 ) + 100 ) , color );
		}
	}
}

//サウンド再生
void CSceneTitle::ScenePlaySound()
{
	if( m_SoundId == 0 )
	{
		m_SoundId = CSoundManager::Play( "Content/sound/title.wav" , CSoundManager::MODE_LOOP );
	}
	else
	{
		CSoundManager::Play( m_SoundId );
	}
}

//サウンド停止
void CSceneTitle::SceneStopSound()
{
	CSoundManager::Stop( m_SoundId );
}

//プレイヤーの入力
void CSceneTitle::TitleInput()
{
	//選択位置が限界まで達したら戻す
	if( m_SceneNum <= SCENE_NUM::SCENE_MIN ) m_SceneNum = SCENE_NUM::SCENE_MAX + SCENE_NUM::SCENE_MIN;	
	if( m_SceneNum >= SCENE_NUM::SCENE_MAX ) m_SceneNum = SCENE_NUM::PLAY;
	
	//-------------------------------------------------実行(シーン移動)-------------------------------------------------
	//インスタンス作成
	//プレイへ
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_SceneNum == SCENE_NUM::PLAY )
	{
		CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
		CSceneManager::GetInstance()->SetNextScene( &CScenePlay::CreateScene );
	}
	//ステージ選択へ
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_SceneNum == SCENE_NUM::EDIT )
	{
		CSoundManager::Play( "Content/sound/cursor.wav" , CSoundManager::MODE_ONCE );
		m_IsStageSelect = true;
		m_SceneNum = STAGE_SELECT_NUM::STAGE01;
		//CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
		//CSceneManager::GetInstance()->SetNextScene( &CSceneEdit::CreateScene );
	}
	//終了
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_SceneNum == SCENE_NUM::EXIT )
	{
		PostQuitMessage( WM_QUIT );
	}
}

//プレイヤーの入力
void CSceneTitle::StageSelectInput()
{
	//選択位置が限界まで達したら戻す
	if( m_SceneNum <= STAGE_SELECT_NUM::STAGE_MIN ) m_SceneNum = STAGE_SELECT_NUM::STAGE_MAX + STAGE_SELECT_NUM::STAGE_MIN;
	if( m_SceneNum >= STAGE_SELECT_NUM::STAGE_MAX ) m_SceneNum = STAGE_SELECT_NUM::STAGE01;
	
	//-------------------------------------------------実行(シーン移動)-------------------------------------------------
	//インスタンス作成
	if( K_B->KeyPressed( K_B->KEY_ENTER ) )
	{
		if( m_SceneNum == STAGE_SELECT_NUM::TITLE )
		{
			m_SceneNum = SCENE_NUM::PLAY;
			m_IsStageSelect = false;
		}
		else
		{
			m_Csv = CCsv::Create( "Content/csv/EditData.csv" );
			m_Csv->EraseToken( 2 , 0 );
			std::stringstream ss;
			ss << "Content/csv/CubeManager0" << m_SceneNum + 1 << ".csv";
			m_Csv->SetToken( 2 , 0 , ss.str() );
			m_Csv->ExportFile();
			CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
			CSceneManager::GetInstance()->SetNextScene( &CSceneEdit::CreateScene );
		}
	}
}
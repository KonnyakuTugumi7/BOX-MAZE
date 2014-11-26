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
	m_sound_id = 0;

	//CSV
	m_csv = CCsv::Create( "Content/csv/TitleData.csv" );

	//カメラ
	m_camera = CViewCamera::Create();

	//------------------------------画像------------------------------

	//背景
	//m_back = CSprite::Create( "Content/graphics/title.jpg" , CGraphicsManager::BACK_2D );
	//m_back->m_pos = D3DXVECTOR3( m_csv->GetToken< int >( 0 , 0 ) , m_csv->GetToken< int >( 0 , 1 ) , 0 );
	m_board = CBoard::Create( CGraphicsManager::m_window_width , CGraphicsManager::m_window_height , "Content/graphics/title.jpg" , CGraphicsManager::BACK_2D );

	//シーンナンバー
	m_scene_num = SCENE_NUM::PLAY;

	//マウスカーソルを表示
	//MOUSE->DisplayCursor( false ); //コメントアウトしないとカメラの動きがおかしくなる;;

	//ステージ選択画面かどうか
	m_is_stage_select = false;

	for( int i = 0 ; i < m_csv->GetNumLine() ; ++i )
	{
		WCHAR wc_buff[ 255 ] = { 0 };
		CCommon::DXconvAnsiToWide( wc_buff , m_csv->GetToken< std::string >( i , 0 ).c_str() , 255 );
		m_logo.push_back( wc_buff );
	}
	m_csv = CCsv::Create( "Content/csv/TitleSelectData.csv" );
	for( int i = 0 ; i < m_csv->GetNumLine() ; ++i )
	{
		WCHAR wc_buff[ 255 ] = { 0 };
		CCommon::DXconvAnsiToWide( wc_buff , m_csv->GetToken< std::string >( i , 0 ).c_str() , 255 );
		m_select_logo.push_back( wc_buff );
	}

	//フォントを生成
	m_font = CFont::Create();
	if (m_font)
	{
		m_font->SetSize( 80.0f , 50.0f );
		m_font->SetAlign( CFont::CENTER );
		for( int i = 0 ; i < m_logo.size() ; ++i ) m_font->CreateTexture( 128 , 512 , ( i == 0 ) ? FontType::MS_GOTHIC : FontType::MS_MINCHOU , m_logo[ i ] );
		for( int i = 0 ; i < m_select_logo.size() ; ++i ) m_font->CreateTexture( 128 , 512 , ( i == 0 ) ? FontType::MS_GOTHIC : FontType::MS_MINCHOU , m_select_logo[ i ] );
	}
}

//解放処理
void CSceneTitle::SceneDelete()
{
	//解放
	m_back.reset();
	m_board.reset();
	m_font.reset();
}

//ロジック処理
void CSceneTitle::SceneFrameMove( const float elapsed_time )
{
	//--------------------------------------選択移動--------------------------------------
	if( K_B->KeyPressed( K_B->KEY_UP ) )
	{
		CSoundManager::Play( "Content/sound/cursor.wav" , CSoundManager::MODE_ONCE );
		m_scene_num -= 1;
	}
		
	if( K_B->KeyPressed( K_B->KEY_DOWN ) )
	{
		CSoundManager::Play( "Content/sound/cursor.wav" , CSoundManager::MODE_ONCE );
		m_scene_num += 1;
	}

	//メニュー画面ごとの処理
	if( m_is_stage_select == false )
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
	V( CGraphicsManager::m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 ) );

	//描画
	CGraphicsManager::SysRender( m_camera );

	D3DXVECTOR4 color(Color::WHITE, 1.0f);

	//メニュー画面ごとの処理
	if( m_is_stage_select == false )
	{
		for( int i = 0 ; i < m_logo.size() ; ++i )
		{
			( m_scene_num == i - 1 ) ? color = D3DXVECTOR4( Color::YELLOW , 1.0f ) : color = D3DXVECTOR4( Color::RED , 1.0f );
			( i == 0 ) ? m_font->SetSize( 100.0f , 70.0f ) : m_font->SetSize( 80.0f , 50.0f );
			m_font->Draw( m_camera , m_logo[ i ] , D3DXVECTOR2( 400 , ( i * 120 ) + 100 ) , color );
		}
	}
	else
	{
		for( int i = 0 ; i < m_select_logo.size() ; ++i )
		{
			( m_scene_num == i - 1 ) ? color = D3DXVECTOR4( Color::YELLOW , 1.0f ) : color = D3DXVECTOR4( Color::RED , 1.0f );
			( i == 0 ) ? m_font->SetSize( 100.0f , 70.0f ) : m_font->SetSize( 80.0f , 50.0f );
			m_font->Draw( m_camera , m_select_logo[ i ] , D3DXVECTOR2( 400 , ( i * 120 ) + 100 ) , color );
		}
	}
}

//サウンド再生
void CSceneTitle::ScenePlaySound()
{
	if( m_sound_id == 0 )
	{
		m_sound_id = CSoundManager::Play( "Content/sound/title.wav" , CSoundManager::MODE_LOOP );
	}
	else
	{
		CSoundManager::Play( m_sound_id );
	}
}

//サウンド停止
void CSceneTitle::SceneStopSound()
{
	CSoundManager::Stop( m_sound_id );
}

//プレイヤーの入力
void CSceneTitle::TitleInput()
{
	//選択位置が限界まで達したら戻す
	if( m_scene_num <= SCENE_NUM::SCENE_MIN ) m_scene_num = SCENE_NUM::SCENE_MAX + SCENE_NUM::SCENE_MIN;	
	if( m_scene_num >= SCENE_NUM::SCENE_MAX ) m_scene_num = SCENE_NUM::PLAY;
	
	//-------------------------------------------------実行(シーン移動)-------------------------------------------------
	//インスタンス作成
	//プレイへ
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_scene_num == SCENE_NUM::PLAY )
	{
		CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
		CSceneManager::GetInstance()->SetNextScene( &CScenePlay::CreateScene );
	}
	//ステージ選択へ
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_scene_num == SCENE_NUM::EDIT )
	{
		CSoundManager::Play( "Content/sound/cursor.wav" , CSoundManager::MODE_ONCE );
		m_is_stage_select = true;
		m_scene_num = STAGE_SELECT_NUM::STAGE01;
		//CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
		//CSceneManager::GetInstance()->SetNextScene( &CSceneEdit::CreateScene );
	}
	//終了
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_scene_num == SCENE_NUM::EXIT )
	{
		PostQuitMessage( WM_QUIT );
	}
}

//プレイヤーの入力
void CSceneTitle::StageSelectInput()
{
	//選択位置が限界まで達したら戻す
	if( m_scene_num <= STAGE_SELECT_NUM::STAGE_MIN ) m_scene_num = STAGE_SELECT_NUM::STAGE_MAX + STAGE_SELECT_NUM::STAGE_MIN;
	if( m_scene_num >= STAGE_SELECT_NUM::STAGE_MAX ) m_scene_num = STAGE_SELECT_NUM::STAGE01;
	
	//-------------------------------------------------実行(シーン移動)-------------------------------------------------
	//インスタンス作成
	if( K_B->KeyPressed( K_B->KEY_ENTER ) )
	{
		if( m_scene_num == STAGE_SELECT_NUM::TITLE )
		{
			m_scene_num = SCENE_NUM::PLAY;
			m_is_stage_select = false;
		}
		else
		{
			m_csv = CCsv::Create( "Content/csv/EditData.csv" );
			m_csv->EraseToken( 2 , 0 );
			std::stringstream ss;
			ss << "Content/csv/CubeManager0" << m_scene_num + 1 << ".csv";
			m_csv->SetToken( 2 , 0 , ss.str() );
			m_csv->ExportFile();
			CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
			CSceneManager::GetInstance()->SetNextScene( &CSceneEdit::CreateScene );
		}
	}
}
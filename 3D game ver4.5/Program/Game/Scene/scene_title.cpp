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
	m_camera = CTPCamera::Create();

	//------------------------------画像------------------------------

	//背景
	m_back = CSprite::Create( "Content/graphics/title.jpg" , CGraphicsManager::BACK_2D );
	m_back->m_pos = D3DXVECTOR3( m_csv->GetToken< int >( 0 , 0 ) , m_csv->GetToken< int >( 0 , 1 ) , 0 );

	//シーンナンバー
	m_scene_num = SCENE_NUM::PLAY;

	//マウスカーソルを表示
	//MOUSE->DisplayCursor( false ); //コメントアウトしないとカメラの動きがおかしくなる;;

	//ステージ選択画面かどうか
	m_is_stage_select = false;

	//m_logo[ 0 ] = CBillboardString::Create( CGraphicsManager::m_pd3dDevice );
	//m_logo.push_back( CBillboardString::Create( CGraphicsManager::m_pd3dDevice ) );
	//m_logo[ 0 ]->SetFont( FACE_NAME::MS_MINCHOU.c_str() );
	//m_logo[ 0 ]->SetMaxPixelSize( 150 );
	for( int i = 0 ; i < 4 ; ++i )
	{
		m_logo.push_back( CBillboardString::Create( CGraphicsManager::m_pd3dDevice ) );
		m_logo[ i ]->SetFont( FACE_NAME::MS_MINCHOU.c_str() );
		m_logo[ i ]->SetMaxPixelSize( ( i == 0 ) ? 150 : 64 );
	}
}

//解放処理
void CSceneTitle::SceneDelete()
{
	//解放
	m_back.reset();
	for( int i = 0 ; i < m_logo.size() ; ++i ) m_logo[ i ].reset();
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
		m_logo[ 0 ]->RegistString( _T( "BOX MAZE" ) );
		m_logo[ 1 ]->RegistString( _T( "START" ) );
		m_logo[ 2 ]->RegistString( _T( "EDIT" ) );
		m_logo[ 3 ]->RegistString( _T( "EXIT" ) );
		m_logo[ 0 ]->SetColor( 0xFFFF0000 );
		m_logo[ 0 ]->SetPosition( 100 , 50 );
	}
	else
	{
		StageSelectInput();
		m_logo[ 0 ]->RegistString( _T( "BOX MAZE" ) );
		m_logo[ 1 ]->RegistString( _T( "STAGE01" ) );
		m_logo[ 2 ]->RegistString( _T( "STAGE02" ) );
		m_logo[ 3 ]->RegistString( _T( "TITLE" ) );
		m_logo[ 0 ]->SetColor( 0xFFFF0000 );
		m_logo[ 0 ]->SetPosition( 100 , 50 );
	}
	for( int i = 1 ; i < m_logo.size() ; ++i )
	{
		m_logo[ i ]->SetPosition( 280 , ( i + 1 ) * 100 + 50 );
		m_logo[ i ]->SetColor( m_scene_num == ( i - 1 ) ? 0xFFFFF000 : 0xFFFF0000 );
	}
}

//描画処理
void CSceneTitle::SceneFrameRender( const float elapsed_time )
{
	HRESULT hr;

	// 画面のクリア																	  //画面の色
	V( CGraphicsManager::m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 ) );

	//描画
	CGraphicsManager::SysRender( m_camera );

	//ロゴ描画
	CGraphicsManager::m_pd3dDevice->BeginScene();
	for( int i = 0 ; i < m_logo.size() ; ++i ) m_logo[ i ]->Draw();
	CGraphicsManager::m_pd3dDevice->EndScene();
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
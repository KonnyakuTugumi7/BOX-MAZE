#include "DXUT.h"
#include "scene_manager.h"
#include "../../Library/Support/common.h"
#include "../../Library/Input/input.h"
#include "../../Library/Sound/sound.h"
#include "../../Library/Sound/sound_manager.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../GameEngine/Graphic/sprite.h"
#include "../../Game/Scene/scene_title.h"
#include "../../Game/Scene/scene_play.h"

//初期化
void CSceneManager::InitSceneManager()
{
	//インスタンス作成
	m_SceneMain = std::make_shared< CSceneTitle >();

	//サウンド再生
	m_SceneMain->ScenePlaySound();

	//フェードシーン作成
	m_Fade = CSceneFade::CreateScene();
}

//解放
void CSceneManager::DeleteSceneManager()
{
	//解放
	m_SceneMain.reset();
	m_Fade.reset();
}

void CSceneManager::SetNextScene( SceneBaseSP ( *next )() )
{
	//シーンのを予約
	m_SceneNext = next;

	//フェードアウト
	m_Fade->SetFadeState( m_Fade->FADE_OUT );
}


//次のシーンに移動
void CSceneManager::ChangeScene()
{
	//サウンド停止
	m_SceneMain->SceneStopSound();

	//シーン移動
	m_SceneMain = m_SceneNext();

	//シーン予約をリセット
	m_SceneNext = NULL;

	//サウンド再生
	m_SceneMain->ScenePlaySound();
}

//ロジック
void CSceneManager::FrameMove( const float elapsed_time )
{
	//キーボード更新
	K_B->Update();

	//マウス更新
    MOUSE->Update();

	//キーボード&マウスロック
	if( m_Fade->GetFadeState() == m_Fade->FADE_IN || m_Fade->GetFadeState() == m_Fade->FADE_OUT )
	{
		K_B->SetIsActive( false );
		MOUSE->SetIsActive( false );
	}
	//アンロック
	else
	{
		K_B->SetIsActive( true );
		MOUSE->SetIsActive( true );
	}

	//サウンドマネージャの更新
	CSoundManager::Updata();

	//グラフィックマネージャの更新
	CGraphicsManager::Update();

	//メインロジック
	m_SceneMain->SceneFrameMove( elapsed_time );

	//フェードロジック
	m_Fade->SceneFrameMove( elapsed_time );

	//シーンチェンジ
	//次のシーンが予約されていた時かつフェードインの時
	if( m_SceneNext != NULL && m_Fade->GetFadeState() == m_Fade->FADE_IN )
	{
		ChangeScene();
	}
}

//描画
void CSceneManager::FrameRender( const float elapsed_time )
{
	//メイン描画
	m_SceneMain->SceneFrameRender( elapsed_time );

	//フェード描画
	m_Fade->SceneFrameRender( elapsed_time );
}
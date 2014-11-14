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
	m_scene_main = std::make_shared< CSceneTitle >();

	//サウンド再生
	m_scene_main->ScenePlaySound();

	//フェードシーン作成
	m_fade = CSceneFade::CreateScene();
}

//解放
void CSceneManager::DeleteSceneManager()
{
	//解放
	m_scene_main.reset();
	m_fade.reset();
}

void CSceneManager::SetNextScene( SceneBaseSP ( *next )() )
{
	//シーンのを予約
	m_scene_next = next;

	//フェードアウト
	m_fade->SetFadeState( m_fade->FADE_OUT );
}


//次のシーンに移動
void CSceneManager::ChangeScene()
{
	//サウンド停止
	m_scene_main->SceneStopSound();

	//シーン移動
	m_scene_main = m_scene_next();

	//シーン予約をリセット
	m_scene_next = NULL;

	//サウンド再生
	m_scene_main->ScenePlaySound();
}

//ロジック
void CSceneManager::FrameMove( const float elapsed_time )
{
	//キーボード更新
	K_B->Update();

	//マウス更新
    MOUSE->Update();

	//キーボード&マウスロック
	if( m_fade->GetFadeState() == m_fade->FADE_IN || m_fade->GetFadeState() == m_fade->FADE_OUT )
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
	m_scene_main->SceneFrameMove( elapsed_time );

	//フェードロジック
	m_fade->SceneFrameMove( elapsed_time );

	//シーンチェンジ
	//次のシーンが予約されていた時かつフェードインの時
	if( m_scene_next != NULL && m_fade->GetFadeState() == m_fade->FADE_IN )
	{
		ChangeScene();
	}
}

//描画
void CSceneManager::FrameRender( const float elapsed_time )
{
	//メイン描画
	m_scene_main->SceneFrameRender( elapsed_time );

	//フェード描画
	m_fade->SceneFrameRender( elapsed_time );
}
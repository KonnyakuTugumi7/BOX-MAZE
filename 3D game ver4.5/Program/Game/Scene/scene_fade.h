#pragma once

#include "DXUT.h"
#include "../../Library/Support/csv.h"
#include "../../GameEngine/Scene/scene_base.h"
#include "../../GameEngine/Graphic/sprite.h"
#include <map>
#include <list>
#include <time.h>
#include <memory>

class CSceneFade;
typedef std::shared_ptr< CSceneFade > SceneFadeSP;

class CSceneFade : public CSceneBase{
private:
	//フェード用画像
	SpriteSP m_fade;

	//フェード管理
	enum FADE_MODE;
	FADE_MODE m_fade_mode;

public:
	//フェード管理
	enum FADE_MODE
	{
		//フェードいていない(通常の状態)
		FADE_NON = 0 , 

		//フェードイン
		FADE_IN = -1 , 

		//フェードアウト
		FADE_OUT = 1
	};

public:
	//インスタンス作成
	static SceneFadeSP CreateScene(){ return std::make_shared< CSceneFade >(); }

	//コンストラクタ
	CSceneFade()
	{
		//初期化
		SceneInit();
	}

	//デストラクタ
	~CSceneFade()
	{
		//解放
		SceneDelete();
	}
	
	//初期化
	void SceneInit();

	//解放
	void SceneDelete();

	//ロジック
	//引数1:経過時間
	void SceneFrameMove( const float elapsed_time );

	//描画
	//引数1:経過時間
	void SceneFrameRender( const float elapsed_time );

	//サウンド再生
	void ScenePlaySound(){}

	//サウンド停止
	void SceneStopSound(){}

	//フェード状態ゲッター
	FADE_MODE GetFadeState(){ return m_fade_mode; }

	//フェード状態セッター
	void SetFadeState( const FADE_MODE mode ){ m_fade_mode = mode; }
};

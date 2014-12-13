#pragma once

#include "../../GameEngine/Scene/scene_base.h"
#include "../../Game/Scene/scene_fade.h"
#include <memory>

#pragma warning( disable : 4244 )

class CSceneManager{
private:
	//フェードシーン
	SceneFadeSP m_Fade;

	//現在のシーンのポインタ
	SceneBaseSP m_SceneMain;

	//次のシーンのCreateSceneを呼ぶときに使う関数ポインタ(フェードアウトが終わったタイミングで呼び出すために関数ポインタを使用)
	SceneBaseSP ( *m_SceneNext )();

private:
	//コンストラクタ
	CSceneManager(){}

	//デストラクタ
	~CSceneManager(){}

public:	
	//インスタンス取得
	static CSceneManager* GetInstance()
	{
		static CSceneManager instance;
		return &instance;
	}

	//初期化
	void InitSceneManager();
	
	//解放
	void DeleteSceneManager();

	//次のシーンを予約
	void SetNextScene( SceneBaseSP ( *next )() );

	//次のシーンのに移動
	void ChangeScene();

	//ロジック
	//引数1:経過時間
	void FrameMove( const float elapsed_time );

	//描画
	//引数1:経過時間
	void FrameRender( const float elapsed_time );
};
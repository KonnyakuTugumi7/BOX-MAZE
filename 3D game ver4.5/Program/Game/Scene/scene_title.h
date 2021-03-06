#pragma once

#include "DXUT.h"
#include "../../Library/Support/csv.h"
#include "../../GameEngine/Scene/scene_base.h"
#include "../../GameEngine/Graphic/sprite.h"
#include "../../GameEngine/Graphic/board.h"
#include <string>
#include <memory>
#include <vector>

#pragma warning( disable : 4244 )

class CSceneTitle;
typedef std::shared_ptr< CSceneTitle > SceneTitleSP;

class CSceneTitle : public CSceneBase{
private:
	//タイトル管理番号
	enum SCENE_NUM{
		PLAY = 0 ,
		EDIT ,
		EXIT ,
		SCENE_MAX ,
		SCENE_MIN = -1
	};

	//タイトル管理番号
	enum STAGE_SELECT_NUM{
		STAGE01 = 0 ,
		STAGE02 ,
		TITLE ,
		STAGE_MAX ,
		STAGE_MIN = -1
	};

	//ステージ選択画面かどうか
	bool m_IsStageSelect;

	//タイトル管理番号
	int m_SceneNum;

	//タイトル画像
	BoardSP m_Back;

	//タイトルロゴ
	FontSP m_Font;
	//タイトルロゴ文字列
	std::vector< std::wstring > m_Logo;
	std::vector< std::wstring > m_SelectLogo;

public:
	//インスタンス作成
	static SceneBaseSP CreateScene(){ return std::make_shared< CSceneTitle >(); }

	//コンストラクタ
	CSceneTitle()
	{
		//初期化
		SceneInit();
	}

	//デストラクタ
	~CSceneTitle()
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
	void ScenePlaySound();

	//サウンド停止
	void SceneStopSound();

	//プレイヤーの入力
	void TitleInput();
	void StageSelectInput();

	//タイトル画面描画
	void TitleRender();

	//ステージセレクト画面描画
	void StageSelectRender();
};

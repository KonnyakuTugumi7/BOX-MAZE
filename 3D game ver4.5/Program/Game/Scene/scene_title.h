#pragma once

#include "DXUT.h"
#include "../../Library/Support/csv.h"
#include "../../GameEngine/Scene/scene_base.h"
#include "../../GameEngine/Graphic/sprite.h"
#include "../../GameEngine/Graphic/board.h"
#include <string>
#include <memory>

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
	bool m_is_stage_select;

	//タイトル管理番号
	int m_scene_num;

	//タイトル画像
	SpriteSP m_back;// , m_logo[ 4 ];
	//BoardSP m_board;

	//タイトルロゴ
	FontSP m_logo[ 3 ];

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

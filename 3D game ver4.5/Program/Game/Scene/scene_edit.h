#pragma once

#include "DXUT.h"
#include "../../Library/Support/csv.h"
#include "../../GameEngine/Graphic/sprite.h"
#include "../../GameEngine/Scene/scene_base.h"
#include "../../GameEngine/Camera/camera_base.h"
#include "../../GameEngine/Graphic/cube_manager.h"
#include "../../GameEngine/Graphic/cube.h"
#include <map>
#include <list>
#include <time.h>
#include <memory>

class CSceneEdit;
typedef std::shared_ptr< CSceneEdit > SceneEditSP;

class CSceneEdit : public CSceneBase{
private:
	//キューブマネージャ
	CubeManagerSP m_cube_manager;

	//選択キューブ
	CubeSP m_select_cube;

	//画面中央の十字カーソル
	SpriteSP m_center_carsor;

	//選択キューブ管理ナンバー
	int m_select_cube_state;

	//ターゲット移動速度
	float m_move_speed;

	//ターゲット座標
	D3DXVECTOR3 m_pos;

public:
	//インスタンス作成
	static SceneBaseSP CreateScene(){ return std::make_shared< CSceneEdit >(); }

	//コンストラクタ
	CSceneEdit()
	{
		//初期化
		SceneInit();
	}

	//デストラクタ
	~CSceneEdit()
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

	//当たり判定
	void SceneCollision();

	//書き出し
	void SceneExport();
};

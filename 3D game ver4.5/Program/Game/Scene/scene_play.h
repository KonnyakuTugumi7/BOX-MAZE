#pragma once

#include "DXUT.h"
#include "../../Library/Support/csv.h"
#include "../../GameEngine/Scene/scene_base.h"
#include "../../GameEngine/Camera/camera_base.h"
#include "../../GameEngine/Graphic/xfile.h"
#include "../../GameEngine/Graphic/cube_manager.h"
#include "../../Game/Object/user_interface.h"
#include <map>
#include <list>
#include <time.h>
#include <memory>

class CScenePlay;
typedef std::shared_ptr< CScenePlay > ScenePlaySP;

class CScenePlay : public CSceneBase{
private:
	//キューブマネージャ
	CubeManagerSP m_cube_manager;

	//ユーザーインターフェース
	UserInterfaceSP m_ui;

public:
	//インスタンス作成
	static SceneBaseSP CreateScene(){ return std::make_shared< CScenePlay >(); }

	//コンストラクタ
	CScenePlay()
	{
		//初期化
		SceneInit();
	}

	//デストラクタ
	~CScenePlay()
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

	//入力
	void PlayInput();

	//カメラ(自機をスタートキューブへ)
	void CameraToStartCube();
};

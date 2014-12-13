#pragma once

#include "DXUT.h"
#include "../../Library/Support/csv.h"
#include "../../GameEngine/Graphic/sprite.h"
#include "../../GameEngine/Scene/scene_base.h"
#include "../../GameEngine/Camera/camera_base.h"
#include "../../Game/Camera/view_camera.h"
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
	CubeManagerSP m_CubeManager;

	//選択キューブ
	CubeSP m_SelectCube;

	//画面中央の十字カーソル
	BoardSP m_CenterCarsor;

	//フォント
	FontSP m_Font;
	std::vector< std::wstring > m_Logo;

	//カメラ
	ViewCameraSP m_ViewCamera;

	//選択キューブ管理ナンバー
	int m_SelectCubeState;

	////ターゲット移動速度
	//float m_MoveSpeed;

	//ターゲット座標
	D3DXVECTOR3 m_Pos;

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

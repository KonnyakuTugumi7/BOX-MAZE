#pragma once

#include "../../GameEngine/Camera/camera_base.h"
#include "../../Library/Graphic/font.h"
#include <memory>

class CSceneBase;
typedef std::shared_ptr< CSceneBase > SceneBaseSP;

//シーン基本クラス
class CSceneBase{
protected:
	//サウンドID
	int m_sound_id;

	//CSV
	CsvSP m_csv;

	//フォント
	FontSP m_font;

	//カメラインスタンス
	CameraBaseSP m_camera;

public:
	//コンストラクタ
	CSceneBase() : m_sound_id( 0 )
	{}

	//仮想デストラクタ
	virtual ~CSceneBase()
	{
		m_csv.reset();
		m_camera.reset();
	}

	//初期化
	virtual void SceneInit() = 0;

	//解放
	virtual void SceneDelete() = 0;

	//ロジック
	//引数1:経過時間
	virtual void SceneFrameMove( const float elapsed_time ) = 0;

	//描画
	//引数1:経過時間
	virtual void SceneFrameRender( const float elapsed_time ) = 0;

	//サウンド再生
	virtual void ScenePlaySound() = 0;

	//サウンド停止
	virtual void SceneStopSound() = 0;
};

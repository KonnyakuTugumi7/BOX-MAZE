#pragma once

#include "DXUT.h"
#include "../../Library/Graphic/graphic_base.h"
#include "../../Library/Graphic/shader_base.h"
#include "../../Library/Graphic/shader_depth_buffer_shadow.h"
#include "../../Library/Graphic/shader_directional_light.h"
#include "../../Library/Graphic/shader_lambert.h"
#include <list>
#include <vector>

#pragma warning ( disable : 4238 ) //非標準の拡張機能が使用されています : 右辺値のクラスが左辺値に使用されます。
#pragma warning ( disable : 4482 ) //非標準の拡張機能が使用されています: enum 'CGraphicsManager::RENDERLIST_STATE' が限定名で使用されます

//グラフィックス関連統括マネージャ
class CGraphicsManager{
public:
	//描画に使用するDirectXのデバイスへのポインタ
	static IDirect3DDevice9* m_pd3dDevice;

	//使用するシェーダ
	static ShaderBaseSP m_sdr_use;

	//基本シェーダ
	static ShaderBaseSP m_sdr_base;

	//平行光源シェーダ
	static ShaderDirectionalLightSP m_sdr_dir_light;

	//レンダーリスト
	static std::vector< std::list< GraphicBaseSP > > m_render_list;

	//レンダリングリストの種類
	static enum RENDERLIST_STATE{
		BACK_2D ,
		FRONT_2D ,
		NORMAL_3D ,
		ALPHA_3D ,
		STATE_NUM
	};

	//ウィンドウ幅
	static float m_window_width;

	//ウィンドウ高さ
	static float m_window_height;

private:
	//コンストラクタ
	CGraphicsManager(){};

	//デストラクタ
	~CGraphicsManager(){};

public:
	//初期化
	//引数1:DirectXのデバイスのポインタ
	static void Initialize( IDirect3DDevice9* pd3dDevice );
	
	//解放
	static void Destroy();

	//描画
	static void SysRender( const CameraBaseWP camera );

	//更新
	static void Update();

	//オブジェクト登録
	//引数1:登録するオブジェクト , 引数2:
	static void RegistObj( const GraphicBaseSP obj , const RENDERLIST_STATE list );
};


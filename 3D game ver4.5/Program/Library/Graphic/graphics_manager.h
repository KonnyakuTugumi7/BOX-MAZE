#pragma once

#include "DXUT.h"
#include "../../Library/Graphic/graphic_base.h"
#include "../../Library/Graphic/shader_base.h"
#include "../../Library/Graphic/shader_depth_buffer_shadow.h"
#include "../../Library/Graphic/shader_directional_light.h"
#include "../../Library/Graphic/shader_lambert.h"
#include "../../Game/Camera/view_camera.h"
#include <list>
#include <vector>

#pragma warning ( disable : 4238 ) //非標準の拡張機能が使用されています : 右辺値のクラスが左辺値に使用されます。
#pragma warning ( disable : 4482 ) //非標準の拡張機能が使用されています: enum 'CGraphicsManager::RENDERLIST_STATE' が限定名で使用されます

//グラフィックス関連統括マネージャ
class CGraphicsManager{
public:
	//描画に使用するDirectXのデバイスへのポインタ
	static IDirect3DDevice9* m_pD3dDevice;

	//使用するシェーダ
	static ShaderBaseSP m_SdrUse;

	//基本シェーダ
	static ShaderBaseSP m_SdrBase;

	//平行光源シェーダ
	static ShaderDirectionalLightSP m_SdrDirLight;

	//レンダーリスト
	static std::vector< std::list< GraphicBaseSP > > m_RenderList;

	//3Dカメラ
	static CameraBaseSP m_3dCamera;

	//2Dカメラ
	static CameraBaseSP m_2dCamera;

	//レンダリングリストの種類
	static enum RENDERLIST_STATE{
		BACK_2D ,
		NORMAL_3D ,
		ALPHA_3D ,
		FRONT_2D ,
		STATE_NUM
	};

	//ウィンドウ幅
	static float m_WindowWidth;

	//ウィンドウ高さ
	static float m_WindowHeight;

private:
	//コンストラクタ
	CGraphicsManager(){}

	//デストラクタ
	~CGraphicsManager(){}

	//コピーコンストラクタ
	CGraphicsManager( const CGraphicsManager& obj ){}

	// = 演算子のオーバーロード
	CGraphicsManager& operator = ( const CGraphicsManager& obj ){}

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


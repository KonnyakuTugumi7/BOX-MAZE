#pragma once

#include "DXUT.h"
#include "../../Library/Graphic/graphic_base.h"
#include "../../Library/Graphic/graphics_manager.h"
#include <memory>
#include <string>

#pragma warning ( disable : 4244 )

class CCube;
//typedef std::weak_ptr< CCube > CubeWP;

class CBoard;
typedef std::shared_ptr< CBoard > BoardSP;

//板ポリクラス
class CBoard : public CGraphicBase{
protected:
	//ビルボード判定フラグ
	bool m_IsBillboard;

	//板ポリサイズ
	D3DXVECTOR3 m_BoardSize;

	//キューブのウィクポ・・・Cube(親)のワールドマトリックスを取得するため
	std::weak_ptr< CCube > m_Cube;

public:
	//コンストラクタ
	CBoard(){}

	//コンストラクタ
	//引数1:ボードの幅 , 引数2:ボードの高さ , 引数3:テクスチャパス
	CBoard( const int width , const int height , const std::string path = "" ){ Init( width , height , path ); }

	//デストラクタ
	virtual ~CBoard(){}

	//生成
	//引数1:ボードの幅 , 引数2:ボードの高さ , 引数3:テクスチャパス , 引数4:レンダリングリストの種類
	static BoardSP Create( const int width , const int height , const std::string path = "" , const CGraphicsManager::RENDERLIST_STATE list = CGraphicsManager::NORMAL_3D );

	//初期化
	//引数1:ボードの幅 , 引数2:ボードの高さ , 引数3:テクスチャパス
	void Init( const int width , const int height , const std::string path = "" );

	//描画
	//引数1:カメラ
	virtual void Render( const CameraBaseWP camera );

	//ワールドマトリックス設定(billboard用) //カメラの方向を向く用
	//引数1:カメラ , 引数2:親のワールドマトリックス
	void SetWldMtxBillBoard( const CameraBaseWP camera );

	//キューブウィークポインタセッター
	inline void SetCubeWP( const std::weak_ptr< CCube > cube ){ m_Cube = cube; }

	//テクスチャセッター
	//引数1:テクスチャ(板ポリには今のところ一枚につき一つのテクスチャしか設定していないのでこれでおk)
	void SetTex( const TextureSP tex );
};
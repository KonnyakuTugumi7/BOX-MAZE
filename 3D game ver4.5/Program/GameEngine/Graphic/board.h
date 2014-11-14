#pragma once

#include "DXUT.h"
#include "../../Library/Graphic/graphic_base.h"
#include <memory>
#include <string>

#pragma warning ( disable : 4244 )

class CGraphicBase;
class CCube;
typedef std::weak_ptr< CCube > CubeWP;
class CCameraBase;
typedef std::weak_ptr< CCameraBase > CameraBaseWP;

class CBoard;
typedef std::shared_ptr< CBoard > BoardSP;

//板ポリクラス
class CBoard : public CGraphicBase{
protected:
	//ビルボード判定フラグ
	bool m_is_billboard;

	//板ポリサイズ
	D3DXVECTOR3 m_board_size;

	//キューブのウィクポ・・・Cube(親)のワールドマトリックスを取得するため
	CubeWP m_cube;

public:
	//コンストラクタ
	CBoard(){}

	//コンストラクタ
	//引数1:ボードの幅 , 引数2:ボードの高さ , 引数3:テクスチャパス
	CBoard( const int width , const int height , const std::string path = "" ){ Init( width , height , path ); }

	//デストラクタ
	~CBoard(){}

	//生成
	//引数1:ボードの幅 , 引数2:ボードの高さ , 引数3:テクスチャパス
	static BoardSP Create( const int width , const int height , const std::string path = "" );

	//初期化
	//引数1:ボードの幅 , 引数2:ボードの高さ , 引数3:テクスチャパス
	void Init( const int width , const int height , const std::string path = "" );

	//描画
	//引数1:カメラ
	virtual void Render( const CameraBaseWP camera );

	//キューブウィークポインタセッター
	inline void SetCubeWP( const CubeWP cube ){ m_cube = cube; }

	//テクスチャセッター
	//引数1:テクスチャ(板ポリには今のところ一枚につき一つのテクスチャしか設定していないのでこれでおk)
	inline void SetTex( const TextureSP tex ){ m_texs.clear();m_texs.push_back( tex ); }
};
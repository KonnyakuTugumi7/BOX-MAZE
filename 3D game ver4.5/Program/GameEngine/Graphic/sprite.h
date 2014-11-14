#pragma once

#include "DXUT.h"
#include "../../Library/Graphic/animation.h"
#include "../../Library/Graphic/graphic_base.h"
#include "../../Library/Graphic/graphics_manager.h"
#include <string>
#include <memory>

#pragma warning ( disable : 4390 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4238 )

class CCameraBase;
typedef std::weak_ptr< CCameraBase > CameraBaseWP;

class CSprite;
typedef std::shared_ptr< CSprite > SpriteSP;

//スプライトクラス
class CSprite : public CAnimation , public CGraphicBase{
private:
	//スプライト(DirectXの2Dを扱うクラス)
	LPD3DXSPRITE m_sprite;

	//ファイル名
	std::string m_name;

public:
	//コンストラクタ
	CSprite(){}

	//デストラクタ
	~CSprite()
	{
		//スプライトの解放
		SAFE_RELEASE( m_sprite );
	}

	//生成
	//引数1:ファイルの名前 , 引数2:レンダリングリストの種類 , 引数3:横に分割する数 , 引数4:縦に分割する数
	static SpriteSP Create( const std::string file_name , const CGraphicsManager::RENDERLIST_STATE list , const int x_num = 1 , const int y_num = 1 );

	//初期化
	//引数1:ファイルの名前 , 引数2:横に分割する数 , 引数3:縦に分割する数
	void Init( const std::string file_name , const int x_num = 1 , const int y_num = 1 );

	//描画
	//引数1:カメラ
	void Render( const CameraBaseWP camera );
};
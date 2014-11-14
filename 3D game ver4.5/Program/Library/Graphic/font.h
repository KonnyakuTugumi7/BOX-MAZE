#pragma once

#include "../../GameEngine/Graphic/board.h"
#include <D3dx9core.h>
#include <string>
#include <memory>

#pragma warning ( disable : 4996 )

class CFont;
typedef std::shared_ptr< CFont > FontSP;

//フォントクラス
class CFont{
private:
	//フォント
	ID3DXFont* m_font;

public:
	//フォントカラー
	enum FontColor{
		//黒
		BLACK = 0xFF000000 ,

		//白
		WHITE = 0xFFFFFFFF ,

		//赤
		RED = 0xFFFF0000 ,

		//緑
		GREEN = 0xFF00FF00 ,

		//青
		BLUE = 0xFF0000FF ,

		//黄
		YELLOW = 0xFFFFF000
	};

public:
	//コンストラクタ
	CFont(){}

	//デストラクタ
	~CFont(){ SAFE_RELEASE( m_font ); }

	//生成
	//引数1:文字列の幅 , 引数2:文字列の高さ , 引数3:フォントタイプ(0:ゴシック , 1:明朝)
	static FontSP Create( const int width , const int height , const int type = 0 );

	//初期化
	//引数1:文字列の幅 , 引数2:文字列の高さ , 引数3:フォントタイプ(0:ゴシック , 1:明朝)
	void Init( const int width , const int height , const int type );

	//描画
	//引数1:描画する文字列 , 引数2:描画座標(x) , 引数3:描画座標(y) , 引数4:文字の色
	void DrawFont( const std::string str , const int posX , const int posY , const FontColor fontcolor );
};

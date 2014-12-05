#pragma once

#include "../../GameEngine/Graphic/board.h"
#include "../../GameEngine/Camera/camera_base.h"
#include "../../Library/Graphic/texture.h"
#include <D3dx9core.h>
#include <string>
#include <memory>
#include <hash_map>

#pragma warning ( disable : 4996 )

class CFont;
typedef std::shared_ptr< CFont > FontSP;

//フォントクラス
class CFont : public CBoard{
private:
	//テクスチャのデータマップ
	std::hash_map<wchar_t, LPDIRECT3DTEXTURE9> m_data;

	//整列順
	enum Align;
	Align m_align;

	//スケール
	D3DXVECTOR3 m_scale;

private:
	//描画
	//引数1:カメラ
	virtual void Render( const CameraBaseWP camera );

public:
	//生成
	static FontSP Create(){ return std::make_shared< CFont >(); }

	//コンストラクタ
	CFont() : m_align( CFont::LEFT ) , m_scale( 1.0f , 1.0f , 1.0f )
	{
		Init( 1 , 1 );
		m_texs.push_back( CTexture::CreateEmptyTex( 1.0f , 1.0f ) );
	}

	//デストラクタ
	~CFont();

	//フォントの整列順
	enum Align { LEFT, RIGHT, CENTER };

	//フォントテクスチャを生成
	//size: 大きさ , weight: 太さ , type: 書式 , fonts: 生成する文字列
	bool CreateTexture( std::size_t size , std::size_t weight , const std::wstring& type , const std::wstring& fonts );

	//描画処理(文字列はフォントテクスチャが生成されていなければ描画されない)
	//camera: カメラ , string: 文字列 , pos: 座標 , color: 色(RGBA)
	void Draw( const CameraBaseWP camera , const std::wstring& string , const D3DXVECTOR2& pos , const D3DXVECTOR4& color );

	//整列順を設定
	void SetAlign( const Align& align ){ m_align = align; }

	//大きさを設定
	//width: 幅 , height: 高さ
	void SetSize( const float width , const float height ) { m_scale.x = width; m_scale.y = height; }
};

//書式を定義
namespace FontType
{
	const std::wstring MS_GOTHIC(L"ＭＳ ゴシック");
	const std::wstring MS_MINCHOU(L"ＭＳ 明朝");
	const std::wstring HG_CIRCLE_GOTHIC(L"HG丸ｺﾞｼｯｸM-PRO");
	const std::wstring HGP_POP(L"HGP創英角ﾎﾟｯﾌﾟ体");
	const std::wstring MEIRIO(L"メイリオ");

}

//利用頻度の高い文字を定義（テクスチャ生成用）
namespace FontUseful
{
	const std::wstring NUMBER(L"0123456789");
	const std::wstring ALPHABET_SMALL(L"abcdefghijklmnopqrstuvwxyz");
	const std::wstring ALPHABET_LARGE(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	const std::wstring ALPHABET(ALPHABET_SMALL + ALPHABET_LARGE);
	const std::wstring HIRAGANA_A(L"あいうえおぁぃぅぇぉ");
	const std::wstring HIRAGANA_KA(L"かきくけこがぎぐけご");
	const std::wstring HIRAGANA_SA(L"さしすせそざじずぜぞ");
	const std::wstring HIRAGANA_TA(L"たちつてとだぢづでど");
	const std::wstring HIRAGANA_NA(L"なにぬねの");
	const std::wstring HIRAGANA_HA(L"はひふへほばびぶべぼぱぴぷぺぽ");
	const std::wstring HIRAGANA_MA(L"まみむめも");
	const std::wstring HIRAGANA_YA(L"やゆよゃゅょ");
	const std::wstring HIRAGANA_RA(L"らりるれろ");
	const std::wstring HIRAGANA_WA(L"わゐをゑん");
	const std::wstring HIRAGANA(HIRAGANA_A + HIRAGANA_KA + HIRAGANA_SA + HIRAGANA_TA + HIRAGANA_NA + HIRAGANA_HA + HIRAGANA_MA + HIRAGANA_YA + HIRAGANA_RA + HIRAGANA_WA);
	const std::wstring KATAKANA_A(L"アイウエオァィゥェォ");
	const std::wstring KATAKANA_KA(L"カキクケコガギグゲゴ");
	const std::wstring KATAKANA_SA(L"サシスセソザジズゼゾ");
	const std::wstring KATAKANA_TA(L"タチツテトダヂヅデド");
	const std::wstring KATAKANA_NA(L"ナニヌネノ");
	const std::wstring KATAKANA_HA(L"ハヒフヘホバビブベボパピプペポ");
	const std::wstring KATAKANA_MA(L"マミムメモ");
	const std::wstring KATAKANA_YA(L"ヤユヨャュョ");
	const std::wstring KATAKANA_RA(L"ラリルレロ");
	const std::wstring KATAKANA_WA(L"ワヰヲヱン");
	const std::wstring KATAKANA(KATAKANA_A + KATAKANA_KA + KATAKANA_SA + KATAKANA_TA + KATAKANA_NA + KATAKANA_HA + KATAKANA_MA + KATAKANA_YA + KATAKANA_RA + KATAKANA_WA);

}

//色を定義
namespace Color
{
	const D3DXVECTOR3 WHITE(1.0f, 1.0f, 1.0f);
	const D3DXVECTOR3 BLACK(0.0f, 0.0f, 0.0f);
	const D3DXVECTOR3 YELLOW(1.0f, 1.0f, 0.0f);
	const D3DXVECTOR3 GREEN(0.0f, 1.0f, 0.0f);
	const D3DXVECTOR3 RED(1.0f, 0.0f, 0.0f);
	const D3DXVECTOR3 BLUE(0.0f, 0.0f, 1.0f);
	const D3DXVECTOR3 LIGHT_BLUE(0.0f, 1.0f, 1.0f);
	const D3DXVECTOR3 PURPLE(1.0f, 0.0f, 1.0f);
}

class CDebugFont;
typedef std::shared_ptr< CDebugFont > DebugFontSP;

//フォントクラス
class CDebugFont{
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
	CDebugFont(){}

	//デストラクタ
	~CDebugFont(){ SAFE_RELEASE( m_font ); }

	//生成
	//引数1:文字列の幅 , 引数2:文字列の高さ , 引数3:フォントタイプ(0:ゴシック , 1:明朝)
	static DebugFontSP Create( const int width , const int height , const int type = 0 );

	//初期化
	//引数1:文字列の幅 , 引数2:文字列の高さ , 引数3:フォントタイプ(0:ゴシック , 1:明朝)
	void Init( const int width , const int height , const int type );

	//描画
	//引数1:描画する文字列 , 引数2:描画座標(x) , 引数3:描画座標(y) , 引数4:文字の色
	void DrawFont( const std::string str , const int posX , const int posY , const FontColor fontcolor );
};
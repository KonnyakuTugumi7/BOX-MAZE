#ifndef BILLBOARDSTRING_H
#define BILLBOARDSTRING_H

#include "DXUT.h"
#include "../../Library/Graphic/BillboardChar.h"
#include <vector>
#include <memory>

//書式を定義
namespace FACE_NAME
{
	const std::wstring MS_GOTHIC(L"ＭＳ ゴシック");
	const std::wstring MS_MINCHOU(L"ＭＳ 明朝");
	const std::wstring HG_CIRCLE_GOTHIC(L"HG丸ｺﾞｼｯｸM-PRO");
	const std::wstring HGP_POP(L"HGP創英角ﾎﾟｯﾌﾟ体");
	const std::wstring MEIRIO(L"メイリオ");
}

class CBillboardString;
typedef std::shared_ptr< CBillboardString > BillboardStringSP;

//ビルボード文字列クラス
//デフォルト値:32ピクセルサイズ , 白色 , 透過度255(不透明) , MSゴシック
class CBillboardString{
private:
	//デバイス
	IDirect3DDevice9* m_Dev;
	//ビルボード文字列
	std::vector< CBillboardChar* > m_StrAry;
	//座標を更新するかどうか
	bool m_IsUpdatePosition;
	//アップデート(文字列を再構成するかどうか)
	bool m_IsUpdate;
	//現在登録されている文字列の長さ
	unsigned int m_CurLen;

protected:
	//カラー
	DWORD m_Color;
	//背景板ポリカラー
	DWORD m_BGColor;
	//ピクセルサイズ
	unsigned int m_PixelSize;
	//アルファ
	int m_Alpha;
	//サイズ
	D3DXVECTOR2 m_Scale;
	//座標
	D3DXVECTOR2 m_BasePos;
	//フェイス名前
	TCHAR m_FaceName[ LF_FACESIZE ];

protected:
	//位置関係更新
	void UpdatePositions();

	//更新
	void Update();

public:
	//コンストラクタ
	//dev:グラフィックデバイス
	CBillboardString( IDirect3DDevice9* dev );
	//デストラクタ
	~CBillboardString();

	//生成
	//dev:グラフィックデバイス
	static BillboardStringSP Create( IDirect3DDevice9* dev ){ return std::make_shared< CBillboardString >( dev ); }

	//--------------------再構築が必要--------------------
	//文字列登録(既に登録されている文字列は破棄し、新しいフォントテクスチャを作成※同じ文字列の場合は変更なし)
	//str:文字列
	void RegistString( const TCHAR* str );

	//フォント設定
	//FaceName:フェース(文字コード)名
	bool SetFont( const TCHAR* FaceName );

	//フォントを囲むボックスの最大幅を設定(実際のフォントの大きさはこの値より若干小さくなる)
	//size:フォント最大サイズ
	bool SetMaxPixelSize( unsigned int size );
	//----------------------------------------------------

	//フォントを貼り付ける板ポリゴンのスケール設定(基本的にSetMaxPixelSizeで設定したフォントのサイズと等倍にすべし)
	//xs:xスケール , ys:yスケール
	void SetScale( float xs, float ys );

	//位置設定
	//x:x座標 , y:y座標
	void SetPosition( float x, float y );

	//アルファ値設定
	//alpha:アルファ値( 0～255 )
	void SetAlpha( int alpha );

	//文字のベースカラー設定
	//color:カラー値(XRGB)
	void SetColor( DWORD color );

	//描画
	bool Draw();
};

#endif
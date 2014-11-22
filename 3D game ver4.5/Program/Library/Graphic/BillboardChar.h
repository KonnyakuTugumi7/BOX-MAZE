#ifndef BILLBOARDCHAR_H
#define BILLBOARDCHAR_H

#pragma warning( disable : 4996 ) //'wcsncpy' の宣言を確認してください。

#include "DXUT.h"
#include <string>
#include <tchar.h>

//頂点関係
#define FVF_2DBILLBOARDVERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 ) //座標変換済み頂点

//ビルボード文字クラス
class CBillboardChar{
protected:
	struct _2DBILLBOARDVERTEX{
		float x,y,z;		//頂点座標 
		float rhw;		//除算数
		DWORD dwColor;	//頂点の色
		float u, v;		//テクスチャ座標 
	};

public:
	enum Regist{
		Regist_OK,			//OK
		Regist_SameChar,	//同じ文字
		Regist_Error,		//エラー
	};

	enum BILLBOARDCHAR_RESOLUTION{
		BILLBOARDCHAR_RESOLUTION_LOW = 0,
		BILLBOARDCHAR_RESOLUTION_MIDDLE = 1,
		BILLBOARDCHAR_RESOLUTION_HIGH = 2
	};

private:
	//ビルボード頂点
	static const _2DBILLBOARDVERTEX m_Vertex_g[ 4 ];
	//文字コード変換
	unsigned int TransCharCode( const TCHAR *c );
	//描画デバイス
	IDirect3DDevice9* m_Dev;
	//文字テクスチャ
	IDirect3DTexture9* m_pTex;
	//頂点バッファ
	IDirect3DVertexBuffer9* m_pVertexBuf;
	//描画文字
	TCHAR m_Char[ 2 ];
	//フォント名
	TCHAR m_lfFaceName[ LF_FACESIZE ];
	//フォントハンドル
	HFONT m_hFont;
	bool m_ISFontHandleRecreate;
	//アップデートするかどうか
	bool m_IsUpdate;
	//アルファ値
	BYTE m_Alpha;
	//フォントサイズ
	unsigned int m_MaxPixel;
	//ビットマップ幅
	int m_iBmp_W;
	//ビットマップ高さ
	int m_iBmp_H;
	//ビットマップオフセット
	int m_iBmpOfs_x, m_iBmpOfs_y;
	//テクスチャサイズ
	int m_TexSize;
	//マージン
	int m_R_Margin, m_B_Margin;
	//フォントカラー
	DWORD m_Color;
	//スケール
	float m_XScale, m_YScale;
	//角度
	float m_Angle;
	//座標
	float m_Pos_X, m_Pos_Y;
	//中心座標
	float m_Center_X, m_Center_Y;
	BILLBOARDCHAR_RESOLUTION m_resolution;

protected:
	//ビルボードテクスチャ作成
	bool CreateTexture( unsigned int width, unsigned int height );

	//フォントハンドル生成
	bool CBillboardChar::CreateFontHandle();

	//フォントビットマップを生成
	DWORD CreateFontBMP( TEXTMETRIC &TM, GLYPHMETRICS &GM, BYTE **outBMP );

	//BMPをテクスチャに適用
	bool ApplyBMPToTex( BYTE* bmp, TEXTMETRIC &TM, GLYPHMETRICS &GM );

	//文字をテクスチャに刻印
	bool PlotCharToTex();

	//描画位置更新
	bool UpdateSizeAndPosition( float x, float y );

	//レンダリング準備
	void SetRenderState();

public:
	// コンストラクタ
	CBillboardChar( IDirect3DDevice9 *dev );
	//デストラクタ
	virtual ~CBillboardChar();

	//アップデート
	bool Update();

	//文字登録
	CBillboardChar::Regist RegistChar( const TCHAR *c );

	//基本情報をコピー
	void CopyBaseInfoFrom( CBillboardChar* src );

	//解像度設定
	void SetResolution( BILLBOARDCHAR_RESOLUTION resolution ){ m_resolution = resolution; }

	//フォント文字サイズ指定
	bool SetMaxPixelSize( unsigned int size )
	{
		if( size == 0 ) return false;
		if( m_MaxPixel != size )
		{
			//文字サイズ変更
			m_MaxPixel = size;
			m_IsUpdate = true;
			m_ISFontHandleRecreate = true;
		}
		return true;
	}

	//フォントスケール指定
	void SetScale( float xs, float ys ){ m_XScale = xs; m_YScale = ys; }

	//回転角度指定
	void SetAngle( float angle ){ m_Angle = angle; }

	//位置指定
	void SetPosition( float x, float y ){ m_Pos_X = x; m_Pos_Y = y; }

	//フォント指定
	bool SetFont( const TCHAR* FaceName )
	{
		//同じフォント名が指定されているので更新しない
		if( _tcscmp( m_lfFaceName, FaceName ) == 0 ) return true;
		_tcsncpy( m_lfFaceName, FaceName, LF_FACESIZE-1 );
		m_IsUpdate = true;
		m_ISFontHandleRecreate = true;
		return true;
	}

	//センター位置指定
	void SetCenterPos( float x, float y ){ m_Center_X = x; m_Center_Y = y; }

	//アルファ値指定
	void SetAlpha( int alpha )
	{
		//アルファ値制限
		if( alpha >= 256 ) alpha = 255;
		if( alpha <= 0 ) alpha = 0;

		m_Alpha = ( BYTE )alpha;
	}

	//文字のベースカラー指定
	void SetColor( DWORD color ){ m_Color = color; }

	//フォントテクスチャ取得
	IDirect3DTexture9* GetFontTexture()
	{
		if( m_pTex ) m_pTex->AddRef();
		return m_pTex;
	}

	//表示文字幅取得
	float GetDispWidth(){ return ( m_iBmpOfs_x + m_iBmp_W ) * m_XScale; }

	//表示文字右位置取得
	float GetRightPos(){ return ( m_iBmpOfs_x + m_iBmp_W + m_R_Margin ) * m_XScale + m_Pos_X; }

	//描画
	bool Draw();
};

#endif
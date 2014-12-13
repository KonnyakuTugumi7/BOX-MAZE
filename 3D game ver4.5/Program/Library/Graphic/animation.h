#pragma once

#include "DXUT.h"
#include <vector>
#include <string>
#include <memory>
#include <atlbase.h>

#pragma warning ( disable : 4389 )

class CTime;
typedef std::shared_ptr< CTime > TimeSP;

//2Dアニメーションクラス
class CAnimation{
protected:
	//ループフラグ
	bool m_IsLoop;

	//描画フラグ
	bool m_IsActive;

	//アニメフレーム番号
	int m_AnimationFrame;

	//画像のサイズ
	D3DXVECTOR3 m_ImgSize;

	//画像の中心座標
	D3DXVECTOR3 m_Center;

	//画像を分割して描画する範囲を格納
	std::vector< RECT > m_Rect;

	//画像情報(幅や高さなど)の取得用
	D3DXIMAGE_INFO m_ImageInfo;

	//画像データ(テクスチャ)
	LPDIRECT3DTEXTURE9 m_Texture;

	//アニメーション用タイマー
	TimeSP m_AnimationTimer;

public:
	//コンストラクタ
	CAnimation();

	//デストラクタ
	virtual ~CAnimation();

	//再生
	void Play();
};
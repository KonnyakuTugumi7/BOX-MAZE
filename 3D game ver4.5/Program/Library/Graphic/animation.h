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
	bool m_is_loop;

	//描画フラグ
	bool m_is_active;

	//アニメフレーム番号
	int m_animation_frame;

	//画像のサイズ
	D3DXVECTOR3 m_img_size;

	//画像の中心座標
	D3DXVECTOR3 m_center;

	//画像を分割して描画する範囲を格納
	std::vector< RECT > m_rect;

	//画像情報(幅や高さなど)の取得用
	D3DXIMAGE_INFO m_image_info;

	//画像データ(テクスチャ)
	LPDIRECT3DTEXTURE9 m_texture;

	//アニメーション用タイマー
	TimeSP m_animation_timer;

public:
	//コンストラクタ
	CAnimation();

	//デストラクタ
	virtual ~CAnimation();

	//再生
	void Play();
};
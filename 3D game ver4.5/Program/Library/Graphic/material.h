#pragma once

//マテリアルクラス
class CMaterial{
public:
	//加算フラグ
	bool m_is_add_blend;
	
	//カラー
	D3DXVECTOR4 m_col;

public:
	//コンストラクタ
	CMaterial::CMaterial() : m_is_add_blend( false ) ,
							 m_col( 1 , 1 , 1 , 1 )
	{}

	//デストラクタ
	virtual ~CMaterial(){}

};
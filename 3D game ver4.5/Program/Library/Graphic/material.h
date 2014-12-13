#pragma once

//マテリアルクラス
class CMaterial{
public:
	//加算フラグ
	bool m_IsAddBlend;
	
	//カラー
	D3DXVECTOR4 m_Col;

public:
	//コンストラクタ
	CMaterial::CMaterial() : m_IsAddBlend( false ) ,
							 m_Col( 1 , 1 , 1 , 1 )
	{}

	//デストラクタ
	virtual ~CMaterial(){}

};
#pragma once

#include "DXUT.h"
#include <memory>

class CVertex;
typedef std::shared_ptr< CVertex > VertexSP;

//頂点情報クラス
class CVertex{
public:
	//座標
    D3DXVECTOR3 m_Pos;
	
	//法線
	D3DXVECTOR3 m_Nml;

	//UV
	D3DXVECTOR2	m_Uv;

	//FVFコード
	static const WORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

public:
	//コンストラクタ
	CVertex() : m_Pos( 0 , 0 , 0 ) , 
				m_Nml( 0 , 0 , 0 ) , 
				m_Uv(  0 , 0  )
	{}

	//デストラクタ
	~CVertex(){}
};
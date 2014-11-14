#pragma once

#include "DXUT.h"
#include <memory>

class CVertex;
typedef std::shared_ptr< CVertex > VertexSP;

//頂点情報クラス
class CVertex{
public:
	//座標
    D3DXVECTOR3 m_pos;
	
	//法線
	D3DXVECTOR3 m_nml;

	//UV
	D3DXVECTOR2	m_uv;

	//FVFコード
	static const WORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

public:
	//コンストラクタ
	CVertex() : m_pos( 0 , 0 , 0 ) , 
				m_nml( 0 , 0 , 0 ) , 
				m_uv(  0 , 0  )
	{}

	//デストラクタ
	~CVertex(){}
};
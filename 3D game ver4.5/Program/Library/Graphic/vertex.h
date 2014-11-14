#pragma once

#include "DXUT.h"
#include <memory>

class CVertex;
typedef std::shared_ptr< CVertex > VertexSP;

//���_���N���X
class CVertex{
public:
	//���W
    D3DXVECTOR3 m_pos;
	
	//�@��
	D3DXVECTOR3 m_nml;

	//UV
	D3DXVECTOR2	m_uv;

	//FVF�R�[�h
	static const WORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

public:
	//�R���X�g���N�^
	CVertex() : m_pos( 0 , 0 , 0 ) , 
				m_nml( 0 , 0 , 0 ) , 
				m_uv(  0 , 0  )
	{}

	//�f�X�g���N�^
	~CVertex(){}
};
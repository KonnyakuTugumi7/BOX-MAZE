#pragma once

//�}�e���A���N���X
class CMaterial{
public:
	//���Z�t���O
	bool m_is_add_blend;
	
	//�J���[
	D3DXVECTOR4 m_col;

public:
	//�R���X�g���N�^
	CMaterial::CMaterial() : m_is_add_blend( false ) ,
							 m_col( 1 , 1 , 1 , 1 )
	{}

	//�f�X�g���N�^
	virtual ~CMaterial(){}

};
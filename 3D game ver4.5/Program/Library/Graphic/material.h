#pragma once

//�}�e���A���N���X
class CMaterial{
public:
	//���Z�t���O
	bool m_IsAddBlend;
	
	//�J���[
	D3DXVECTOR4 m_Col;

public:
	//�R���X�g���N�^
	CMaterial::CMaterial() : m_IsAddBlend( false ) ,
							 m_Col( 1 , 1 , 1 , 1 )
	{}

	//�f�X�g���N�^
	virtual ~CMaterial(){}

};
#pragma once 

#include "DXUT.h"
#include <string>
#include <map>
#include <memory>

class CTexture;
typedef std::shared_ptr< CTexture > TextureSP;

//�e�N�X�`���N���X
class CTexture{
private:
	//���O
	std::string	m_Name;

	//DirectX �e�N�X�`��
	LPDIRECT3DTEXTURE9 m_DirectxTex;

	//�Ǘ��}�b�v
	static std::map< std::string , TextureSP > m_Textures;

public:
	//�R���X�g���N�^
	CTexture(){}

	//�f�X�g���N�^
	~CTexture(){}

	//����
	//����1:�t�@�C���p�X
	static TextureSP Create( const std::string& path );

	//��̃e�N�X�`���쐬
	//����1:�� , ����2:����
	static TextureSP CreateEmptyTex( const DWORD w , const DWORD h );

	//�e�N�X�`���̉��( ���O���w�肵�Ȃ��ƑS����� )
	static void Destroy( const std::string name = "all" );

	//�e�N�X�`���Q�b�^�[
	LPDIRECT3DTEXTURE9 GetTex(){ return m_DirectxTex; }

	//�e�N�X�`���Z�b�^�[
	//tex: �e�N�X�`��
	void SetTex( const LPDIRECT3DTEXTURE9 tex )
	{
		//�e�N�X�`���̎Q�ƃJ�E���^�𑝂₵�A�Â��e�N�X�`�����J�����Ă���V�����e�N�X�`����ݒ�
		if( tex ) tex->AddRef();
		if( m_DirectxTex ) m_DirectxTex->Release();
		m_DirectxTex = tex;
	}
};
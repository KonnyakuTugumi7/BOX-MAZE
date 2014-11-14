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
	std::string	m_name;

	//DirectX �e�N�X�`��
	LPDIRECT3DTEXTURE9 m_directx_tex;

	//�Ǘ��}�b�v
	static std::map< std::string , TextureSP > m_textures;

public:
	//�R���X�g���N�^
	CTexture(){}

	//�f�X�g���N�^
	~CTexture(){}

	//����
	//����1:�t�@�C���p�X
	static TextureSP Create( const std::string& path );

	//��̃e�N�X�`���쐬(���d���[�h�̉\���A���Ȃ̂ŗv�C��)
	//����1:�� , ����2:����
	static TextureSP CreateEmptyTex( const DWORD w , const DWORD h );

	//�e�N�X�`���̉��( ���O���w�肵�Ȃ��ƑS����� )
	static void Destroy( const std::string name = "all" );

	//�e�N�X�`���Q�b�^�[
	LPDIRECT3DTEXTURE9 GetTex(){ return m_directx_tex; }
};
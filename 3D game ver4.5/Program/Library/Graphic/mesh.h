#pragma once 

#include "DXUT.h"
#include <string>
#include <map>
#include <memory>

class CMesh;
typedef std::shared_ptr< CMesh > MeshSP;

//���b�V���N���X
class CMesh{
private:
	//�e�N�X�`���̖��O
	std::string	m_Name;

	//DirectX ���b�V��
	LPD3DXMESH m_DirectxMesh;

	//�}�e���A���o�b�t�@
	LPD3DXBUFFER m_MaterialBuff;

	//�}�e���A���̐�
	DWORD m_MaterialNum;
	
	// �Ǘ��}�b�v
	static std::map< std::string , MeshSP > m_Meshs;

public:	
	//�R���X�g���N�^
	CMesh() : m_DirectxMesh( NULL ) ,
			  m_MaterialBuff( NULL )
	{}

	//�f�X�g���N�^
	~CMesh(){}

	//���b�V���𐶐�
	static MeshSP Create( const std::string& _path );

	//�Ǝ��̒��_�t�H�[�}�b�g(FVF)��p���ă��b�V���𐶐�
	static MeshSP CreateFromFVF( const std::string _path );

	// �o�^
	static MeshSP Regist( const std::string& m_path , MeshSP p_mesh );

	// �e�N�X�`���̉��( ���O���w�肵�Ȃ��ƑS����� )
	static void Destroy( const std::string _name = "all" );

	//���b�V���Q�b�^�[
	LPD3DXMESH GetMesh(){ return m_DirectxMesh; }

	//�}�e���A���o�b�t�@�Q�b�^�[
	LPD3DXBUFFER GetMaterialBuff(){ return m_MaterialBuff; }

	//�}�e���A�����Q�b�^�[
	DWORD GetMaterialNum(){ return m_MaterialNum; }

	//�}�e���A�����Z�b�^�[
	void SetMaterialNum( const DWORD num ){ m_MaterialNum = num; }

};
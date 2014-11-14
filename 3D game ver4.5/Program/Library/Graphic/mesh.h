#pragma once 

#include "DXUT.h"
#include <string>
#include <map>
#include <memory>

class CMesh;
typedef std::shared_ptr< CMesh > MeshSP;

//���b�V���N���X
class CMesh{
public:
	//�e�N�X�`���̖��O
	std::string	m_name;

	//DirectX ���b�V��
	LPD3DXMESH m_directx_mesh;

	//�}�e���A���o�b�t�@
	LPD3DXBUFFER m_material_buff;

	//�}�e���A���̐�
	DWORD m_material_num;
	
	// �Ǘ��}�b�v
	static std::map< std::string , MeshSP > m_meshs;

public:	
	//�R���X�g���N�^
	CMesh() : m_directx_mesh( NULL ) ,
			  m_material_buff( NULL )
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
	static void Destroy( const std::string m_name = "all" );

	//���b�V���Q�b�^�[
	LPD3DXMESH GetMesh(){ return m_directx_mesh; }

	//�}�e���A���o�b�t�@�Q�b�^�[
	LPD3DXBUFFER GetMaterialBuff(){ return m_material_buff; }

	//�}�e���A�����Q�b�^�[
	DWORD GetMaterialNum(){ return m_material_num; }

	//�}�e���A�����Z�b�^�[
	void SetMaterialNum( const DWORD num ){ m_material_num = num; }

};
#include "DXUT.h"
#include "mesh.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Graphic/vertex.h"

//mesh�N���X�̐ÓI�����o�̎���
std::map< std::string , MeshSP > CMesh::m_Meshs;

//���b�V���𐶐�
MeshSP CMesh::Create( const std::string& _path )
{
	// �Ǘ��}�b�v������Ƀ�������ɑ��݂��Ȃ����d���`�F�b�N
	std::map< std::string, MeshSP >::iterator it = m_Meshs.find( _path ) ;

	// ���݂��Ȃ���ΐV�������[�h
	if( it == m_Meshs.end() )
	{
		//���b�V��
		MeshSP meshSP = std::make_shared< CMesh >();
		meshSP->m_Name = _path ;
		WCHAR wc_buff[ 255 ] = { 0 } ;

		CCommon::DXconvAnsiToWide( wc_buff, _path.c_str(), 255 ) ;
		D3DXLoadMeshFromX( wc_buff, D3DXMESH_MANAGED, CGraphicsManager::m_pD3dDevice, NULL, &meshSP->m_MaterialBuff, NULL, &meshSP->m_MaterialNum, &meshSP->m_DirectxMesh ) ;

		// �Ǘ��}�b�v�ɓo�^
		m_Meshs.insert( std::make_pair( _path , meshSP ) ) ;

		return meshSP;
	}

	//���ɑ��݂���Ȃ烁�b�V����Ԃ�
	return it->second;
}

//�Ǝ��̒��_�t�H�[�}�b�g(FVF)��p���ă��b�V���𐶐�
MeshSP CMesh::CreateFromFVF( const std::string _path )
{
	// �Ǘ��}�b�v������Ƀ�������ɑ��݂��Ȃ����d���`�F�b�N
	std::map< std::string, MeshSP >::iterator it = m_Meshs.find( _path ) ;

	// ���݂��Ȃ���ΐV�������[�h
	if( it == m_Meshs.end() )
	{
		//���b�V��
		MeshSP meshSP = std::make_shared< CMesh >();
		meshSP->m_Name = _path;

		//���b�V������	  //���b�V���̖ʂ̐�	//���b�V���̒��_�̐�
		D3DXCreateMeshFVF( 2 , 4 , D3DXMESH_MANAGED , CVertex::FVF , CGraphicsManager::m_pD3dDevice , &meshSP->m_DirectxMesh );

		// �Ǘ��}�b�v�ɓo�^
		m_Meshs.insert( std::make_pair( _path , meshSP ) ) ;

		return meshSP;
	}

	//���ɑ��݂���Ȃ烁�b�V����Ԃ�
	return it->second;
}

// �o�^
MeshSP CMesh::Regist( const std::string& _path, MeshSP p_mesh )
{
	// �Ǘ��}�b�v������Ƀ�������ɑ��݂��Ȃ����d���`�F�b�N
	std::map< std::string, MeshSP >::iterator it = m_Meshs.find( _path ) ;

	// ���݂��Ȃ���ΐV�������[�h
	if( it == m_Meshs.end() )
	{
		// �Ǘ��}�b�v�ɓo�^
		m_Meshs.insert( std::make_pair( _path , p_mesh ) ) ;
	}

	return p_mesh;
}

// ���b�V���̉��
void CMesh::Destroy( const std::string _name )
{
	std::map< std::string, MeshSP >::iterator it = m_Meshs.begin() ;

	// �S�����
	if( _name == "all" )
	{
		while( it != m_Meshs.end() )
		{
			SAFE_RELEASE( (it->second)->m_DirectxMesh ) ;
			SAFE_RELEASE( (it->second)->m_MaterialBuff ) ;
			it->second.reset();
			m_Meshs.erase( it++ ) ;
		}
		//�}�b�v���N���A
		m_Meshs.clear();
	}//�e�N�X�`�����P�������
	else
	{
		it = m_Meshs.find( _name ) ;
		if( it != m_Meshs.end() )
		{
			SAFE_RELEASE( (it->second)->m_DirectxMesh ) ;
			SAFE_RELEASE( (it->second)->m_MaterialBuff ) ;
			it->second.reset();
		}
	}
}

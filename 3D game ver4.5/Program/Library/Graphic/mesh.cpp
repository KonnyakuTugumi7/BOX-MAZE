#include "DXUT.h"
#include "mesh.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Graphic/vertex.h"

//mesh�N���X�̐ÓI�����o�̎���
std::map< std::string , MeshSP > CMesh::m_meshs;

//���b�V���𐶐�
MeshSP CMesh::Create( const std::string& _path )
{
	// �Ǘ��}�b�v������Ƀ�������ɑ��݂��Ȃ����d���`�F�b�N
	std::map< std::string, MeshSP >::iterator it = m_meshs.find( _path ) ;

	// ���݂��Ȃ���ΐV�������[�h
	if( it == m_meshs.end() )
	{
		//���b�V��
		MeshSP meshSP = std::make_shared< CMesh >();
		meshSP->m_name = _path ;
		WCHAR wc_buff[ 255 ] = { 0 } ;

		CCommon::DXconvAnsiToWide( wc_buff, _path.c_str(), 255 ) ;
		D3DXLoadMeshFromX( wc_buff, D3DXMESH_MANAGED, CGraphicsManager::m_pd3dDevice, NULL, &meshSP->m_material_buff, NULL, &meshSP->m_material_num, &meshSP->m_directx_mesh ) ;

		// �Ǘ��}�b�v�ɓo�^
		m_meshs.insert( std::make_pair( _path , meshSP ) ) ;

		return meshSP;
	}

	//���ɑ��݂���Ȃ烁�b�V����Ԃ�
	return it->second;
}

//�Ǝ��̒��_�t�H�[�}�b�g(FVF)��p���ă��b�V���𐶐�
MeshSP CMesh::CreateFromFVF( const std::string _path )
{
	// �Ǘ��}�b�v������Ƀ�������ɑ��݂��Ȃ����d���`�F�b�N
	std::map< std::string, MeshSP >::iterator it = m_meshs.find( _path ) ;

	// ���݂��Ȃ���ΐV�������[�h
	if( it == m_meshs.end() )
	{
		//���b�V��
		MeshSP meshSP = std::make_shared< CMesh >();
		meshSP->m_name = _path;

		//���b�V������	  //���b�V���̖ʂ̐�	//���b�V���̒��_�̐�
		D3DXCreateMeshFVF( 2 , 4 , D3DXMESH_MANAGED , CVertex::FVF , CGraphicsManager::m_pd3dDevice , &meshSP->m_directx_mesh );

		// �Ǘ��}�b�v�ɓo�^
		m_meshs.insert( std::make_pair( _path , meshSP ) ) ;

		return meshSP;
	}

	//���ɑ��݂���Ȃ烁�b�V����Ԃ�
	return it->second;
}

// �o�^
MeshSP CMesh::Regist( const std::string& _path, MeshSP p_mesh )
{
	// �Ǘ��}�b�v������Ƀ�������ɑ��݂��Ȃ����d���`�F�b�N
	std::map< std::string, MeshSP >::iterator it = m_meshs.find( _path ) ;

	// ���݂��Ȃ���ΐV�������[�h
	if( it == m_meshs.end() )
	{
		// �Ǘ��}�b�v�ɓo�^
		m_meshs.insert( std::make_pair( _path , p_mesh ) ) ;
	}

	return p_mesh;
}

// ���b�V���̉��
void CMesh::Destroy( const std::string m_name )
{
	std::map< std::string, MeshSP >::iterator it = m_meshs.begin() ;

	// �S�����
	if( m_name == "all" )
	{
		while( it != m_meshs.end() )
		{
			SAFE_RELEASE( (it->second)->m_directx_mesh ) ;
			SAFE_RELEASE( (it->second)->m_material_buff ) ;
			it->second.reset();
			m_meshs.erase( it++ ) ;
		}
		//�}�b�v���N���A
		m_meshs.clear();
	}//�e�N�X�`�����P�������
	else
	{
		it = m_meshs.find( m_name ) ;
		if( it != m_meshs.end() )
		{
			SAFE_RELEASE( (it->second)->m_directx_mesh ) ;
			SAFE_RELEASE( (it->second)->m_material_buff ) ;
			it->second.reset();
		}
	}
}

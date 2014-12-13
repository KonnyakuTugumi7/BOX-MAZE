#include "DXUT.h"
#include "texture.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"

//texture�N���X�̐ÓI�����o�̎���
std::map< std::string, TextureSP > CTexture::m_Textures ;

//����
TextureSP CTexture::Create( const std::string& _path )
{
	//�Ǘ��}�b�v������Ƀ�������ɑ��݂��Ȃ����d���`�F�b�N
	std::map< std::string , TextureSP >::iterator it = m_Textures.find( _path );

	//���݂��Ȃ���ΐV�������[�h
	if( it == m_Textures.end() )
	{
		//�e�N�X�`��
		TextureSP texSP = std::make_shared< CTexture >();
		texSP->m_Name = _path;
		WCHAR wc_buff[ 255 ] = { 0 };
		CCommon::DXconvAnsiToWide( wc_buff , _path.c_str() , 255 );
		D3DXCreateTextureFromFile( CGraphicsManager::m_pD3dDevice , wc_buff , &texSP->m_DirectxTex );

		//�Ǘ��}�b�v�ɓo�^
		m_Textures.insert( std::make_pair( _path , texSP ) );

		return texSP;
	}

	//���ɑ��݂���Ȃ炻�̃e�N�X�`����Ԃ�
	return it->second;
}

//��̃e�N�X�`���쐬
TextureSP CTexture::CreateEmptyTex( const DWORD w , const DWORD h )
{
	//���O
	std::stringstream ss;
	ss << w << h;

	//�Ǘ��}�b�v������Ƀ�������ɑ��݂��Ȃ����d���`�F�b�N
	std::map< std::string , TextureSP >::iterator it = m_Textures.find( ss.str() );

	//���݂��Ȃ���ΐV�������[�h
	if( it == m_Textures.end() )
	{
		//�e�N�X�`��
		TextureSP texSP = std::make_shared< CTexture >();
		D3DXCreateTexture( CGraphicsManager::m_pD3dDevice , w , h , 0 , 0 , D3DFMT_A8R8G8B8 , D3DPOOL_MANAGED , &texSP->m_DirectxTex ); 
		texSP->m_Name = ss.str();
		//�Ǘ��}�b�v�ɓo�^
		m_Textures.insert( std::make_pair( ss.str() , texSP ) );

		return texSP;
	}

	//���ɑ��݂���Ȃ炻�̃e�N�X�`����Ԃ�
	return it->second;
}

//�e�N�X�`���̉��
void CTexture::Destroy( const std::string _name )
{
	std::map< std::string , TextureSP >::iterator it = m_Textures.begin();

	//�S�����
	if( _name == "all" )
	{
		while( it != m_Textures.end() )
		{
			SAFE_RELEASE( ( *it ).second->m_DirectxTex );
			( *it ).second.reset();
			++it;
		}
		//�}�b�v���N���A
		m_Textures.clear();
	}//�e�N�X�`�����P�������
	else
	{
		it = m_Textures.find( _name );
		if( it != m_Textures.end() )
		{
			SAFE_RELEASE( ( *it ).second->m_DirectxTex );
			( *it ).second.reset();
		}
	}
}


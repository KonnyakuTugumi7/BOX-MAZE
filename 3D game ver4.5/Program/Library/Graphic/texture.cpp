#include "DXUT.h"
#include "texture.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"

//texture�N���X�̐ÓI�����o�̎���
std::map< std::string, TextureSP > CTexture::m_textures ;

//����
TextureSP CTexture::Create( const std::string& _path )
{
	//�Ǘ��}�b�v������Ƀ�������ɑ��݂��Ȃ����d���`�F�b�N
	std::map< std::string , TextureSP >::iterator it = m_textures.find( _path );

	//���݂��Ȃ���ΐV�������[�h
	if( it == m_textures.end() )
	{
		//�e�N�X�`��
		TextureSP texSP = std::make_shared< CTexture >();
		texSP->m_name = _path;
		WCHAR wc_buff[ 255 ] = { 0 };
		CCommon::DXconvAnsiToWide( wc_buff , _path.c_str() , 255 );
		D3DXCreateTextureFromFile( CGraphicsManager::m_pd3dDevice , wc_buff , &texSP->m_directx_tex );

		//�Ǘ��}�b�v�ɓo�^
		m_textures.insert( std::make_pair( _path , texSP ) );

		return texSP;
	}

	//���ɑ��݂���Ȃ炻�̃e�N�X�`����Ԃ�
	return it->second;
}

//��̃e�N�X�`���쐬(���d���[�h�̉\���A���Ȃ̂ŗv�C��)

TextureSP CTexture::CreateEmptyTex( const DWORD w , const DWORD h )
{
	//�e�N�X�`��
	TextureSP texSP = std::make_shared< CTexture >();
	D3DXCreateTexture( CGraphicsManager::m_pd3dDevice , w , h , 0 , 0 , D3DFMT_A8R8G8B8 , D3DPOOL_MANAGED , &texSP->m_directx_tex ); 
	return texSP;
}

//�e�N�X�`���̉��
void CTexture::Destroy( const std::string _name )
{
	std::map< std::string , TextureSP >::iterator it = m_textures.begin();

	//�S�����
	if( _name == "all" )
	{
		while( it != m_textures.end() )
		{
			( *it ).second->m_directx_tex->Release();
			( *it ).second.reset();
			++it;
		}
		//�}�b�v���N���A
		m_textures.clear();
	}//�e�N�X�`�����P�������
	else
	{
		it = m_textures.find( _name );
		if( it != m_textures.end() )
		{
			SAFE_RELEASE( ( *it ).second->m_directx_tex );
			( *it ).second.reset();
		}
	}
}


#include "DXUT.h"
#include "xfile.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Graphic/mesh.h"
#include "../../Library/Graphic/texture.h"

//���f���쐬
XFileSP CXFile::Create( const std::string& _path )
{
	//���f��
	XFileSP model = std::make_shared< CXFile >();
	
	//���[�h
	model->Load( _path );

	//�����_�����O���X�g�ւ̓o�^
	CGraphicsManager::RegistObj( model , CGraphicsManager::NORMAL_3D );

	return model;
}

//���f�����[�h
void CXFile::Load( const std::string& _path )
{
	// X �t�@�C�� �ƃe�N�X�`���̃��[�h
	m_Mesh = CMesh::Create( _path ) ;

	// �}�e���A�����̎擾
	D3DXMATERIAL *p_materials = ( D3DXMATERIAL* )( m_Mesh->GetMaterialBuff()->GetBufferPointer() ) ;

	// �e�N�X�`���̃��[�h
	std::stringstream ss;
	WCHAR wc_buff[ 255 ] = { 0 };

	TextureSP tex;

	for( int i = 0 ; i < m_Mesh->GetMaterialNum() ; i++ )
	{
		memset( wc_buff , 0 , sizeof( WCHAR ) * 255 );
		ss.str("");
		if( NULL == p_materials[ i ].pTextureFilename )
		{
			tex.reset();
			m_Texs.push_back( NULL );
			continue;
		}
		ss << "Content/mesh/" << p_materials[ i ].pTextureFilename;

		// �Ǝ��e�N�X�`���N���X�Ƃ��Đ���
		std::string str = ss.str().c_str();
		tex = CTexture::Create( str );

		m_Texs.push_back( tex );
	}
}

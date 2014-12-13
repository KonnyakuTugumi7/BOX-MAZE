#include "DXUT.h"
#include "xfile.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Graphic/mesh.h"
#include "../../Library/Graphic/texture.h"

//モデル作成
XFileSP CXFile::Create( const std::string& _path )
{
	//モデル
	XFileSP model = std::make_shared< CXFile >();
	
	//ロード
	model->Load( _path );

	//レンダリングリストへの登録
	CGraphicsManager::RegistObj( model , CGraphicsManager::NORMAL_3D );

	return model;
}

//モデルロード
void CXFile::Load( const std::string& _path )
{
	// X ファイル とテクスチャのロード
	m_Mesh = CMesh::Create( _path ) ;

	// マテリアル情報の取得
	D3DXMATERIAL *p_materials = ( D3DXMATERIAL* )( m_Mesh->GetMaterialBuff()->GetBufferPointer() ) ;

	// テクスチャのロード
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

		// 独自テクスチャクラスとして生成
		std::string str = ss.str().c_str();
		tex = CTexture::Create( str );

		m_Texs.push_back( tex );
	}
}

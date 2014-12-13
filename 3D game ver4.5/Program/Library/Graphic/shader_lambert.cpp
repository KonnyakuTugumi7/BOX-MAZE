#include "DXUT.h"
#include "shader_lambert.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Support/common.h"

//コンストラクタ
//引数1:シェーダファイルパス
CShaderLambert::CShaderLambert( const std::string& file_pass )
{
	WCHAR wc_buff[ 255 ] = { 0 };
	CCommon::DXconvAnsiToWide( wc_buff, file_pass.c_str(), 255 );

	//シェーダファイルのロード
	D3DXCreateEffectFromFile( CGraphicsManager::m_pD3dDevice , wc_buff , NULL , NULL , D3DXSHADER_DEBUG , NULL , &m_Sdr , NULL );
	//テクニック
	m_SdrTechnique = m_Sdr->GetTechniqueByName( "TShader" );
	//テクスチャ
	m_SdrTex = m_Sdr->GetParameterByName( NULL , "tex0" );
	//カラー
	m_SdrColor = m_Sdr->GetParameterByName( NULL , "Color" );
	//ワールド x ビュー x プロジェクション 行列
	m_SdrWvmp = m_Sdr->GetParameterByName( NULL , "WVP" );
}
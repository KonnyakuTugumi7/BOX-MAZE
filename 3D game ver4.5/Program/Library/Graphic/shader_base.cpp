#include "DXUT.h"
#include "shader_base.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Support/common.h"

//コンストラクタ
CShaderBase::CShaderBase( const std::string& file_pass )
{
	WCHAR wc_buff[ 255 ] = { 0 };
	CCommon::DXconvAnsiToWide( wc_buff, file_pass.c_str(), 255 );

	//シェーダファイルのロード
	D3DXCreateEffectFromFile( CGraphicsManager::m_pd3dDevice , wc_buff , NULL , NULL , D3DXSHADER_DEBUG , NULL , &m_sdr , NULL );
	//テクニック
	m_sdr_technique = m_sdr->GetTechniqueByName( "TShader" );
	//テクスチャ
	m_sdr_tex = m_sdr->GetParameterByName( NULL , "SrcTex" );
	//カラー
	m_sdr_color = m_sdr->GetParameterByName( NULL , "Color" );
	//ワールド x ビュー x プロジェクション 行列
	m_sdr_wvmp = m_sdr->GetParameterByName( NULL , "WVP" );
}
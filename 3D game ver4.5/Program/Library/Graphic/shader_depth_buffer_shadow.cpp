#include "DXUT.h"
#include "shader_depth_buffer_shadow.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Support/common.h"

//コンストラクタ
CShaderDepthBufferShadow::CShaderDepthBufferShadow( const std::string& file_pass )
{
	WCHAR wc_buff[ 255 ] = { 0 };
	CCommon::DXconvAnsiToWide( wc_buff, file_pass.c_str(), 255 );

	//シェーダファイルのロード
	HRESULT hr = D3DXCreateEffectFromFile( CGraphicsManager::m_pd3dDevice , wc_buff , NULL , NULL , D3DXSHADER_DEBUG , NULL , &m_sdr , NULL );

	if( SUCCEEDED(hr) )
	{
		//テクニック
		m_sdr_technique = m_sdr->GetTechniqueByName( "TShader" );
		//テクスチャ0
		m_sdr_tex = m_sdr->GetParameterByName( NULL , "tex0" );
		//カラー
		m_sdr_color = m_sdr->GetParameterByName( NULL , "Color" );
		//ワールド x ビュー x プロジェクション 行列
		m_sdr_wvmp = m_sdr->GetParameterByName( NULL , "WVP" );
		//ワールド x ビュー x プロジェクション 行列
		m_LWVP        = m_sdr->GetParameterByName( NULL, "m_LWVP" );
		//ワールド x ビュー x プロジェクション 行列 x テクスチャ
		m_LWVPT       = m_sdr->GetParameterByName( NULL, "m_LWVPT" );
		//テクスチャ1
		m_dep_tex = m_sdr->GetParameterByName( NULL , "tex1" );
		//ライトの方向
		m_LightDir    = m_sdr->GetParameterByName( NULL, "m_LightDir" );
		//アンビエント色
		m_Ambient     = m_sdr->GetParameterByName( NULL, "m_Ambient" );
		//バイアス値
		m_Bias        = m_sdr->GetParameterByName( NULL, "m_Bias" );
		//影の色
		m_ShadowColor = m_sdr->GetParameterByName( NULL, "m_ShadowColor" );
	}
}

//シェーダ開始
void CShaderDepthBufferShadow::Begin()
{
	CGraphicsManager::m_pd3dDevice->GetTransform( D3DTS_VIEW , &m_matProj );
	CGraphicsManager::m_pd3dDevice->GetTransform( D3DTS_PROJECTION, &m_matProj );
	if( m_sdr ) m_sdr->Begin( NULL , 0 ); 
}

//アンビエント色セット
//引数1:アンビエント色
void CShaderDepthBufferShadow::SetAmbient( const float ambient )
{
	if( m_sdr )
	{
		D3DXVECTOR4 A;
		A = D3DXVECTOR4( ambient, ambient, ambient, 1.0f );
		m_sdr->SetVector( m_Ambient, &A );
	}
	else
	{
		D3DMATERIAL9 old_material;
		CGraphicsManager::m_pd3dDevice->GetMaterial( &old_material );
		old_material.Ambient.r = ambient;
		old_material.Ambient.g = ambient;
		old_material.Ambient.b = ambient;
		old_material.Ambient.a = 1.0f;
		CGraphicsManager::m_pd3dDevice->SetMaterial( &old_material );
	}
}
//引数1:アンビエント色
void CShaderDepthBufferShadow::SetAmbient( const D3DXVECTOR4& ambient )
{
	if( m_sdr )
	{
		m_sdr->SetVector( m_Ambient, &ambient );
	}
	else
	{
		D3DMATERIAL9 old_material;
		CGraphicsManager::m_pd3dDevice->GetMaterial( &old_material );
		old_material.Ambient.r = ambient.x;
		old_material.Ambient.g = ambient.y;
		old_material.Ambient.b = ambient.z;
		old_material.Ambient.a = ambient.w;
		CGraphicsManager::m_pd3dDevice->SetMaterial( &old_material );
	}
}

//行列設定
//引数1:ワールド行列 , 引数2:ライト , 引数3:ライトの方向
void CShaderDepthBufferShadow::SetMatrix( const D3DXMATRIX& world , const D3DXMATRIX& light , const D3DXVECTOR4& light_dir )
{
	if( m_sdr )
	{
		D3DXMATRIX m , m1 , m2;
		D3DXVECTOR4 LightDir;
		D3DXVECTOR4 v;

		//カメラ基準の行列変換マトリックスをセットする
		m = world * m_matView * m_matProj;
		m_sdr->SetMatrix( m_sdr_wvmp , &m );

		//ライトの方向ベクトルを計算
		LightDir = light_dir;
		D3DXMatrixInverse( &m1 , NULL , &world );
		D3DXVec4Transform( &v, &LightDir , &m1 );
		//XYZ成分について正規化する
		D3DXVec3Normalize( (D3DXVECTOR3*)&v, (D3DXVECTOR3*)&v );
		m_sdr->SetVector( m_LightDir , &v );

		//ライト基準の行列変換マトリックスをセットする
		m = world * light;
		m_sdr->SetMatrix( m_LWVP , &m );

		//ライト基準の行列変換マトリックスをテクスチャー座標系へ変換
		D3DXMatrixScaling( &m1 , 0.5f , -0.5f , 1.0f );
		D3DXMatrixTranslation( &m2 , 0.5f , 0.5f , 0.0f );
		m = m * m1 * m2;
		m_sdr->SetMatrix( m_LWVPT, &m );
	}
	else
	{
		CGraphicsManager::m_pd3dDevice->SetTransform( D3DTS_WORLD , &world );
	}
}
#pragma once

#include "DXUT.h"
#include "../../Library/Graphic/shader_base.h"
#include <string>
#include <memory>

class CShaderDepthBufferShadow;
typedef std::shared_ptr< CShaderDepthBufferShadow > ShaderDepthBufferShadowSP;

class CShaderDepthBufferShadow : public CShaderBase{
public:
	//----------------------ハンドル----------------------
	//ワールド座標系 × ビュー(ライト基準)座標系 × 正射影座標系
	D3DXHANDLE m_LWVP;

	//ワールド座標系 × ビュー(ライト基準)座標系 × 正射影座標系 × テクスチャー座標系
	D3DXHANDLE m_LWVPT;

	//デプスバッファサーフェイスのテクスチャー
	D3DXHANDLE m_dep_tex;

	//ライトの方向
	D3DXHANDLE m_LightDir;

	//アンビエント色
	D3DXHANDLE m_Ambient;

	//バイアス値(Z値の比較による誤差を補正す)
	D3DXHANDLE m_Bias;

	//影の色(影によるメッシュのカラーの減算値)
	D3DXHANDLE m_ShadowColor;
	//----------------------------------------------------

	//ビュー行列
	D3DXMATRIX m_matView;

	//プロジェクション行列
	D3DXMATRIX m_matProj;

public:
	//コンストラクタ
	//引数1:ファイルパス
	CShaderDepthBufferShadow( const std::string& file_pass );

	//デストラクタ
	~CShaderDepthBufferShadow(){}

	//生成
	//引数1:ファイルパス
	static ShaderDepthBufferShadowSP Create( const std::string& file_pass ){ return std::make_shared< CShaderDepthBufferShadow >( file_pass ); }

	//シェーダ開始
	virtual void Begin();

	//アンビエント色セット
	//引数1:アンビエント色
	void SetAmbient( const float ambient );

	//引数1:アンビエント色
	void SetAmbient( const D3DXVECTOR4& ambient );

	//行列設定
	//引数1:ワールド行列 , 引数2:ライト , 引数3:ライトの方向
	void SetMatrix( const D3DXMATRIX& world , const D3DXMATRIX& light , const D3DXVECTOR4& light_dir );

	//Z値の比較によって発生する計算誤差を軽減するためのバイアス値
	//引数1:バイアス値
	void SetBias( const float& bias )
	{
		if( m_sdr )
		{
			m_sdr->SetFloat( m_Bias , bias );
		}
	}

	//影によるメッシュのカラーの減算値
	//引数1:影の色(1.0fで完全に黒になる)
	void SetShadowColor( const float& shadow_color )
	{
		if( m_sdr )
		{
			m_sdr->SetFloat( m_ShadowColor , shadow_color );
		}
	}
};
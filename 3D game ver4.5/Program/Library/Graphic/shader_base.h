#pragma once

#include "DXUT.h"
#include <string>
#include <memory>

class CShaderBase;
typedef std::shared_ptr< CShaderBase > ShaderBaseSP;

//シェーダ基本クラス
class CShaderBase{
public:
	//シェーダ本体
	LPD3DXEFFECT m_Sdr;

	//-----------ハンドル-----------
	//テクニック
	D3DXHANDLE m_SdrTechnique;

	//テクスチャ
	D3DXHANDLE m_SdrTex;

	//カラー
	D3DXHANDLE m_SdrColor;
	//------------------------------

	//ワールド x ビュー x プロジェクション行列
	D3DXHANDLE m_SdrWvmp;

public:
	//コンストラクタ
	CShaderBase(){}

	//コンストラクタ
	//引数1:シェーダファイルパス
	CShaderBase( const std::string& file_pass );

	//デストラクタ
	virtual ~CShaderBase()
	{
		//シェーダ解放
		SAFE_RELEASE( m_Sdr );
	}

	//生成
	//引数1:ファイルパス
	static ShaderBaseSP Create( const std::string& file_pass ){ return std::make_shared< CShaderBase >( file_pass ); }

	//シェーダ開始
	virtual void Begin(){ if( m_Sdr ) m_Sdr->Begin( NULL , 0 ); }

	//シェーダ終了
	void End(){ if( m_Sdr ) m_Sdr->End(); }

	//シェーダパス開始
	void BeginPass( const UINT pass ){ if( m_Sdr ) m_Sdr->BeginPass( pass ); }

	//シェーダパス終了
	void EndPass(){ if( m_Sdr ) m_Sdr->EndPass(); }
};
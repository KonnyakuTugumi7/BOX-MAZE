#pragma once

#include "DXUT.h"
#include "../../Library/Graphic/shader_base.h"
#include <string>
#include <memory>

class CShaderLambert;
typedef std::shared_ptr< CShaderLambert > ShaderLambertSP;

//ランバート照明は視線方向に依存しない光源の入射方向と面の法線だけで算出される陰影処理技法・・・らしい
//ここではメッシュをライト位置からながめて、Z値を取得する処理
class CShaderLambert : public CShaderBase{
public:
	//コンストラクタ
	//引数1:シェーダファイルパス
	CShaderLambert( const std::string& file_pass );

	//デストラクタ
	~CShaderLambert(){}

	//生成
	//引数1:シェーダファイルパス
	static ShaderLambertSP Create( const std::string& file_pass ){ return std::make_shared< CShaderLambert >( file_pass ); }
};
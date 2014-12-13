#pragma once

#include "DXUT.h"
#include "../../Library/Graphic/shader_base.h"
#include <string>
#include <memory>

class CShaderDirectionalLight;
typedef std::shared_ptr< CShaderDirectionalLight > ShaderDirectionalLightSP;

//平行光源シェーダクラス
class CShaderDirectionalLight : public CShaderBase{
public:
	//--------------ハンドル--------------
	//平行光源の向き
	D3DXHANDLE m_DirLight;

	//回転行列
	D3DXHANDLE m_RotMtx;

	//アンビエント色
	D3DXHANDLE m_Ambient;
	//------------------------------------

	//平行光源の向き
	D3DXVECTOR4 m_Dir;

public:
	//コンストラクタ
	//引数1:ファイルパス
	CShaderDirectionalLight( const std::string& file_pass );

	//デストラクタ
	~CShaderDirectionalLight(){}

	//生成
	//引数1:ファイルパス
	static ShaderDirectionalLightSP Create( const std::string& file_pass ){ return std::make_shared< CShaderDirectionalLight >( file_pass ); }
};
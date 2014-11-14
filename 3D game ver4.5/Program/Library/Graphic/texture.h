#pragma once 

#include "DXUT.h"
#include <string>
#include <map>
#include <memory>

class CTexture;
typedef std::shared_ptr< CTexture > TextureSP;

//テクスチャクラス
class CTexture{
private:
	//名前
	std::string	m_name;

	//DirectX テクスチャ
	LPDIRECT3DTEXTURE9 m_directx_tex;

	//管理マップ
	static std::map< std::string , TextureSP > m_textures;

public:
	//コンストラクタ
	CTexture(){}

	//デストラクタ
	~CTexture(){}

	//生成
	//引数1:ファイルパス
	static TextureSP Create( const std::string& path );

	//空のテクスチャ作成(多重ロードの可能性アリなので要修正)
	//引数1:幅 , 引数2:高さ
	static TextureSP CreateEmptyTex( const DWORD w , const DWORD h );

	//テクスチャの解放( 名前を指定しないと全部解放 )
	static void Destroy( const std::string name = "all" );

	//テクスチャゲッター
	LPDIRECT3DTEXTURE9 GetTex(){ return m_directx_tex; }
};
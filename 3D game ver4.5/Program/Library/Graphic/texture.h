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
	std::string	m_Name;

	//DirectX テクスチャ
	LPDIRECT3DTEXTURE9 m_DirectxTex;

	//管理マップ
	static std::map< std::string , TextureSP > m_Textures;

public:
	//コンストラクタ
	CTexture(){}

	//デストラクタ
	~CTexture(){}

	//生成
	//引数1:ファイルパス
	static TextureSP Create( const std::string& path );

	//空のテクスチャ作成
	//引数1:幅 , 引数2:高さ
	static TextureSP CreateEmptyTex( const DWORD w , const DWORD h );

	//テクスチャの解放( 名前を指定しないと全部解放 )
	static void Destroy( const std::string name = "all" );

	//テクスチャゲッター
	LPDIRECT3DTEXTURE9 GetTex(){ return m_DirectxTex; }

	//テクスチャセッター
	//tex: テクスチャ
	void SetTex( const LPDIRECT3DTEXTURE9 tex )
	{
		//テクスチャの参照カウンタを増やし、古いテクスチャを開放してから新しいテクスチャを設定
		if( tex ) tex->AddRef();
		if( m_DirectxTex ) m_DirectxTex->Release();
		m_DirectxTex = tex;
	}
};
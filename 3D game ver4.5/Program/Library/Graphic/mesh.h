#pragma once 

#include "DXUT.h"
#include <string>
#include <map>
#include <memory>

class CMesh;
typedef std::shared_ptr< CMesh > MeshSP;

//メッシュクラス
class CMesh{
private:
	//テクスチャの名前
	std::string	m_Name;

	//DirectX メッシュ
	LPD3DXMESH m_DirectxMesh;

	//マテリアルバッファ
	LPD3DXBUFFER m_MaterialBuff;

	//マテリアルの数
	DWORD m_MaterialNum;
	
	// 管理マップ
	static std::map< std::string , MeshSP > m_Meshs;

public:	
	//コンストラクタ
	CMesh() : m_DirectxMesh( NULL ) ,
			  m_MaterialBuff( NULL )
	{}

	//デストラクタ
	~CMesh(){}

	//メッシュを生成
	static MeshSP Create( const std::string& _path );

	//独自の頂点フォーマット(FVF)を用いてメッシュを生成
	static MeshSP CreateFromFVF( const std::string _path );

	// 登録
	static MeshSP Regist( const std::string& m_path , MeshSP p_mesh );

	// テクスチャの解放( 名前を指定しないと全部解放 )
	static void Destroy( const std::string _name = "all" );

	//メッシュゲッター
	LPD3DXMESH GetMesh(){ return m_DirectxMesh; }

	//マテリアルバッファゲッター
	LPD3DXBUFFER GetMaterialBuff(){ return m_MaterialBuff; }

	//マテリアル数ゲッター
	DWORD GetMaterialNum(){ return m_MaterialNum; }

	//マテリアル数セッター
	void SetMaterialNum( const DWORD num ){ m_MaterialNum = num; }

};
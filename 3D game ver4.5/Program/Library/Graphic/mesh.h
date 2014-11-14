#pragma once 

#include "DXUT.h"
#include <string>
#include <map>
#include <memory>

class CMesh;
typedef std::shared_ptr< CMesh > MeshSP;

//メッシュクラス
class CMesh{
public:
	//テクスチャの名前
	std::string	m_name;

	//DirectX メッシュ
	LPD3DXMESH m_directx_mesh;

	//マテリアルバッファ
	LPD3DXBUFFER m_material_buff;

	//マテリアルの数
	DWORD m_material_num;
	
	// 管理マップ
	static std::map< std::string , MeshSP > m_meshs;

public:	
	//コンストラクタ
	CMesh() : m_directx_mesh( NULL ) ,
			  m_material_buff( NULL )
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
	static void Destroy( const std::string m_name = "all" );

	//メッシュゲッター
	LPD3DXMESH GetMesh(){ return m_directx_mesh; }

	//マテリアルバッファゲッター
	LPD3DXBUFFER GetMaterialBuff(){ return m_material_buff; }

	//マテリアル数ゲッター
	DWORD GetMaterialNum(){ return m_material_num; }

	//マテリアル数セッター
	void SetMaterialNum( const DWORD num ){ m_material_num = num; }

};
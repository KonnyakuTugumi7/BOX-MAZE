#pragma once

#include "DXUT.h"
#include "../../Library/Support/csv_platinum.h"
#include "../../GameEngine/Graphic/cube.h"
#include <memory>
#include <vector>

#pragma warning( disable:4482 ) //非標準の拡張機能が使用されています: enum 'CCubeManager::CUBE_STATE' が限定名で使用されます
#pragma warning( disable:4244 ) //'int' から 'FLOAT' への変換です。データが失われる可能性があります。

class CCubeManager;
typedef std::shared_ptr< CCubeManager > CubeManagerSP;

//typedef std::vector< CubeSP > CUBE1;
//typedef std::vector< std::vector< CubeSP > > CUBE2;
//typedef std::vector< std::vector< std::vector< CubeSP > > > CUBE3;
typedef std::vector< CubeSP > CUBE1;
typedef std::vector< CUBE1 > CUBE2;
typedef std::vector< CUBE2 > CUBE3;

//キューブマネージャ
class CCubeManager{
private:
	//現在のステージ名(全ステージクリア判定で使う)
	std::string m_stage_name;

	//スタートキューブの座標保存用
	D3DXVECTOR3 m_start_pos;

	//ゴールキューブの座標保存用
	D3DXVECTOR3 m_goal_pos;

public:
	//キューブの３次元配列
	CUBE3 m_cubes;

	std::vector< std::vector<std::vector< D3DXVECTOR3 > > > m_cubes_pos;

	//CSVPlatinum
	CsvPlatinumSP m_csv_platinum;

	//キューブの種類
	enum CUBE_STATE{
		NON = 0 ,
		START = 1 ,
		GOAL = 2 ,
		NORMAL = 3 ,
		CUBE_STATE_NUM = 4
	};

public:
	//コンストラクタ
	//引数1:ファイルパス
	CCubeManager( const std::string file_path );

	//デストラクタ
	~CCubeManager(){};

	//生成
	//引数1:ファイルパス
	static CubeManagerSP Create( const std::string file_path ){ return std::make_shared< CCubeManager >( file_path ); }

	//指定した種類のキューブがいくつあるか検索
	int FindSpecifiedCubeNum( const CUBE_STATE cube );

	//ステージ名ゲッター
	std::string GetStageName(){ return m_stage_name; }

	//スタートキューブ座標ゲッター
	D3DXVECTOR3 GetStartPos(){ return m_start_pos; }

	//ゴールキューブ座標ゲッター
	D3DXVECTOR3 GetGoalPos(){ return m_goal_pos; }
};
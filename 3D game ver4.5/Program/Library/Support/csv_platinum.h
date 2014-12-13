#pragma once

#include "../../Library/Support/csv.h"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <memory>

class CCsvPlatinum;
typedef std::shared_ptr< CCsvPlatinum > CsvPlatinumSP;

//CSV_Platinumクラス(CSVのPlatinum専用クラス)
//CSVファイル毎に必要になる(使いまわすなら逐一CSVファイルを読み込むべし)
//まずコンストラクタ(またはImportFile関数)でCSVファイルを読み込む
//テーブルに格納されたトークンを色々する
//書き換えた値をExportFile関数で書き出す
class CCsvPlatinum : public CCsv{
private:
	//マップの横幅
	int m_Width;

	//マップの奥行
	int m_Depth;

	//マップチップ1つの幅(pixel)
	int m_ChipWidth;

	//マップチップ1つの高さ(pixel)
	int m_ChipHeight;

	//レイヤーの数
	int m_LayerNum;

	//レイヤーインデックス
	//レイヤー0を抜かしたインデックスが昇順で格納されていく
	std::vector< int > m_LayerIdx;

public:
	//コンストラクタ(新しくステージを作るとき)
	CCsvPlatinum(){}

	//コンストラクタ(既にあるステージの時)
	//引数1:ファイルパス
	CCsvPlatinum( const std::string& file_path )
	{
		//ファイルパス
		m_Path = file_path;

		//CSVファイル読み込み
		ImportFile( file_path );

		//マップの幅
		m_Width = GetToken< int >( 0 , 0 );

		//マップの奥行
		m_Depth = GetToken< int >( 0 , 1 );

		//レイヤーの数
		m_LayerNum = GetToken< int >( 0 , 4 );

		for( int i = 0 ; i < GetNumLine() ; i++ )
		{
			if( GetToken< std::string >( i , 0 ) == "" )
			{
				m_LayerIdx.push_back( i );
			}
		}
	}

	//デストラクタ
	~CCsvPlatinum()
	{
		//解放
		EraseToken();
		m_LayerIdx.clear();
	}

	//生成
	//引数1:ファイルパス
	static CsvPlatinumSP Create( const std::string& file_path ){ return std::make_shared< CCsvPlatinum >( file_path ); }

	//トークン検索(指定のトークンがいくつ存在するか)
	//引数1:検索するトークン
	//戻り値:指定のトークンの数
	virtual int FindSpecifiedTokenNum( const std::string token )
	{
		int num = 0;
		for( int line = 1 ; line < GetNumLine() ; ++line ) num += std::count( m_CsvTable[ line ].begin() , m_CsvTable[ line ].end() , token );
		
		return num;
	}

	//レイヤーインデックスゲッター
	//引数1:レイヤー番号
	inline int GetLayerIdx( const int layer = 0 )
	{
		//レイヤーは空白で区切ってるのでレイヤー0のインデックスは保持してない
		//よってレイヤー0だけ例外でベタ打ち
		if( layer == 0 )
		{
			return 1;
		}
		else
		{
			return m_LayerIdx[ layer - 1 ] + 1;
		}
	}

	//マップ幅ゲッター
	inline int GetMapWidth(){ return m_Width; }

	//マップ奥行ゲッター
	inline int GetMapDepth(){ return m_Depth; }

	//レイヤー数ゲッター
	inline int GetLayerNum(){ return m_LayerNum; }
};
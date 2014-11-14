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
	int m_width;

	//マップの奥行
	int m_depth;

	//マップチップ1つの幅(pixel)
	int m_chip_width;

	//マップチップ1つの高さ(pixel)
	int m_chip_height;

	//レイヤーの数
	int m_layer_num;

	//レイヤーインデックス
	//レイヤー0を抜かしたインデックスが昇順で格納されていく
	std::vector< int > m_layer_idx;

public:
	////コンストラクタ
	//CCsvPlatinum() : m_width( 0 ) , 
	//				 m_depth( 0 ) , 
	//				 m_chip_width( 0 ) , 
	//				 m_chip_height( 0 ) , 
	//				 m_layer_num( 0 )
	//{
	//	m_path = ""; 
	//}

	//コンストラクタ
	//引数1:ファイルパス
	CCsvPlatinum( const std::string& file_path )
	{
		//ファイルパス
		m_path = file_path;

		//CSVファイル読み込み
		ImportFile( file_path );

		//マップの幅
		m_width = GetToken< int >( 0 , 0 );

		//マップの奥行
		m_depth = GetToken< int >( 0 , 1 );

		//レイヤーの数
		m_layer_num = GetToken< int >( 0 , 4 );

		for( int i = 0 ; i < GetNumLine() ; i++ )
		{
			if( GetToken< std::string >( i , 0 ) == "" )
			{
				m_layer_idx.push_back( i );
			}
		}
	}

	//デストラクタ
	~CCsvPlatinum()
	{
		//解放
		EraseToken();
		m_layer_idx.clear();
	}

	//生成
	//引数1:ファイルパス
	static CsvPlatinumSP Create( const std::string& file_path ){ return std::make_shared< CCsvPlatinum >( file_path ); }

	////トークンセッター
	////引数1:行 , 引数2:列 , 引数3:レイヤー番号 引数4:トークン
	//void SetToken( const int line , const int row , const int layer , const std::string& token )
	//{
	//	int layer_idx = GetLayerIdx( layer );

	//	//指定セルが正しいかどうか
	//	if( !IsValidPos( line , layer_idx + row ) )
	//	{
	//		//セルを確保
	//		//現在のラインより指定ラインが大きかったら
	//		if( m_csv_table.size() < line + 1 ) m_csv_table.resize( ( line + 1 ) );
	//		m_csv_table[ line ].resize( layer_idx + row + 1 );
	//	}

	//	//イテレータ
	//	std::vector< std::string >::iterator it = m_csv_table[ line ].begin() + layer_idx + row;

	//	//トークンをセット
	//	m_csv_table[ line ].insert( it , 1 , token );

	//	//std::vectorのinsert関数はイテレータが指す場所の直前に値を挿入するので
	//	//挿入されたセルの1つ次のセルがにエンプティートークンが入っていたら削除する
	//	std::string empty = m_csv_table[ line ][ layer_idx + row + 1 ];
	//	if( empty == "" )
	//	{
	//		EraseToken( line , row + 1 , layer );
	//	}
	//}

	////トークンオールイレーサー
	//void EraseToken()
	//{
	//	m_csv_table.clear();
	//}

	////トークンイレーサー
	////引数1:行 , 引数2:列 , 引数3:レイヤー番号
	//void EraseToken( const int line , const int row , const int layer )
	//{
	//	int layer_idx = GetLayerIdx( layer );

	//	//イテレータ
	//	std::vector< std::string >::iterator it = m_csv_table[ line ].begin() + layer_idx + row;

	//	//削除
	//	it = m_csv_table[ line ].erase( it );
	//}

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
			return m_layer_idx[ layer - 1 ] + 1;
		}
	}

	//マップ幅ゲッター
	inline int GetMapWidth(){ return m_width; }

	//マップ奥行ゲッター
	inline int GetMapDepth(){ return m_depth; }

	//レイヤー数ゲッター
	inline int GetLayerNum(){ return m_layer_num; }
};
#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <memory>
#include <map>

#pragma warning( disable : 4018 )
#pragma warning( disable : 4127 )

class CCsv;
typedef std::shared_ptr< CCsv > CsvSP;

//CSVクラス
//CSVファイル毎に必要になる(使いまわすなら逐一CSVファイルを読み込むべし)
//まずコンストラクタ(またはImportFile関数)でCSVファイルを読み込む
//テーブルに格納されたトークンを色々する
//書き換えた値をExportFile関数で書き出す
//CSVファイルの//を入れた行はコメント扱いとなり、データテーブルには読み込まれない
//ただし、コメントは対応する値の1つ上の行に書くべし
class CCsv{
protected:
	//ファイルパス
	std::string m_Path;

	//CSVファイルのデータを格納するテーブル
	std::vector< std::vector< std::string > > m_CsvTable;

	//CSVファイルのコメントを格納するマップ
	//key:データテーブルの何行目の値に対応するか(コメントは除く) , value:コメント
	std::map< int , std::string > m_CommentMap;

public:
	//コンストラクタ
	CCsv(): m_Path( "" ){}

	//コンストラクタ
	//引数1:ファイルパス
	CCsv( const std::string& file_path ) : m_Path( file_path )
	{
		//CSVファイル読み込み
		ImportFile( file_path );
	}

	//デストラクタ
	virtual ~CCsv()
	{
		//トークンテーブル解放
		EraseToken();

		//コメント解放
		m_CommentMap.clear();
	}

	//生成
	//引数1:ファイルパス
	static CsvSP Create( const std::string& file_path ){ return std::make_shared< CCsv >( file_path ); }
	
	//CSVファイル読み込み
	//引数1:ファイルパス
	virtual void ImportFile( const std::string& file_path );

	//CSVファイル書き出し
	void ExportFile();

	//ファイルが読み込まれたかどうか
	//戻り値:ファイル読込済み:true , ファイル未読込:false
	bool IsImportFile() const
	{
		//文字列を比較して空かどうか調べる
		if( strcmp( m_Path.c_str() , "" ) == 0 ) return false;

		return true;
	}

	//セルの有効無効
	//引数1:行 , 引数2:列
	//戻り値:有効なセル指定:true , 無効なセル指定:false
	bool IsValidPos( const int line , const int row ) const
	{
		//読み込まれているかどうか
		if( !IsImportFile() ) return false;

		//トークンテーブルが空かどうか
		if( m_CsvTable.empty() ) return false;

		//指定の行が有効かどうか
		if( ( line < 0 ) || ( GetNumLine() <= line ) ) return false;

		//指定の列が有効かどうか
		if( ( row < 0 ) || ( ( int ) m_CsvTable[ line ].size() <= row ) ) return false;

		return true;
	}

	//トークン検索(指定のトークンがいくつ存在するか)
	//引数1:検索するトークン
	//戻り値:指定のトークンの数
	virtual int FindSpecifiedTokenNum( const std::string token )
	{
		int num = 0;
		for( int line = 0 ; line < GetNumLine() ; ++line ) num += std::count( m_CsvTable[ line ].begin() , m_CsvTable[ line ].end() , token );
		return num;
	}

	//トークンゲッター
	//引数1:行 , 引数2:列
	//戻り値:トークン
	template < typename T >
	T GetToken( const int line , const int row ) const
	{
		//現在int,float,stringにしか対応していないのでそれ以外の時はassertを出す
		assert( 0 );
		return;
	}
	//int用
	template <>
	int GetToken< int >( const int line , const int row ) const
	{
		//指定セルが正しいかどうか
		if( !IsValidPos( line , row ) ) assert( 0 );

		//テーブルのトークンをint型に変換して取得
		return strtol( ( const char* )m_CsvTable[ line ][ row ].c_str() , NULL , 10 );
	}
	//float用
	template <>
	float GetToken< float >( const int line , const int row ) const
	{
		//指定セルが正しいかどうか
		if( !IsValidPos( line , row ) ) assert( 0 );

		//テーブルのトークンをfloat型に変換して取得
		return ( float )strtod( ( const char* )m_CsvTable[ line ][ row ].c_str() , NULL );
	}
	//string用
	template<>
	std::string GetToken< std::string >( const int line , const int row ) const
	{
		//指定セルが正しいかどうか
		if( !IsValidPos( line , row ) ) assert( 0 );

		//テーブルのトークンを取得
		return m_CsvTable[ line ][ row ];
	}

	//トークンサイズゲッター
	//引数1:行 , 引数2:列
	//戻り値:有効なセル指定:セル番地 , 無効なセル指定:-1
	int GetTokenSize( const int line , const int row ) const
	{
		//指定セルが正しいかどうか
		if( !IsValidPos( line , row ) ) return -1;

		return ( int )m_CsvTable[ line ][ row ].size();
	}

	//列ゲッター
	//引数1:列
	//戻り値:有効なセル指定:指定行の列数 , 無効なセル指定:-1
	int GetNumRow( const int line ) const
	{
		//指定セルが正しいかどうか
		if( !IsValidPos( line , 0 ) ) return -1;

		return ( int )m_CsvTable[ line ].size();
	}

	//行ゲッター
	//戻り値:行数
	int GetNumLine() const
	{
		return ( int )m_CsvTable.size();
	}

	//トークンセッター
	//引数1:行 , 引数2:列 , 引数3:トークン
	void SetToken( const int line , const int row , const std::string& token )
	{
		//指定セルが正しいかどうか
		if( !IsValidPos( line , row ) )
		{
			//セルを確保
			//現在のラインより指定ラインが大きかったら
			if( m_CsvTable.size() < line + 1 ) m_CsvTable.resize( ( line + 1 ) );
			m_CsvTable[ line ].resize( row + 1 );
		}

		//イテレータ
		std::vector< std::string >::iterator it = m_CsvTable[ line ].begin() + row;

		//トークンをセット
		m_CsvTable[ line ].insert( it , 1 , token );

		//std::vectorのinsert関数はイテレータが指す場所の直前に値を挿入するので
		//挿入されたセルの1つ次のセルがにエンプティートークンが入っていたら削除する
		std::string empty = m_CsvTable[ line ][ row + 1 ];
		if( empty == "" )
		{
			EraseToken( line , row + 1 );
		}
	}

	//トークンオールイレーサー
	virtual void EraseToken()
	{
		m_CsvTable.clear();
	}

	//トークンイレーサー
	//引数1:行 , 引数2:列
	void EraseToken( const int line , const int row )
	{
		//イテレータ
		std::vector< std::string >::iterator it = m_CsvTable[ line ].begin() + row;

		//削除
		it = m_CsvTable[ line ].erase( it );
	}

	//エンプティートークンイレーサー
	void EraseEmptyToken()
	{
		//トークンテーブル内の無効セル(何も入っていない"")を削除
		for( int i = 0 ; i < GetNumLine() ; ++i )
		{
			std::vector< std::string >::iterator it;
			it = std::remove( m_CsvTable[ i ].begin() , m_CsvTable[ i ].end() , "" );
			m_CsvTable[ i ].erase( it , m_CsvTable[ i ].end() );
		}
	}
};
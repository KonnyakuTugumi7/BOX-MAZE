#include "DXUT.h"
#include "csv.h"
#include <assert.h>

//CSVファイル読み込み
void CCsv::ImportFile( const std::string& file_path )
{
	//ファイルパス
	m_path = file_path;

	//入力ストリーム
	std::ifstream fin;

	//ロケールを定義 //日本語パスに対応
	setlocale( LC_ALL , "");

	//読み取りモードでファイルを開く
	fin.open( file_path , std::ios::in );

	//開けていなかったらエラーを返す
	if( !fin.is_open() ) assert( 0 );
	////出力ストリーム
	//std::ofstream fout;
	//fout.open( file_path , std::ios::trunc );
	//return;

	//次の列
	std::string new_line;
	char* c_new_line;

	//strtok_sが返したトークン
	char* tk;

	//strtok_sが内部使用する
	char* ctx;

	//1行のトークンを一旦こちらに格納
	std::vector< std::string > line_token;

	//テーブルの中身を一掃
	m_csv_table.clear();

	//カウンタ
	int count = 0;

	//ファイルの終端までループを回す
	while( !fin.eof() )
	{
		//トークンを一掃
		line_token.clear();

		//格納限界を超えないように、一旦stringで取得
		getline( fin , new_line );

		//コメント
		if( new_line.find( "//" ) != new_line.npos )
		{
			//コメントを格納
			m_comment_map.insert( std::make_pair( count , new_line ) );

			continue;
		}

		//1列分のトークンサイズを確保
		c_new_line = new char[ new_line.size() + 1 ];

		//トークンをコピー
		strcpy_s( c_new_line , new_line.size() + 1 , new_line.c_str() );
		
		while( 1 )
		{
			//空の状態の時				
			if( line_token.empty() )
			{
				//","で文字列を区切る
				tk = strtok_s( c_new_line , "," , &ctx );

				//","が見つからない時
				if( tk == NULL )
				{
					//終端まで来たらテーブルにトークンを格納してループを抜ける
					line_token.push_back( c_new_line );
					m_csv_table.push_back( line_token );
					break;
				}
			}
			else
			{
				//","で文字列を区切る
				tk = strtok_s( NULL , "," , &ctx );

				//","が見つからない時
				if( tk == NULL )
				{
					//終端まで来たらテーブルにトークンを格納してループを抜ける
					m_csv_table.push_back( line_token );
					break;
				}
			}
			//トークンを格納
			line_token.push_back( tk );
		}
		//使い終わったのでデリート
		delete [] c_new_line;

		//カウンタを進める
		++count;
	}
}

//CSVファイル書き出し
void CCsv::ExportFile()
{
	//出力ストリーム
	std::ofstream ofs;

	//ファイルを開いた時に以前の中身を全て削除する
	ofs.open( m_path , std::ios::trunc );

	//開けていなかったらエラーを返す
	if( !ofs.is_open() ) assert( 0 );

	//最後のトークンまでループを回して書き出す
	for( int line = 0 ; line < GetNumLine() ; ++line )
	{
		//コメント
		std::map< int , std::string >::iterator it = m_comment_map.find( line );
		if( it != m_comment_map.end() )
		{
			//コメント書き出し
			ofs << ( *it ).second;
			ofs << std::endl;
		}

		//行
		int row = 0;
		while( 1 )
		{
			//無効セルまで来たら改行してループを抜ける
			if( IsValidPos( line , row ) == false )
			{
				//改行
				if( GetNumLine() != line + 1 )
				{
					ofs << std::endl;
				}
				break;
			}

			//トークン書き出し
			ofs << m_csv_table[ line ][ row ];

			//セパレータ打ち込み
			if( IsValidPos( line , row + 1 ) == true )
			{
				ofs << ",";
			}

			//行を進める
			++row;
		}
	}
}

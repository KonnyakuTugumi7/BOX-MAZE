#include "dxut.h"
//#include "wsb_xml.h"
//#include <stdlib.h>
//#include <fstream>
//
//#define wsb_xml_fail				(-1)
//#define wsb_xml_false				(0)
//#define wsb_xml_true				(1)
//#define xml_reader_type_root		(99)
//#define wsb_xml_print_debug			(0)
//
//using namespace std ;
//
//void print( const xmlchar *str ) {
////	std::cout << (const char*)str << std::endl ;
//}
//
//void print( std::string str ) {
////	std::cout << str << std::endl ;
//}
//
//
////==================================================================
////	name....loadxmlfile
////	work....ファイルからの読み込み
////	arg1....ファイル名
////	ret.....[ 成功 : cwsbxml へのポインタ ]  [ 失敗 : null ]
////	tips....none
////==================================================================
//cwsbxmlsp cwsbxml::loadxmlfile( std::string _filename ) {
//
//	// ファイルパスの取得
//	xmltextreaderptr wkreader ;
//	
//	// xml リーダーの読み込み
//	wkreader = xmlnewtextreaderfilename( _filename.c_str() ) ;
//	
//	cwsbxmlsp wkxml ;
//	cwsbxmlsp wknullobj ;
//	if ( null == wkreader ) {
//		xmlfreetextreader( wkreader ) ;
//		return wknullobj ;
//	}
//	
//	// パース開始
//	int wkresult = xmltextreaderread( wkreader ) ;
//	if ( wsb_xml_true == wkresult ) {
//		wkxml = cwsbxmlsp( new cwsbxml ) ;
//		wkxml->m_this_week = cwsbxmlwp( wkxml ) ;
//		wkresult = cwsbxml::parseprocess( wkreader, wkxml, xml_reader_type_root ) ;
//	}
//	xmlfreetextreader( wkreader ) ;
//	
//	// 解析失敗
//	if ( wkresult == wsb_xml_fail ){
//		return wknullobj ;
//	}
//	
//    wkxml->m_file_name = _filename ;
//	return wkxml ;
//}
//
//
////==================================================================
////	name....loadxmlstring
////	work....文字列データからxml を作成
////	arg1....文字列データ
////	ret.....[ 成功 : cwsbxml へのポインタ ]  [ 失敗 : null ]
////	tips....none
////==================================================================
//cwsbxmlsp cwsbxml::loadxmlstring( std::string _xmlstring ) {
//	
//    xmlparserinputbufferptr wkinputbuf = xmlallocparserinputbuffer(xml_char_encoding_utf8);
//     
//	cwsbxmlsp wkxml ;
//	cwsbxmlsp wknullobj ;
//    
//	if( !wkinputbuf ) return wknullobj ;
//	
//	xmlbufferptr wkxmlbuf = xmlbuffercreatestatic( (void*)_xmlstring.c_str(), _xmlstring.length() ) ;
//	wkinputbuf->buffer = wkxmlbuf ;
//	
//	xmltextreaderptr wkreader = xmlnewtextreader( wkinputbuf, null ) ;
//	if ( null == wkreader ) {
//		xmlfreetextreader( wkreader ) ;
//		return wknullobj ;
//	}
//
//	/*
//	 *	解析処理
//	 */
//	int wkresult = xmltextreaderread( wkreader ) ;
//	if ( wsb_xml_true == wkresult ) {
//		wkxml = cwsbxmlsp( new cwsbxml ) ;
//		wkxml->m_this_week = cwsbxmlwp( wkxml ) ;
//		wkresult = cwsbxml::parseprocess( wkreader, wkxml, xml_reader_type_root ) ;
//	}
//	xmlfreetextreader( wkreader ) ;
//	
//	// 解析失敗
//	if ( wkresult == wsb_xml_fail ){
//		return wknullobj ;
//	}
//	
//	return wkxml ;
//}
//
////=======================================================================================
////	name... parseprocess
////	work... パース処理
////	arg1... xmltextreaderptr
////	ret.... [ 失敗 -1 ]	[ なし 0 ] [ 成功 1 ] 
////	tips	:	none
////=======================================================================================
//int wknowtype, wknexttype ;
//const xmlchar *wkname, *wkvalue ;
//cwsbxmlsp wkxml ;
//cwsbxmlsp wk_add ;
//cwsbxmlsp wk_attr_xml ;
//int cwsbxml::parseprocess( xmltextreaderptr _reader, cwsbxmlsp _prevxml, int _prevtype ) {
//	
//	
//	// xmltextreaderptr が存在しない
//	if ( _reader == null ) {
//		#if wsb_xml_print_debug
//		print( "err : _reader is null" ) ;
//		#endif
//		return wsb_xml_fail ;
//	}
//	
//	// 要素タイプの取得
//	if ( ( wknowtype = xmltextreadernodetype( _reader ) ) == wsb_xml_fail ) {
//		#if wsb_xml_print_debug
//		print( "err : 要素タイプ is -1" ) ;
//		#endif
//		return wsb_xml_fail ;
//	}
//	
//	/*
//	 *
//	 *	要素タイプによって挙動が変化
//	 *
//	 */
//	switch ( wknowtype ) {
//			
//		/*
//		 *	開始要素
//		 */
//		case xml_reader_type_element :
//			{
//				// 要素名の取得
//				if ( ( wkname = xmltextreaderconstname( _reader ) ) == null ) {
//					#if wsb_xml_print_debug
//					print( "err : 要素名が存在しない" ) ;
//					#endif
//					return wsb_xml_fail ;
//				}
//				#if wsb_xml_print_debug
//				print( wkname ) ;
//				#endif
//				
//				/*
//				 *	要素が空かどうかを判定
//				 *	属性を取得した後だとちゃんととれなかったのでこの時点で
//				 *	空要素かどうかの判定を取る
//				 */
//				int wkisempty = xmltextreaderisemptyelement( _reader ) ;
//
//				/*
//				 *	前回のタイプによって挙動が変化
//				 */
//				switch ( _prevtype ) {
//						
//					/*
//					 *	前回がタグの開始であれば、child を生成
//					 *	_prevxml は parent
//					 */
//					case xml_reader_type_element :
//						{
//							wkxml = cwsbxmlsp( new cwsbxml ) ;
//							wkxml->m_this_week = cwsbxmlwp( wkxml ) ;
////							wkxml->m_tag_name = tostring( wkname ) ;
//							wkxml->m_tag_name = string( (const char*)wkname );
//
//							wkxml->m_tag_parent = cwsbxmlwp( _prevxml ) ;
//							_prevxml->m_tag_child = wkxml ;
//						}
//						break ;
//						
//					/*
//					 *	前回がタグの終了であれば、next を生成
//					 *	_prevxml は brother
//					 */
//					case xml_reader_type_end_element :
//						{
//							// next の最後尾を検出
//							while ( true ) {
//								if ( _prevxml->m_tag_next == null ) break ;
//								_prevxml = _prevxml->m_tag_next ;
//							}
//							
//							// next を生成
//							wkxml = cwsbxmlsp( new cwsbxml ) ;
//							wkxml->m_this_week = cwsbxmlwp( wkxml ) ;
////							wkxml->m_tag_name = tostring( wkname ) ;
//							wkxml->m_tag_name = string( (const char*)wkname ) ;
//							wkxml->m_tag_parent = cwsbxmlwp( _prevxml->m_tag_parent ) ;
//							_prevxml->m_tag_next = wkxml ;
//						}
//						break ;
//						
//					/*
//					 *	始めの一回だけここを通るルート
//					 *	ルートデータの設定
//					 *	_prevxml は root
//					 */
//					case xml_reader_type_root :
//						{
//							wkxml = _prevxml ;
////							wkxml->m_tag_name = tostring( wkname ) ;
//							wkxml->m_tag_name = string( (const char*)wkname ) ;
//							wkxml->m_string = "wsb_root_xml" ;
//							wkxml->m_tag_parent = cwsbxmlwp( wkxml ) ;
//							
//							#if wsb_xml_print_debug
//							print( "root ですよ" ) ;
//							print( wkxml->m_string ) ;
//							#endif
//						}
//						break ;
//						
//					default :
//							return wsb_xml_fail ;
//						break ;
//				}
//				
//				
//				// 属性の取得
//				int wkresult = xmltextreadermovetonextattribute( _reader ) ;
//				
//				if ( wkresult == wsb_xml_true ) {
//					
//					
//					wk_attr_xml = wkxml ;
//
//					// 属性が存在した場合は属性すべて取り出す
//					while ( true ) {
//						
//						// 属性名の取得
//						if ( ( wkname = xmltextreaderconstname( _reader ) ) == null ) {
//							#if wsb_xml_print_debug
//							print( "err : 名前の取得に失敗" ) ;
//							#endif
//							return wsb_xml_fail ;
//						}
//						print( wkname ) ;
//						
//						// 属性データの取得
//						if ( ( wkvalue = xmltextreaderconstvalue( _reader ) ) == null ) {
//							#if wsb_xml_print_debug
//							print( "err : 値の取得に失敗" ) ;
//							#endif
//							return wsb_xml_fail ;
//						}
//						
//						#if wsb_xml_print_debug
//						print( wkvalue ) ;
//						#endif
//						
//						// 属性の生成
//						wk_add = cwsbxmlsp( new cwsbxml ) ;
//						wk_add->m_this_week = cwsbxmlwp( wk_add ) ;
////						wk_add->m_tag_name	= tostring( wkname ) ;
////						wk_add->m_string	= tostring( wkvalue ) ;
//						wk_add->m_tag_name = string( (const char*)wkname ) ;
//						wk_add->m_string = string( (const char*)wkvalue ) ;
//
//						wk_add->m_s32		= strtol( (const char*)wkvalue, null, 10 ) ;
//						wk_add->m_f32		= (float)strtod( (const char*)wkvalue, null ) ;
//						
//						wk_attr_xml->m_attr = wk_add ;
//						wk_attr_xml = wk_attr_xml->m_attr ;
//						
//						// 次の属性の取得
//						wkresult = xmltextreadermovetonextattribute( _reader ) ;
//						if ( wkresult == wsb_xml_true ) {
//						}
//						else if ( wkresult == wsb_xml_false ) {
//							break ;
//						}
//						else if ( wkresult == wsb_xml_fail ) {
//							#if wsb_xml_print_debug
//							print( "err : 属性の取得に失敗 (1)" ) ;
//							#endif
//							return wsb_xml_fail ;
//						}
//					}
//					
//					/*
//					 *	空要素かどうかでの挙動の変化はここで行う
//					 */
//					if ( wkisempty == wsb_xml_true ) {
//						// 属性有りの空要素
//						wknexttype	= xml_reader_type_end_element ; 
//					}
//					else if ( wkisempty == wsb_xml_false ) {
//						// 属性有りの通常要素
//						wknexttype	= wknowtype ;
//					}
//					else if ( wkisempty == wsb_xml_fail ) {
//						#if wsb_xml_print_debug
//						print( "err : 要素が空かどうかの取得に失敗" ) ;
//						#endif
//						return wsb_xml_fail ;
//					}
//					
//				}
//				else if ( wkresult == wsb_xml_false ) {
//					// 属性無し通常要素
//					wknexttype = wknowtype ;
//				}
//				else if ( wkresult == wsb_xml_fail ) {
//					#if wsb_xml_print_debug
//					print( "err : 属性の取得に失敗" ) ;
//					#endif
//					return wsb_xml_fail ;
//				}
//			}
//			break ;
//			
//		/*
//		 *	属性
//		 *
//		 *	ここには入ってきているのかどうかわからない
//		 */
//		case xml_reader_type_attribute :
//			{
//				// 属性名の取得
//				if ( ( wkname = xmltextreaderconstname( _reader ) ) == null ) {
//					#if wsb_xml_print_debug
//					print( "err : 名前の取得に失敗 (1)" ) ;
//					#endif
//					return wsb_xml_fail ;
//				}
//				#if wsb_xml_print_debug
//				print( wkname ) ;
//				#endif
//				
//				// 属性データの取得
//				if ( ( wkvalue = xmltextreaderconstvalue( _reader ) ) == null ) {
//					#if wsb_xml_print_debug
//					print( "err : 値の取得に失敗 (1)" ) ;
//					#endif
//					return wsb_xml_fail ;
//				}
//				#if wsb_xml_print_debug
//				print( wkvalue ) ;
//				print( "属性ですよ" ) ;
//				#endif
//				wkxml		= _prevxml ;
//				wknexttype	= _prevtype ;
//			}
//			break ;
//			
//		/*
//		 *	スペース
//		 *	空白データの場合は前回のデータをそのまま渡す
//		 */
//		case xml_reader_type_whitespace :
//		case xml_reader_type_significant_whitespace :
//			{
//				// 値の取得
//				if ( ( wkvalue = xmltextreaderconstvalue( _reader ) ) == null ) {
//					#if wsb_xml_print_debug
//					print( "err : 値の取得に失敗 (2)" ) ;
//					#endif
//					return wsb_xml_fail ;
//				}
//				
//				#if wsb_xml_print_debug
//				print( "スペース" ) ;
//				print( wkvalue ) ;
//				#endif
//				
//				wkxml		= _prevxml ;
//				wknexttype	= _prevtype ;
//			}
//			break ;
//			
//		/*
//		 *	文字列データ
//		 *	_prevxml が文字列データを挿入する xml になる
//		 */
//		case xml_reader_type_text :
//			{
//				// 値の取得
//				if ( (wkvalue = xmltextreaderconstvalue( _reader ) ) == null ) {
//					#if wsb_xml_print_debug
//					print( "err : 値の取得に失敗 (3)" ) ;
//					#endif
//					return wsb_xml_fail ;
//				}
//				
//				#if wsb_xml_print_debug
//				print( "文字列データの取得" ) ;
//				print( wkvalue ) ;
//				#endif
//				
//				wkxml			= _prevxml ;
////				wkxml->m_string	= tostring( wkvalue ) ;
//				wkxml->m_string = string( (const char*)wkvalue ) ;
//
//				wkxml->m_s32	= strtol( (const char*)wkvalue, null, 10 ) ;
//				wkxml->m_f32	= (float)strtod( (const char*)wkvalue, null ) ;
//				
//				wknexttype	= wknowtype ;
//			}	
//			break ;
//			
//		/*
//		 *	終了要素
//		 *	前回も要素の終了であった場合は parent を返す
//		 */
//		case xml_reader_type_end_element :
//			{
//				if ( _prevtype == xml_reader_type_end_element )
//					wkxml = cwsbxmlsp( _prevxml->m_tag_parent ) ;
//				else
//					wkxml = _prevxml ;
//				
//				wknexttype = wknowtype ;
//			}
//			break ;
//			
//		/*
//		 *	ドキュメント
//		 */
//		case xml_reader_type_document :
//			{
//				#if wsb_xml_print_debug
//				print( "ドキュメント" ) ;
//				#endif
//			}
//			break ;
//			
//		/*
//		 *	その他のタイプの場合は前回のデータをそのまま引き継ぐ
//		 */
//		default :
//			{
//				wkxml		= _prevxml ;
//				wknexttype	= _prevtype ;
//			}
//			break ;
//	}
//	
//	// 属性データの場合の処理
//	if ( wknowtype == xml_reader_type_attribute )
//		return xmltextreadermovetonextattribute( _reader ) ;
//	
//	// 次のデータを読み込む
//	int wkcompflag = xmltextreaderread( _reader ) ;
//	if ( wkcompflag == wsb_xml_true )
//		return cwsbxml::parseprocess( _reader, wkxml, wknexttype ) ;
//	else if ( wkcompflag == wsb_xml_fail )
//		return wsb_xml_fail ;
//	
//	return wsb_xml_false ;
//
//}
//
////=========================================================
//// name... createxml
//// work... xml を作成
//// arg1... タグ名
//// ret.... xml へのスマートポインタ
//// tips... none
////=========================================================
//cwsbxmlsp cwsbxml::createxml( std::string _root_name )
//{
//	cwsbxmlsp wkxml ;
//	wkxml = cwsbxmlsp( new cwsbxml ) ;
//	wkxml->m_this_week = cwsbxmlwp( wkxml ) ;
//	wkxml->m_tag_name = _root_name ;
//	wkxml->m_string = "wsb_root_xml" ;
//	return wkxml ;
//}
//
////=========================================================
//// work... xmlをセーブ
//// arg1... セーブファイル名
////=========================================================
//void cwsbxml::savexmlfile( std::string _filename )
//{
//	std::string xml_str ;
//	getxmlstring( &xml_str ) ;
//	std::ofstream ofs( _filename ) ;
//	ofs << xml_str << std::endl ;
//}
//
////=====================================================
//// テキストベースの xmlを取得
//// arg1... 出力結果
//// arg2... 指定しなくてok
////=====================================================
//void cwsbxml::getxmlstring( string *_string, bool start )
//{
//	
//	if( start ){
//		*_string = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>" ;
//		*_string += "\n" ;
//	}
//	
//	string esc = "\n" ;
//	cwsbxmlsp sarch ;
//	
//	*_string += "<" + this->m_tag_name + ">" ;
//	if( this->m_tag_child != null ){
//		*_string += esc ;
//		sarch = this->m_tag_child ;
//		sarch->getxmlstring( _string, false ) ;
//		
//		// 子供に兄弟がいる場合は親が処理する
//		// 兄弟に兄弟を処理させてはだめ
//		if( this->m_tag_child->m_tag_next != null ){
//			sarch = this->m_tag_child ;
//			while( sarch->m_tag_next != null ){
//				sarch = sarch->m_tag_next ;
//				sarch->getxmlstring( _string, false ) ;
//			}
//		}
//		
//	}else{
//		*_string += this->m_string ;
//	}
//	*_string += "</" + this->m_tag_name + ">" + esc ;
//
//	return ;
//}
//
//
////=========================================================
////=========================================================
//void cwsbxml::set( int _v )
//{
//	char str[64] ;
//	memset( str, 0, sizeof(str) ) ;
//	sprintf_s( str, "%d", _v ) ;
//	this->m_string = str ;
//	this->m_s32	= _v ;
//	this->m_f32	= (float)_v ;
//}
//
////=========================================================
////=========================================================
//void cwsbxml::set( float _v )
//{
//
//	char str[64] ;
//	memset( str, 0, sizeof(str) ) ;
//	sprintf_s( str, "%f", _v ) ;
//	this->m_string = str ;
//	this->m_s32	= (int)_v ;
//	this->m_f32	= _v ;
//}
//
////=========================================================
////=========================================================
//void cwsbxml::set( std::string _string )
//{
//	this->m_string = _string ;
//	this->m_s32		= strtol( (const char*)_string.c_str(), null, 10 ) ;
//	this->m_f32	= (float)strtod( (const char*)_string.c_str(), null ) ;
//}
//
////=========================================================
//// name... getelementnum
//// work.... エレメントの数を取得する
//// arg1.... エレメント名 ( 指定なしの場合は全ての子供のエレメント )
//// ret....... [ 成功 : xml へのスマートポインタ ] [ 失敗 : null オブジェクト ]
//// tips...... none
////=========================================================
//int	cwsbxml::getelementnum( std::string _tag_name )
//{
//	int element_count = 0 ;
//	bool all_ok = false ;
//	cwsbxmlsp sarch = this->m_tag_child ;
//	
//	if( _tag_name == "all_element" ){
//		all_ok = true ;
//	}
//	
//	while( sarch != null ){
//		if( all_ok ){
//			element_count++ ;
//		}else if( sarch->m_tag_name == _tag_name ){
//			element_count++ ;
//		}
//		sarch = sarch->m_tag_next ;
//	}
//	
//	return element_count ;
//	
//}
//
////=========================================================
//// name... getelement
//// work.... エレメントを取得する
//// arg1.... エレメント名
//// arg2.... 何番目を取得するか
//// ret....... [ 成功 : xml へのスマートポインタ ] [ 失敗 : null オブジェクト ]
//// tips...... none
////=========================================================
//cwsbxmlsp cwsbxml::getelement( std::string _tag_name, int _idx )
//{
//	int element_count = 0 ;
//	cwsbxmlsp null_obj ;
//	cwsbxmlsp sarch = this->m_tag_child ;
//	
//	while( sarch != null ){
//		if( sarch->m_tag_name == _tag_name ){
//			if( element_count == _idx ) return sarch ;
//			element_count++ ;
//		}
//		sarch = sarch->m_tag_next ;
//	}
//	
//	return null_obj ;
//}
//
//
////=========================================================
//// name... getelement
//// work.... エレメントを取得する
//// arg1.... 何番目を取得するか
//// ret....... [ 成功 : xml へのスマートポインタ ] [ 失敗 : null オブジェクト ]
//// tips...... none
////=========================================================
//cwsbxmlsp cwsbxml::getelement( int _idx )
//{
//	cwsbxmlsp null_obj ;
//	cwsbxmlsp sarch = this->m_tag_child ;
//	
//	for( int i = 0 ; i < _idx ; i++ ){
//		if( sarch == null ) return null_obj ;
//		sarch = sarch->m_tag_next ;
//	}
//	return sarch ;
//	
//}
//
//
////=========================================================
//// name... getelementtop
//// work.... 最上位のエレメントを取得する
//// ret....... [ 成功 : xml へのスマートポインタ ] [ 失敗 : null オブジェクト ]
//// tips...... none
////=========================================================
//cwsbxmlsp cwsbxml::getelementtop()
//{
//	cwsbxmlsp parent = getparentelement() ;
//	while( 1 ){
//		if( parent->getstring() == "wsb_root_xml" ) return parent ;
//		parent = parent->getparentelement() ;
//	}
//	return parent ;
//}
//
//
////=========================================================
//// name... getattrnum
//// work.... 属性の数を取得する
//// arg1.... 属性名 ( 指定なしの場合は全ての属性)
//// ret....... [ 成功 : xml へのスマートポインタ ] [ 失敗 : null オブジェクト ]
//// tips...... none
////=========================================================
//int	cwsbxml::getattrnum( std::string _tag_name )
//{
//	int attr_count = 0 ;
//	bool all_ok = false ;
//	cwsbxmlsp sarch = this->m_attr ;
//	
//	if( _tag_name == "all_attribute" ){
//		all_ok = true ;
//	}
//	
//	while( sarch != null ){
//		if( all_ok ){
//			attr_count++ ;
//		}else if( sarch->m_tag_name == _tag_name ){
//			attr_count++ ;
//		}
//		sarch = sarch->m_attr ;
//	}
//	
//	return attr_count ;
//	
//}
//
//
////=========================================================
//// name... getattr
//// work.... 属性を取得する
//// arg1.... 属性名
//// arg2.... 何番目を取得するか
//// ret....... [ 成功 : xml へのスマートポインタ ] [ 失敗 : null オブジェクト ]
//// tips...... none
////=========================================================
//cwsbxmlsp cwsbxml::getattr( std::string _tag_name, int _idx )
//{
//	int attr_count = 0 ;
//	cwsbxmlsp null_obj ;
//	cwsbxmlsp sarch = this->m_attr ;
//	
//	while( sarch != null ){
//		if( sarch->m_tag_name == _tag_name ){
//			if( attr_count == _idx ){
//				return sarch ;
//			}
//			attr_count++ ;
//		}
//		sarch = sarch->m_attr ;
//	}
//	
//	return null_obj ;
//}
//
////=========================================================
//// name... getattr
//// work.... 属性を取得する
//// arg1.... 何番目を取得するか
//// ret....... [ 成功 : xml へのスマートポインタ ] [ 失敗 : null オブジェクト ]
//// tips...... none
////=========================================================
//cwsbxmlsp cwsbxml::getattr( int _idx )
//{
//	cwsbxmlsp null_obj ;
//	cwsbxmlsp sarch = this->m_attr ;
//	
//	for( int i = 0 ; i < _idx ; i++ ){
//		if( sarch == null ) return null_obj ;
//		sarch = sarch->m_attr ;
//	}
//	return sarch ;
//	
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

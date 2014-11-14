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
////	work....�t�@�C������̓ǂݍ���
////	arg1....�t�@�C����
////	ret.....[ ���� : cwsbxml �ւ̃|�C���^ ]  [ ���s : null ]
////	tips....none
////==================================================================
//cwsbxmlsp cwsbxml::loadxmlfile( std::string _filename ) {
//
//	// �t�@�C���p�X�̎擾
//	xmltextreaderptr wkreader ;
//	
//	// xml ���[�_�[�̓ǂݍ���
//	wkreader = xmlnewtextreaderfilename( _filename.c_str() ) ;
//	
//	cwsbxmlsp wkxml ;
//	cwsbxmlsp wknullobj ;
//	if ( null == wkreader ) {
//		xmlfreetextreader( wkreader ) ;
//		return wknullobj ;
//	}
//	
//	// �p�[�X�J�n
//	int wkresult = xmltextreaderread( wkreader ) ;
//	if ( wsb_xml_true == wkresult ) {
//		wkxml = cwsbxmlsp( new cwsbxml ) ;
//		wkxml->m_this_week = cwsbxmlwp( wkxml ) ;
//		wkresult = cwsbxml::parseprocess( wkreader, wkxml, xml_reader_type_root ) ;
//	}
//	xmlfreetextreader( wkreader ) ;
//	
//	// ��͎��s
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
////	work....������f�[�^����xml ���쐬
////	arg1....������f�[�^
////	ret.....[ ���� : cwsbxml �ւ̃|�C���^ ]  [ ���s : null ]
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
//	 *	��͏���
//	 */
//	int wkresult = xmltextreaderread( wkreader ) ;
//	if ( wsb_xml_true == wkresult ) {
//		wkxml = cwsbxmlsp( new cwsbxml ) ;
//		wkxml->m_this_week = cwsbxmlwp( wkxml ) ;
//		wkresult = cwsbxml::parseprocess( wkreader, wkxml, xml_reader_type_root ) ;
//	}
//	xmlfreetextreader( wkreader ) ;
//	
//	// ��͎��s
//	if ( wkresult == wsb_xml_fail ){
//		return wknullobj ;
//	}
//	
//	return wkxml ;
//}
//
////=======================================================================================
////	name... parseprocess
////	work... �p�[�X����
////	arg1... xmltextreaderptr
////	ret.... [ ���s -1 ]	[ �Ȃ� 0 ] [ ���� 1 ] 
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
//	// xmltextreaderptr �����݂��Ȃ�
//	if ( _reader == null ) {
//		#if wsb_xml_print_debug
//		print( "err : _reader is null" ) ;
//		#endif
//		return wsb_xml_fail ;
//	}
//	
//	// �v�f�^�C�v�̎擾
//	if ( ( wknowtype = xmltextreadernodetype( _reader ) ) == wsb_xml_fail ) {
//		#if wsb_xml_print_debug
//		print( "err : �v�f�^�C�v is -1" ) ;
//		#endif
//		return wsb_xml_fail ;
//	}
//	
//	/*
//	 *
//	 *	�v�f�^�C�v�ɂ���ċ������ω�
//	 *
//	 */
//	switch ( wknowtype ) {
//			
//		/*
//		 *	�J�n�v�f
//		 */
//		case xml_reader_type_element :
//			{
//				// �v�f���̎擾
//				if ( ( wkname = xmltextreaderconstname( _reader ) ) == null ) {
//					#if wsb_xml_print_debug
//					print( "err : �v�f�������݂��Ȃ�" ) ;
//					#endif
//					return wsb_xml_fail ;
//				}
//				#if wsb_xml_print_debug
//				print( wkname ) ;
//				#endif
//				
//				/*
//				 *	�v�f���󂩂ǂ����𔻒�
//				 *	�������擾�����ゾ�Ƃ����ƂƂ�Ȃ������̂ł��̎��_��
//				 *	��v�f���ǂ����̔�������
//				 */
//				int wkisempty = xmltextreaderisemptyelement( _reader ) ;
//
//				/*
//				 *	�O��̃^�C�v�ɂ���ċ������ω�
//				 */
//				switch ( _prevtype ) {
//						
//					/*
//					 *	�O�񂪃^�O�̊J�n�ł���΁Achild �𐶐�
//					 *	_prevxml �� parent
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
//					 *	�O�񂪃^�O�̏I���ł���΁Anext �𐶐�
//					 *	_prevxml �� brother
//					 */
//					case xml_reader_type_end_element :
//						{
//							// next �̍Ō�������o
//							while ( true ) {
//								if ( _prevxml->m_tag_next == null ) break ;
//								_prevxml = _prevxml->m_tag_next ;
//							}
//							
//							// next �𐶐�
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
//					 *	�n�߂̈�񂾂�������ʂ郋�[�g
//					 *	���[�g�f�[�^�̐ݒ�
//					 *	_prevxml �� root
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
//							print( "root �ł���" ) ;
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
//				// �����̎擾
//				int wkresult = xmltextreadermovetonextattribute( _reader ) ;
//				
//				if ( wkresult == wsb_xml_true ) {
//					
//					
//					wk_attr_xml = wkxml ;
//
//					// ���������݂����ꍇ�͑������ׂĎ��o��
//					while ( true ) {
//						
//						// �������̎擾
//						if ( ( wkname = xmltextreaderconstname( _reader ) ) == null ) {
//							#if wsb_xml_print_debug
//							print( "err : ���O�̎擾�Ɏ��s" ) ;
//							#endif
//							return wsb_xml_fail ;
//						}
//						print( wkname ) ;
//						
//						// �����f�[�^�̎擾
//						if ( ( wkvalue = xmltextreaderconstvalue( _reader ) ) == null ) {
//							#if wsb_xml_print_debug
//							print( "err : �l�̎擾�Ɏ��s" ) ;
//							#endif
//							return wsb_xml_fail ;
//						}
//						
//						#if wsb_xml_print_debug
//						print( wkvalue ) ;
//						#endif
//						
//						// �����̐���
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
//						// ���̑����̎擾
//						wkresult = xmltextreadermovetonextattribute( _reader ) ;
//						if ( wkresult == wsb_xml_true ) {
//						}
//						else if ( wkresult == wsb_xml_false ) {
//							break ;
//						}
//						else if ( wkresult == wsb_xml_fail ) {
//							#if wsb_xml_print_debug
//							print( "err : �����̎擾�Ɏ��s (1)" ) ;
//							#endif
//							return wsb_xml_fail ;
//						}
//					}
//					
//					/*
//					 *	��v�f���ǂ����ł̋����̕ω��͂����ōs��
//					 */
//					if ( wkisempty == wsb_xml_true ) {
//						// �����L��̋�v�f
//						wknexttype	= xml_reader_type_end_element ; 
//					}
//					else if ( wkisempty == wsb_xml_false ) {
//						// �����L��̒ʏ�v�f
//						wknexttype	= wknowtype ;
//					}
//					else if ( wkisempty == wsb_xml_fail ) {
//						#if wsb_xml_print_debug
//						print( "err : �v�f���󂩂ǂ����̎擾�Ɏ��s" ) ;
//						#endif
//						return wsb_xml_fail ;
//					}
//					
//				}
//				else if ( wkresult == wsb_xml_false ) {
//					// ���������ʏ�v�f
//					wknexttype = wknowtype ;
//				}
//				else if ( wkresult == wsb_xml_fail ) {
//					#if wsb_xml_print_debug
//					print( "err : �����̎擾�Ɏ��s" ) ;
//					#endif
//					return wsb_xml_fail ;
//				}
//			}
//			break ;
//			
//		/*
//		 *	����
//		 *
//		 *	�����ɂ͓����Ă��Ă���̂��ǂ����킩��Ȃ�
//		 */
//		case xml_reader_type_attribute :
//			{
//				// �������̎擾
//				if ( ( wkname = xmltextreaderconstname( _reader ) ) == null ) {
//					#if wsb_xml_print_debug
//					print( "err : ���O�̎擾�Ɏ��s (1)" ) ;
//					#endif
//					return wsb_xml_fail ;
//				}
//				#if wsb_xml_print_debug
//				print( wkname ) ;
//				#endif
//				
//				// �����f�[�^�̎擾
//				if ( ( wkvalue = xmltextreaderconstvalue( _reader ) ) == null ) {
//					#if wsb_xml_print_debug
//					print( "err : �l�̎擾�Ɏ��s (1)" ) ;
//					#endif
//					return wsb_xml_fail ;
//				}
//				#if wsb_xml_print_debug
//				print( wkvalue ) ;
//				print( "�����ł���" ) ;
//				#endif
//				wkxml		= _prevxml ;
//				wknexttype	= _prevtype ;
//			}
//			break ;
//			
//		/*
//		 *	�X�y�[�X
//		 *	�󔒃f�[�^�̏ꍇ�͑O��̃f�[�^�����̂܂ܓn��
//		 */
//		case xml_reader_type_whitespace :
//		case xml_reader_type_significant_whitespace :
//			{
//				// �l�̎擾
//				if ( ( wkvalue = xmltextreaderconstvalue( _reader ) ) == null ) {
//					#if wsb_xml_print_debug
//					print( "err : �l�̎擾�Ɏ��s (2)" ) ;
//					#endif
//					return wsb_xml_fail ;
//				}
//				
//				#if wsb_xml_print_debug
//				print( "�X�y�[�X" ) ;
//				print( wkvalue ) ;
//				#endif
//				
//				wkxml		= _prevxml ;
//				wknexttype	= _prevtype ;
//			}
//			break ;
//			
//		/*
//		 *	������f�[�^
//		 *	_prevxml ��������f�[�^��}������ xml �ɂȂ�
//		 */
//		case xml_reader_type_text :
//			{
//				// �l�̎擾
//				if ( (wkvalue = xmltextreaderconstvalue( _reader ) ) == null ) {
//					#if wsb_xml_print_debug
//					print( "err : �l�̎擾�Ɏ��s (3)" ) ;
//					#endif
//					return wsb_xml_fail ;
//				}
//				
//				#if wsb_xml_print_debug
//				print( "������f�[�^�̎擾" ) ;
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
//		 *	�I���v�f
//		 *	�O����v�f�̏I���ł������ꍇ�� parent ��Ԃ�
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
//		 *	�h�L�������g
//		 */
//		case xml_reader_type_document :
//			{
//				#if wsb_xml_print_debug
//				print( "�h�L�������g" ) ;
//				#endif
//			}
//			break ;
//			
//		/*
//		 *	���̑��̃^�C�v�̏ꍇ�͑O��̃f�[�^�����̂܂܈����p��
//		 */
//		default :
//			{
//				wkxml		= _prevxml ;
//				wknexttype	= _prevtype ;
//			}
//			break ;
//	}
//	
//	// �����f�[�^�̏ꍇ�̏���
//	if ( wknowtype == xml_reader_type_attribute )
//		return xmltextreadermovetonextattribute( _reader ) ;
//	
//	// ���̃f�[�^��ǂݍ���
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
//// work... xml ���쐬
//// arg1... �^�O��
//// ret.... xml �ւ̃X�}�[�g�|�C���^
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
//// work... xml���Z�[�u
//// arg1... �Z�[�u�t�@�C����
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
//// �e�L�X�g�x�[�X�� xml���擾
//// arg1... �o�͌���
//// arg2... �w�肵�Ȃ���ok
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
//		// �q���ɌZ�킪����ꍇ�͐e����������
//		// �Z��ɌZ������������Ă͂���
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
//// work.... �G�������g�̐����擾����
//// arg1.... �G�������g�� ( �w��Ȃ��̏ꍇ�͑S�Ă̎q���̃G�������g )
//// ret....... [ ���� : xml �ւ̃X�}�[�g�|�C���^ ] [ ���s : null �I�u�W�F�N�g ]
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
//// work.... �G�������g���擾����
//// arg1.... �G�������g��
//// arg2.... ���Ԗڂ��擾���邩
//// ret....... [ ���� : xml �ւ̃X�}�[�g�|�C���^ ] [ ���s : null �I�u�W�F�N�g ]
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
//// work.... �G�������g���擾����
//// arg1.... ���Ԗڂ��擾���邩
//// ret....... [ ���� : xml �ւ̃X�}�[�g�|�C���^ ] [ ���s : null �I�u�W�F�N�g ]
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
//// work.... �ŏ�ʂ̃G�������g���擾����
//// ret....... [ ���� : xml �ւ̃X�}�[�g�|�C���^ ] [ ���s : null �I�u�W�F�N�g ]
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
//// work.... �����̐����擾����
//// arg1.... ������ ( �w��Ȃ��̏ꍇ�͑S�Ă̑���)
//// ret....... [ ���� : xml �ւ̃X�}�[�g�|�C���^ ] [ ���s : null �I�u�W�F�N�g ]
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
//// work.... �������擾����
//// arg1.... ������
//// arg2.... ���Ԗڂ��擾���邩
//// ret....... [ ���� : xml �ւ̃X�}�[�g�|�C���^ ] [ ���s : null �I�u�W�F�N�g ]
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
//// work.... �������擾����
//// arg1.... ���Ԗڂ��擾���邩
//// ret....... [ ���� : xml �ւ̃X�}�[�g�|�C���^ ] [ ���s : null �I�u�W�F�N�g ]
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

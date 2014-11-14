#ifndef SOUND_H
#define SOUND_H

#define _WIN32_DCOM
#define _CRT_SECURE_NO_DEPRECATE

#include "DXUT.h"
#include <windows.h>
#include <xaudio2.h>
#include <strsafe.h>
#include <shellapi.h>
#include <mmsystem.h>
#include <conio.h>
#include "SDKwavefile.h"

#include <windows.h>
#include <process.h>
#include <string>

#include "sound_manager.h"

// サウンドハンドル
class CSound{
private :
	CSound( const std::string file_name, const int mode ){
		m_mode = mode ;
		m_is_end = false ;
		m_is_erase_ok = false ;
		m_file_name = file_name ;
		m_state = 0 ;
		m_next_state = 0 ;
	} ;
	~CSound(){} ;

	int			m_id ;
	int			m_mode ;
	int			m_state ;
	int			m_next_state ;
	bool		m_is_end ;
	bool		m_is_erase_ok ;
	HANDLE		m_thread_h ;
	std::string m_file_name ;

	//----------------------------------------------
	// スレッド
	static void run( LPVOID pParam ) ;

	friend class CSoundManager ;

} ;



#endif


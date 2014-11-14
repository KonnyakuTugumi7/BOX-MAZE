#include "DXUT.h"
#include "sound_manager.h"

int CSoundManager::m_id_count = 0 ;
std::map< std::string, CSoundWav* > CSoundManager::m_buffers ;
std::map< std::string, std::map<int,CSound*> > CSoundManager::m_sounds ;

//--------------------------------------------------------------------------------------
// Forward declaration
//--------------------------------------------------------------------------------------
static HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename );


static HRESULT DXconvAnsiToWide( WCHAR* wstrDest, const CHAR* strSrc, int ct )
{
    // 引数の*がNULLの場合と、文字数が1以下の場合はエラー
    if( wstrDest==NULL || strSrc==NULL || ct < 1 )
        return E_INVALIDARG;

    int nResult = MultiByteToWideChar( CP_ACP, 0, strSrc, -1, wstrDest, ct );
    wstrDest[ct-1] = 0;
    
    if( nResult == 0 )
        return E_FAIL;
    return S_OK;
}

//--------------------------------------------------------------------------------------
// Helper function to try to find the location of a media file
//--------------------------------------------------------------------------------------
HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename )
{
    bool bFound = false;

    if( NULL == strFilename || strFilename[0] == 0 || NULL == strDestPath || cchDest < 10 )
        return E_INVALIDARG;

    // Get the exe name, and exe path
    WCHAR strExePath[MAX_PATH] = {0};
    WCHAR strExeName[MAX_PATH] = {0};
    WCHAR* strLastSlash = NULL;
    GetModuleFileName( NULL, strExePath, MAX_PATH );
    strExePath[MAX_PATH - 1] = 0;
    strLastSlash = wcsrchr( strExePath, TEXT( '\\' ) );
    if( strLastSlash )
    {
        wcscpy_s( strExeName, MAX_PATH, &strLastSlash[1] );

        // Chop the exe name from the exe path
        *strLastSlash = 0;

        // Chop the .exe from the exe name
        strLastSlash = wcsrchr( strExeName, TEXT( '.' ) );
        if( strLastSlash )
            *strLastSlash = 0;
    }

    wcscpy_s( strDestPath, cchDest, strFilename );
    if( GetFileAttributes( strDestPath ) != 0xFFFFFFFF )
        return S_OK;

    // Search all parent directories starting at .\ and using strFilename as the leaf name
    WCHAR strLeafName[MAX_PATH] = {0};
    wcscpy_s( strLeafName, MAX_PATH, strFilename );

    WCHAR strFullPath[MAX_PATH] = {0};
    WCHAR strFullFileName[MAX_PATH] = {0};
    WCHAR strSearch[MAX_PATH] = {0};
    WCHAR* strFilePart = NULL;

    GetFullPathName( L".", MAX_PATH, strFullPath, &strFilePart );
    if( strFilePart == NULL )
        return E_FAIL;

    while( strFilePart != NULL && *strFilePart != '\0' )
    {
        swprintf_s( strFullFileName, MAX_PATH, L"%s\\%s", strFullPath, strLeafName );
        if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
        {
            wcscpy_s( strDestPath, cchDest, strFullFileName );
            bFound = true;
            break;
        }

        swprintf_s( strFullFileName, MAX_PATH, L"%s\\%s\\%s", strFullPath, strExeName, strLeafName );
        if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
        {
            wcscpy_s( strDestPath, cchDest, strFullFileName );
            bFound = true;
            break;
        }

        swprintf_s( strSearch, MAX_PATH, L"%s\\..", strFullPath );
        GetFullPathName( strSearch, MAX_PATH, strFullPath, &strFilePart );
    }
    if( bFound )
        return S_OK;

    // On failure, return the file as the path but also return an error code
    wcscpy_s( strDestPath, cchDest, strFilename );

    return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
}



void CSoundManager::Load( const std::string& file_name )
{
	std::map< std::string, CSoundWav* >::iterator it = m_buffers.find( file_name ) ;
	if( it != m_buffers.end() ) return ;

    HRESULT hr;
	CSoundWav* p_wav = new CSoundWav() ;

    //
    // Locate the wave file
    //
	WCHAR strFilePath[MAX_PATH] = {0} ;
	WCHAR w_path[256] = {0} ;
	DXconvAnsiToWide( w_path, file_name.c_str(), (file_name.length()+1) ) ;
    if( FAILED( hr = FindMediaFileCch( strFilePath, MAX_PATH, w_path ) ) )
    {
#ifdef _DEBUG
		WCHAR d_buff[256] = {0} ;
		std::string d_str = "サウンドデータのロードに失敗 001 : " + file_name ;
		DXconvAnsiToWide( d_buff, d_str.c_str(), (d_str.length()+1) ) ;
		DXUTTRACE( L"\n------------------------------------------------------------\n" ) ;
		DXUTTRACE( d_buff ) ;
		DXUTTRACE( L"\n------------------------------------------------------------\n" ) ;
#endif
		return ;
    }

    //
    // Read in the wave file
    //
    if( FAILED( hr = p_wav->m_wav.Open( strFilePath, NULL, WAVEFILE_READ ) ) )
    {
#ifdef _DEBUG
		WCHAR d_buff[256] = {0} ;
		std::string d_str = "サウンドデータのロードに失敗 002 : " + file_name ;
		DXconvAnsiToWide( d_buff, d_str.c_str(), (d_str.length()+1) ) ;
		DXUTTRACE( L"\n------------------------------------------------------------\n" ) ;
		DXUTTRACE( d_buff ) ;
		DXUTTRACE( L"\n------------------------------------------------------------\n" ) ;
#endif
		return ;
    }

    // Get format of wave file
    WAVEFORMATEX* pwfx = p_wav->m_wav.GetFormat();

    // Calculate how many bytes and samples are in the wave
    DWORD cbWaveSize = p_wav->m_wav.GetSize();

    // Read the sample data into memory
	p_wav->m_p_buffer = new BYTE[ cbWaveSize ];
    if( FAILED( hr = p_wav->m_wav.Read( p_wav->m_p_buffer, cbWaveSize, &cbWaveSize ) ) )
    {
        SAFE_DELETE_ARRAY( p_wav->m_p_buffer );

#ifdef _DEBUG
		WCHAR d_buff[256] = {0} ;
		std::string d_str = "サウンドデータのロードに失敗 003 : " + file_name ;
		DXconvAnsiToWide( d_buff, d_str.c_str(), (d_str.length()+1) ) ;
		DXUTTRACE( L"\n------------------------------------------------------------\n" ) ;
		DXUTTRACE( d_buff ) ;
		DXUTTRACE( L"\n------------------------------------------------------------\n" ) ;
#endif
		return ;
    }

	m_buffers.insert( std::make_pair( file_name, p_wav ) ) ;

}

//----------------------------------------------------------------------------------------------------
void CSoundManager::Updata()
{
	std::map< std::string, std::map<int,CSound*> >::iterator it = m_sounds.begin() ;
	while( it != m_sounds.end() ){
		std::map<int, CSound*>::iterator itm = it->second.begin() ;
		while( itm != it->second.end() ){
			if( itm->second->m_is_erase_ok ){
				delete itm->second ;
				it->second.erase( itm++ ) ;
				continue ;
			}
			itm++ ;
		}
		if( it->second.empty() ){
			m_sounds.erase( it++ ) ;
		}else{
			it++ ;
		}
	}


	std::map< std::string, CSoundWav* >::iterator itw = m_buffers.begin() ;
	while( itw != m_buffers.end() ){
		it = m_sounds.find( itw->first ) ;
		if( it == m_sounds.end() && itw->second->m_is_delete_ok ){
			delete itw->second ;
			m_buffers.erase( itw++ ) ;
			continue ;
		}
		itw++ ;
	}

}


//----------------------------------------------------------------------------------------------------
void CSoundManager::Erase( const int sound_id )
{
	std::map< std::string, std::map<int,CSound*> >::iterator it = m_sounds.begin() ;
	while( it != m_sounds.end() ){
		std::map<int, CSound*>::iterator itm = it->second.find( sound_id ) ;
		if( itm == it->second.end() ){
			it++ ;
			continue ;
		}
		delete itm->second ;
		it->second.erase( itm ) ;
		break ;
	}
}

//----------------------------------------------------------------------------------------------------
int CSoundManager::GetState( const int sound_id )
{
	std::map< std::string, std::map<int,CSound*> >::iterator it = m_sounds.begin() ;
	while( it != m_sounds.end() ){
		std::map<int, CSound*>::iterator itm = it->second.find( sound_id ) ;
		if( itm == it->second.end() ){
			it++ ;
			continue ;
		}
		return itm->second->m_state ;
	}
	return CSoundManager::STATE_NONE ;
}

//----------------------------------------------------------------------------------------------------
int CSoundManager::Play( const std::string& file_name, int mode )
{
	m_id_count++ ;
	CSound* p_sound = new CSound( file_name, mode ) ;
	std::map< std::string, std::map<int,CSound*> >::iterator it = m_sounds.find( file_name ) ;
	if( it == m_sounds.end() ){
		std::map<int, CSound*> m ;
		m.insert( std::make_pair( m_id_count, p_sound ) ) ;
		m_sounds.insert( std::make_pair( file_name, m ) ) ;
	}else{
		it->second.insert( std::make_pair( m_id_count, p_sound ) ) ;
	}

	HANDLE mutex ;
	mutex = CreateMutex(NULL,FALSE,NULL);
	p_sound->m_thread_h = (HANDLE)_beginthread( CSound::run, 0, p_sound ) ;
	p_sound->m_state = CSoundManager::STATE_PLAY ;
	return m_id_count ;
}

//----------------------------------------------------------------------------------------------------
void CSoundManager::Play( const int sound_id )
{
	std::map< std::string, std::map<int,CSound*> >::iterator it = m_sounds.begin() ;
	while( it != m_sounds.end() ){
		std::map<int, CSound*>::iterator itm = it->second.find( sound_id ) ;
		if( itm == it->second.end() ){
			it++ ;
			continue ;
		}
		itm->second->m_next_state = CSoundManager::STATE_REPLAY ;
		break ;
	}
}


//----------------------------------------------------------------------------------------------------
void CSoundManager::Stop( const int sound_id )
{
	std::map< std::string, std::map<int,CSound*> >::iterator it = m_sounds.begin() ;
	while( it != m_sounds.end() ){
		std::map<int, CSound*>::iterator itm = it->second.find( sound_id ) ;
		if( itm == it->second.end() ){
			it++ ;
			continue ;
		}
		itm->second->m_next_state = CSoundManager::STATE_STOP ;
		break ;
	}
}


//----------------------------------------------------------------------------------------------------
void CSoundManager::Delete( const std::string& file_name )
{
	std::map< std::string, std::map<int,CSound*> >::iterator it = m_sounds.find( file_name ) ;
	std::map< std::string, CSoundWav* >::iterator itw = m_buffers.find( file_name ) ;
	std::map<int, CSound*>::iterator itm ;

	if( it != m_sounds.end() ){
		itm = it->second.begin() ;
		while( itm != it->second.end() ){
			itm->second->m_is_end = true ;
			itm++ ;
		}
	}

	if( itw != m_buffers.end() ){
		itw->second->m_is_delete_ok = true ;
	}

}


//----------------------------------------------------------------------------------------------------
void CSoundManager::Destroy()
{
	std::map< std::string, CSoundWav* >::iterator itw = m_buffers.begin() ;
	while( itw != m_buffers.end() ){
		CSoundManager::Delete( itw->first ) ;
		itw++ ;
	}

	while(1){
		CSoundManager::Updata() ;
		if( m_buffers.empty() && m_sounds.empty() ){
			break ;
		}
		Sleep(10) ;
	}

}


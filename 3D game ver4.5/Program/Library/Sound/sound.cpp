#include "DXUT.h"
#include "sound.h"

void CSound::run( LPVOID pParam )
{

	CSound* p_sound = (CSound*)pParam ;
	std::map<std::string, CSoundWav*>::iterator it = CSoundManager::m_buffers.find( p_sound->m_file_name ) ;

	if( it == CSoundManager::m_buffers.end() ){
		// DXUTTrace
		p_sound->m_is_erase_ok = true ;
		return ;
	}

	CSoundWav* p_wav = it->second ;

    HRESULT hr;
    UINT32 flags = 0;
    IXAudio2* pXAudio2 = NULL;
    IXAudio2MasteringVoice* pMasteringVoice = NULL;

    CoInitializeEx( NULL, COINIT_MULTITHREADED );

	#ifdef _DEBUG
    flags |= XAUDIO2_DEBUG_ENGINE;
	#endif

    if( FAILED( hr = XAudio2Create( &pXAudio2, flags ) ) )
    {
        CoUninitialize();
        return ;
    }

    //
    // Create a mastering voice
    //
    if( FAILED( hr = pXAudio2->CreateMasteringVoice( &pMasteringVoice ) ) )
    {
        SAFE_RELEASE( pXAudio2 );
        CoUninitialize();
        return ;
    }

    // Get format of wave file
	WAVEFORMATEX* pwfx = p_wav->m_wav.GetFormat();

    // Calculate how many bytes and samples are in the wave
    DWORD cbWaveSize = p_wav->m_wav.GetSize();

    // Create the source voice
	IXAudio2SourceVoice* pSourceVoice;
    if( FAILED( hr = pXAudio2->CreateSourceVoice( &pSourceVoice, pwfx ) ) )
    {

    }

    XAUDIO2_BUFFER buffer = {0};
    buffer.pAudioData = p_wav->m_p_buffer;
    buffer.Flags = XAUDIO2_END_OF_STREAM;  
    buffer.AudioBytes = cbWaveSize;
	if( p_sound->m_mode == CSoundManager::MODE_LOOP ){
		buffer.LoopCount = XAUDIO2_LOOP_INFINITE ;
	}

    if( FAILED( hr = pSourceVoice->SubmitSourceBuffer( &buffer ) ) )
    {
        pSourceVoice->DestroyVoice();
    }
    hr = pSourceVoice->Start( 0 );


    // Let the sound play
    BOOL isRunning = TRUE;
    while( SUCCEEDED( hr ) && isRunning )
    {
		if( p_sound->m_is_end ) break ;
		if( p_sound->m_next_state == CSoundManager::STATE_REPLAY ){
			p_sound->m_next_state = CSoundManager::STATE_NONE ;
			p_sound->m_state = CSoundManager::STATE_PLAY ;
			pSourceVoice->Stop(0) ;
			pSourceVoice->FlushSourceBuffers();
			pSourceVoice->SubmitSourceBuffer( &buffer ) ;
			pSourceVoice->Start(0) ;
			continue ;
		}
		if( p_sound->m_next_state == CSoundManager::STATE_STOP ){
			p_sound->m_next_state = CSoundManager::STATE_NONE ;
			p_sound->m_state = CSoundManager::STATE_STOP ;
			pSourceVoice->Stop(0) ;
			continue ;
		}

		XAUDIO2_VOICE_STATE state;
		pSourceVoice->GetState( &state );
		isRunning = ( state.BuffersQueued > 0 ) != 0;
        Sleep( 10 );
    }

    pSourceVoice->DestroyVoice();
    pMasteringVoice->DestroyVoice();
    SAFE_RELEASE( pXAudio2 );
    CoUninitialize();
	p_sound->m_is_erase_ok = true ;
}




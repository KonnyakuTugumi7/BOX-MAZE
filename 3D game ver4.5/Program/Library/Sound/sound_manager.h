#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#pragma warning ( disable : 4127 ) //条件式が定数です。
#pragma warning ( disable : 4189 ) //ローカル変数が初期化されましたが、参照されていません。

#include "DXUT.h"
#include <map>
#include <list>
#include <string>
#include <windows.h>

#include "sound.h"

#pragma comment( lib , "dsound.lib" )

//-------------------------------------------------------------
// バッファデータ
class CSoundWav{
private :
	CSoundWav() : m_p_buffer(NULL), m_is_delete_ok(false)
	{
	} ;
	~CSoundWav(){
		SAFE_DELETE( m_p_buffer ) ;
	} ;
	bool m_is_delete_ok ;
	CWaveFile m_wav ;
	BYTE* m_p_buffer ;
	friend class CSound ;
	friend class CSoundManager ;
} ;


//-------------------------------------------------------------
// マネージャ本体
class CSoundManager{

private :

	// new 禁止
	CSoundManager(){} ;

	// delete 禁止
	~CSoundManager(){} ;

	static int m_id_count ;
	static std::map< std::string, CSoundWav* > m_buffers ;
	static std::map< std::string, std::map<int,CSound*> > m_sounds ;
	static void Erase( const int sound_id ) ;
	friend class CSound ;

public :

	//-----------------------------------------------------------
	// プレイモード
	enum{
		MODE_ONCE,	// 一度だけ再生
		MODE_LOOP	// ループ再生
	} ;

	//-----------------------------------------------------------
	// 状態
	enum{
		STATE_NONE,		// 無効( 存在しない等 )
		STATE_STOP,		// 再生されていない状態
		STATE_PLAY,		// 再生中
		STATE_REPLAY,	// ( システム側で使用 )
	} ;

	//-----------------------------------------------------------
	// 更新
	// arg1... none
	// ret.... none
	// tips... 毎フレーム初めに呼び出してください
	static void Updata() ;

	//-----------------------------------------------------------
	// 状態の取得
	// arg1... サウンドID
	// ret.... 状態が帰ってきます ( STATE_NONE, STATE_STOP, STATE_PLAY )
	// tips... none
	static int GetState( const int sound_id ) ;

	//-----------------------------------------------------------
	// ロード
	// arg1... ファイルパス
	// ret.... none
	// tips... none
	static void Load( const std::string& file_name ) ;

	//-----------------------------------------------------------
	// 再生
	// arg1... ファイルパス
	// arg2... プレイモード
	// ret.... サウンドID
	// tips... 毎フレーム呼び出さない事
	// tips... 単発再生で指定して返されるサウンドIDは再生が終了すると ID が無効になります。
	static int Play( const std::string& file_name, int mode = MODE_ONCE ) ;

	//-----------------------------------------------------------
	// 停止させた BGM などのリプレイ
	// arg1... サウンドID
	// ret.... none
	// tips... 毎フレーム呼び出さない事
	static void Play( const int sound_id ) ;

	//-----------------------------------------------------------
	// 停止
	// arg1... サウンドID
	// ret.... none
	// tips... none
	static void Stop( const int sound_id ) ;

	//-----------------------------------------------------------
	// 解放
	// arg1... ファイルパス
	// ret.... none
	// tips... none
	static void Delete( const std::string& file_name ) ;

	//-----------------------------------------------------------
	// マネージャの終了処理
	// arg1... none
	// ret.... none
	// tips... ゲーム終了時に呼び出す
	static void Destroy() ;

} ;

#endif


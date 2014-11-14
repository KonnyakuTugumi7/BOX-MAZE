#include "DXUT.h"
#include "common.h"

//--------------------------------------------------------------------------

//静的変数の実体と初期化
IDirect3DDevice9* CCommon::m_pd3dDevice;

//--------------------------------------------------------------------------

// ワイドキャラへの変換
HRESULT CCommon::DXconvAnsiToWide( WCHAR* wstrDest, const CHAR* strSrc, const int ct )
{
	// 引数の*がNULLの場合と、文字数が1以下の場合はエラー
	if( wstrDest==NULL || strSrc==NULL || ct < 1 )
		return E_INVALIDARG;

	int nResult = MultiByteToWideChar( CP_ACP , 0 , strSrc , -1 , wstrDest , ct );
	wstrDest[ ct - 1 ] = 0;

	if( nResult == 0 )
		return E_FAIL;

	return S_OK;
}

//経過時間取得
float CCommon::GetElapsedTimeInSeconds()
{
	//この関数が呼び出された最後の瞬間からの経過時間(秒単位)を返す

	static const int MAX_SAMPLE_COUNT = 50;

	static float frameTimes[MAX_SAMPLE_COUNT];
	static float timeScale = 0.0f;
	static float actualElapsedTimeSec = 0.0f;
	static INT64 freq = 0;
	static INT64 lastTime = 0;
	static int sampleCount = 0;
	static bool initialized = false;

	INT64 time = 0;
	float elapsedTimeSec = 0.0f;

	if (!initialized)
	{
		initialized = true;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&freq));
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&lastTime));
		timeScale = 1.0f / freq;
	}

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&time));
	elapsedTimeSec = (time - lastTime) * timeScale;
	lastTime = time;

	if (fabsf(elapsedTimeSec - actualElapsedTimeSec) < 1.0f)
	{
		memmove(&frameTimes[1], frameTimes, sizeof(frameTimes) - sizeof(frameTimes[0]));
		frameTimes[0] = elapsedTimeSec;

		if (sampleCount < MAX_SAMPLE_COUNT)
			++sampleCount;
	}

	actualElapsedTimeSec = 0.0f;

	for (int i = 0; i < sampleCount; ++i)
		actualElapsedTimeSec += frameTimes[i];

	if (sampleCount > 0)
		actualElapsedTimeSec /= sampleCount;

	return actualElapsedTimeSec;
}

//クォータニオンを元にしたベクトルを計算(ベクトルにクォータニオンを反映)
D3DXVECTOR3 CCommon::QuaternionTransformCoord( const D3DXVECTOR3 v , const D3DXQUATERNION qtn )
{
	//ベクトルをクォータニオンへ設定
	D3DXQUATERNION pos , conjugate;
	pos.x = v.x;
	pos.y = v.y;
	pos.z = v.z;
	pos.w = 0.0f;

	//クォータニオンの共役と乗算して返す
	D3DXQuaternionMultiply( &pos , &pos , &qtn );
	D3DXQuaternionConjugate( &conjugate , &qtn );
	D3DXQuaternionMultiply( &pos , &conjugate , &pos );

	return D3DXVECTOR3( pos.x , pos.y , pos.z );
}

//クォータ二オンの差分(算出した差分をqtn1に乗算するとqtn2の姿勢になる)
D3DXQUATERNION CCommon::WsbQuaternionDelta( D3DXQUATERNION* _pQtn1, D3DXQUATERNION* _pQtn2 )
{
	D3DXQUATERNION q0i;
	D3DXQuaternionInverse( &q0i, _pQtn1 );
	D3DXQUATERNION qd = q0i * ( *_pQtn2 );
	if (qd.w < 0)
	{
		qd.x *= -1.0f;
		qd.y *= -1.0f;
		qd.z *= -1.0f;
		qd.w *= -1.0f;
	}
	return qd;
}
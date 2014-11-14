#include "DXUT.h"
#include "common.h"

//--------------------------------------------------------------------------

//�ÓI�ϐ��̎��̂Ə�����
IDirect3DDevice9* CCommon::m_pd3dDevice;

//--------------------------------------------------------------------------

// ���C�h�L�����ւ̕ϊ�
HRESULT CCommon::DXconvAnsiToWide( WCHAR* wstrDest, const CHAR* strSrc, const int ct )
{
	// ������*��NULL�̏ꍇ�ƁA��������1�ȉ��̏ꍇ�̓G���[
	if( wstrDest==NULL || strSrc==NULL || ct < 1 )
		return E_INVALIDARG;

	int nResult = MultiByteToWideChar( CP_ACP , 0 , strSrc , -1 , wstrDest , ct );
	wstrDest[ ct - 1 ] = 0;

	if( nResult == 0 )
		return E_FAIL;

	return S_OK;
}

//�o�ߎ��Ԏ擾
float CCommon::GetElapsedTimeInSeconds()
{
	//���̊֐����Ăяo���ꂽ�Ō�̏u�Ԃ���̌o�ߎ���(�b�P��)��Ԃ�

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

//�N�H�[�^�j�I�������ɂ����x�N�g�����v�Z(�x�N�g���ɃN�H�[�^�j�I���𔽉f)
D3DXVECTOR3 CCommon::QuaternionTransformCoord( const D3DXVECTOR3 v , const D3DXQUATERNION qtn )
{
	//�x�N�g�����N�H�[�^�j�I���֐ݒ�
	D3DXQUATERNION pos , conjugate;
	pos.x = v.x;
	pos.y = v.y;
	pos.z = v.z;
	pos.w = 0.0f;

	//�N�H�[�^�j�I���̋����Ə�Z���ĕԂ�
	D3DXQuaternionMultiply( &pos , &pos , &qtn );
	D3DXQuaternionConjugate( &conjugate , &qtn );
	D3DXQuaternionMultiply( &pos , &conjugate , &pos );

	return D3DXVECTOR3( pos.x , pos.y , pos.z );
}

//�N�H�[�^��I���̍���(�Z�o����������qtn1�ɏ�Z�����qtn2�̎p���ɂȂ�)
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
#pragma once

#include "DXUT.h"
#include <atlbase.h>

#pragma warning ( disable : 4189 ) //ローカル変数が初期化されましたが、参照されていません

class CCommon{
public:
	//DirectXのデバイスへのポインタ
	static IDirect3DDevice9* m_pD3dDevice;

	// ワイド文字への変換
	static HRESULT DXconvAnsiToWide( WCHAR* wstrDest, const CHAR* strSrc, const int ct );

	//経過時間取得
	static float GetElapsedTimeInSeconds();

	//クォータニオンを元にしたベクトルを計算(ベクトルにクォータニオンを反映)
	//引数1:変換前のベクトル , 引数2:クォータニオン(反映させたい角度)
	static D3DXVECTOR3 QuaternionTransformCoord( const D3DXVECTOR3 v , const D3DXQUATERNION qtn );

	//クォータ二オンの差分(算出した差分をqtn1に乗算するとqtn2の姿勢になる)
	//引数1:反映前のクォータ二オン , 引数2:反映させるクォータ二オン
	static D3DXQUATERNION  WsbQuaternionDelta( D3DXQUATERNION* _pQtn1 , D3DXQUATERNION* _pQtn2 );

};
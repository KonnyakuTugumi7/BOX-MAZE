#pragma once

#include "DXUT.h"
#include <atlbase.h>

#pragma warning ( disable : 4189 ) //���[�J���ϐ�������������܂������A�Q�Ƃ���Ă��܂���

class CCommon{
public:
	//DirectX�̃f�o�C�X�ւ̃|�C���^
	static IDirect3DDevice9* m_pD3dDevice;

	// ���C�h�����ւ̕ϊ�
	static HRESULT DXconvAnsiToWide( WCHAR* wstrDest, const CHAR* strSrc, const int ct );

	//�o�ߎ��Ԏ擾
	static float GetElapsedTimeInSeconds();

	//�N�H�[�^�j�I�������ɂ����x�N�g�����v�Z(�x�N�g���ɃN�H�[�^�j�I���𔽉f)
	//����1:�ϊ��O�̃x�N�g�� , ����2:�N�H�[�^�j�I��(���f���������p�x)
	static D3DXVECTOR3 QuaternionTransformCoord( const D3DXVECTOR3 v , const D3DXQUATERNION qtn );

	//�N�H�[�^��I���̍���(�Z�o����������qtn1�ɏ�Z�����qtn2�̎p���ɂȂ�)
	//����1:���f�O�̃N�H�[�^��I�� , ����2:���f������N�H�[�^��I��
	static D3DXQUATERNION  WsbQuaternionDelta( D3DXQUATERNION* _pQtn1 , D3DXQUATERNION* _pQtn2 );

};
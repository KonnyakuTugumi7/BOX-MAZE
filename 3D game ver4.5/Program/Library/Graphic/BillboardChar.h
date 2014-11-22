#ifndef BILLBOARDCHAR_H
#define BILLBOARDCHAR_H

#pragma warning( disable : 4996 ) //'wcsncpy' �̐錾���m�F���Ă��������B

#include "DXUT.h"
#include <string>
#include <tchar.h>

//���_�֌W
#define FVF_2DBILLBOARDVERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 ) //���W�ϊ��ςݒ��_

//�r���{�[�h�����N���X
class CBillboardChar{
protected:
	struct _2DBILLBOARDVERTEX{
		float x,y,z;		//���_���W 
		float rhw;		//���Z��
		DWORD dwColor;	//���_�̐F
		float u, v;		//�e�N�X�`�����W 
	};

public:
	enum Regist{
		Regist_OK,			//OK
		Regist_SameChar,	//��������
		Regist_Error,		//�G���[
	};

	enum BILLBOARDCHAR_RESOLUTION{
		BILLBOARDCHAR_RESOLUTION_LOW = 0,
		BILLBOARDCHAR_RESOLUTION_MIDDLE = 1,
		BILLBOARDCHAR_RESOLUTION_HIGH = 2
	};

private:
	//�r���{�[�h���_
	static const _2DBILLBOARDVERTEX m_Vertex_g[ 4 ];
	//�����R�[�h�ϊ�
	unsigned int TransCharCode( const TCHAR *c );
	//�`��f�o�C�X
	IDirect3DDevice9* m_Dev;
	//�����e�N�X�`��
	IDirect3DTexture9* m_pTex;
	//���_�o�b�t�@
	IDirect3DVertexBuffer9* m_pVertexBuf;
	//�`�敶��
	TCHAR m_Char[ 2 ];
	//�t�H���g��
	TCHAR m_lfFaceName[ LF_FACESIZE ];
	//�t�H���g�n���h��
	HFONT m_hFont;
	bool m_ISFontHandleRecreate;
	//�A�b�v�f�[�g���邩�ǂ���
	bool m_IsUpdate;
	//�A���t�@�l
	BYTE m_Alpha;
	//�t�H���g�T�C�Y
	unsigned int m_MaxPixel;
	//�r�b�g�}�b�v��
	int m_iBmp_W;
	//�r�b�g�}�b�v����
	int m_iBmp_H;
	//�r�b�g�}�b�v�I�t�Z�b�g
	int m_iBmpOfs_x, m_iBmpOfs_y;
	//�e�N�X�`���T�C�Y
	int m_TexSize;
	//�}�[�W��
	int m_R_Margin, m_B_Margin;
	//�t�H���g�J���[
	DWORD m_Color;
	//�X�P�[��
	float m_XScale, m_YScale;
	//�p�x
	float m_Angle;
	//���W
	float m_Pos_X, m_Pos_Y;
	//���S���W
	float m_Center_X, m_Center_Y;
	BILLBOARDCHAR_RESOLUTION m_resolution;

protected:
	//�r���{�[�h�e�N�X�`���쐬
	bool CreateTexture( unsigned int width, unsigned int height );

	//�t�H���g�n���h������
	bool CBillboardChar::CreateFontHandle();

	//�t�H���g�r�b�g�}�b�v�𐶐�
	DWORD CreateFontBMP( TEXTMETRIC &TM, GLYPHMETRICS &GM, BYTE **outBMP );

	//BMP���e�N�X�`���ɓK�p
	bool ApplyBMPToTex( BYTE* bmp, TEXTMETRIC &TM, GLYPHMETRICS &GM );

	//�������e�N�X�`���ɍ���
	bool PlotCharToTex();

	//�`��ʒu�X�V
	bool UpdateSizeAndPosition( float x, float y );

	//�����_�����O����
	void SetRenderState();

public:
	// �R���X�g���N�^
	CBillboardChar( IDirect3DDevice9 *dev );
	//�f�X�g���N�^
	virtual ~CBillboardChar();

	//�A�b�v�f�[�g
	bool Update();

	//�����o�^
	CBillboardChar::Regist RegistChar( const TCHAR *c );

	//��{�����R�s�[
	void CopyBaseInfoFrom( CBillboardChar* src );

	//�𑜓x�ݒ�
	void SetResolution( BILLBOARDCHAR_RESOLUTION resolution ){ m_resolution = resolution; }

	//�t�H���g�����T�C�Y�w��
	bool SetMaxPixelSize( unsigned int size )
	{
		if( size == 0 ) return false;
		if( m_MaxPixel != size )
		{
			//�����T�C�Y�ύX
			m_MaxPixel = size;
			m_IsUpdate = true;
			m_ISFontHandleRecreate = true;
		}
		return true;
	}

	//�t�H���g�X�P�[���w��
	void SetScale( float xs, float ys ){ m_XScale = xs; m_YScale = ys; }

	//��]�p�x�w��
	void SetAngle( float angle ){ m_Angle = angle; }

	//�ʒu�w��
	void SetPosition( float x, float y ){ m_Pos_X = x; m_Pos_Y = y; }

	//�t�H���g�w��
	bool SetFont( const TCHAR* FaceName )
	{
		//�����t�H���g�����w�肳��Ă���̂ōX�V���Ȃ�
		if( _tcscmp( m_lfFaceName, FaceName ) == 0 ) return true;
		_tcsncpy( m_lfFaceName, FaceName, LF_FACESIZE-1 );
		m_IsUpdate = true;
		m_ISFontHandleRecreate = true;
		return true;
	}

	//�Z���^�[�ʒu�w��
	void SetCenterPos( float x, float y ){ m_Center_X = x; m_Center_Y = y; }

	//�A���t�@�l�w��
	void SetAlpha( int alpha )
	{
		//�A���t�@�l����
		if( alpha >= 256 ) alpha = 255;
		if( alpha <= 0 ) alpha = 0;

		m_Alpha = ( BYTE )alpha;
	}

	//�����̃x�[�X�J���[�w��
	void SetColor( DWORD color ){ m_Color = color; }

	//�t�H���g�e�N�X�`���擾
	IDirect3DTexture9* GetFontTexture()
	{
		if( m_pTex ) m_pTex->AddRef();
		return m_pTex;
	}

	//�\���������擾
	float GetDispWidth(){ return ( m_iBmpOfs_x + m_iBmp_W ) * m_XScale; }

	//�\�������E�ʒu�擾
	float GetRightPos(){ return ( m_iBmpOfs_x + m_iBmp_W + m_R_Margin ) * m_XScale + m_Pos_X; }

	//�`��
	bool Draw();
};

#endif
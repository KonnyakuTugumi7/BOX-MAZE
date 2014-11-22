#ifndef BILLBOARDSTRING_H
#define BILLBOARDSTRING_H

#include "DXUT.h"
#include "../../Library/Graphic/BillboardChar.h"
#include <vector>
#include <memory>

//�������`
namespace FACE_NAME
{
	const std::wstring MS_GOTHIC(L"�l�r �S�V�b�N");
	const std::wstring MS_MINCHOU(L"�l�r ����");
	const std::wstring HG_CIRCLE_GOTHIC(L"HG�ۺ޼��M-PRO");
	const std::wstring HGP_POP(L"HGP�n�p�p�߯�ߑ�");
	const std::wstring MEIRIO(L"���C���I");
}

class CBillboardString;
typedef std::shared_ptr< CBillboardString > BillboardStringSP;

//�r���{�[�h������N���X
//�f�t�H���g�l:32�s�N�Z���T�C�Y , ���F , ���ߓx255(�s����) , MS�S�V�b�N
class CBillboardString{
private:
	//�f�o�C�X
	IDirect3DDevice9* m_Dev;
	//�r���{�[�h������
	std::vector< CBillboardChar* > m_StrAry;
	//���W���X�V���邩�ǂ���
	bool m_IsUpdatePosition;
	//�A�b�v�f�[�g(��������č\�����邩�ǂ���)
	bool m_IsUpdate;
	//���ݓo�^����Ă��镶����̒���
	unsigned int m_CurLen;

protected:
	//�J���[
	DWORD m_Color;
	//�w�i�|���J���[
	DWORD m_BGColor;
	//�s�N�Z���T�C�Y
	unsigned int m_PixelSize;
	//�A���t�@
	int m_Alpha;
	//�T�C�Y
	D3DXVECTOR2 m_Scale;
	//���W
	D3DXVECTOR2 m_BasePos;
	//�t�F�C�X���O
	TCHAR m_FaceName[ LF_FACESIZE ];

protected:
	//�ʒu�֌W�X�V
	void UpdatePositions();

	//�X�V
	void Update();

public:
	//�R���X�g���N�^
	//dev:�O���t�B�b�N�f�o�C�X
	CBillboardString( IDirect3DDevice9* dev );
	//�f�X�g���N�^
	~CBillboardString();

	//����
	//dev:�O���t�B�b�N�f�o�C�X
	static BillboardStringSP Create( IDirect3DDevice9* dev ){ return std::make_shared< CBillboardString >( dev ); }

	//--------------------�č\�z���K�v--------------------
	//������o�^(���ɓo�^����Ă��镶����͔j�����A�V�����t�H���g�e�N�X�`�����쐬������������̏ꍇ�͕ύX�Ȃ�)
	//str:������
	void RegistString( const TCHAR* str );

	//�t�H���g�ݒ�
	//FaceName:�t�F�[�X(�����R�[�h)��
	bool SetFont( const TCHAR* FaceName );

	//�t�H���g���͂ރ{�b�N�X�̍ő啝��ݒ�(���ۂ̃t�H���g�̑傫���͂��̒l���኱�������Ȃ�)
	//size:�t�H���g�ő�T�C�Y
	bool SetMaxPixelSize( unsigned int size );
	//----------------------------------------------------

	//�t�H���g��\��t����|���S���̃X�P�[���ݒ�(��{�I��SetMaxPixelSize�Őݒ肵���t�H���g�̃T�C�Y�Ɠ��{�ɂ��ׂ�)
	//xs:x�X�P�[�� , ys:y�X�P�[��
	void SetScale( float xs, float ys );

	//�ʒu�ݒ�
	//x:x���W , y:y���W
	void SetPosition( float x, float y );

	//�A���t�@�l�ݒ�
	//alpha:�A���t�@�l( 0�`255 )
	void SetAlpha( int alpha );

	//�����̃x�[�X�J���[�ݒ�
	//color:�J���[�l(XRGB)
	void SetColor( DWORD color );

	//�`��
	bool Draw();
};

#endif
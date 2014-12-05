#pragma once

#include "../../GameEngine/Graphic/board.h"
#include "../../GameEngine/Camera/camera_base.h"
#include "../../Library/Graphic/texture.h"
#include <D3dx9core.h>
#include <string>
#include <memory>
#include <hash_map>

#pragma warning ( disable : 4996 )

class CFont;
typedef std::shared_ptr< CFont > FontSP;

//�t�H���g�N���X
class CFont : public CBoard{
private:
	//�e�N�X�`���̃f�[�^�}�b�v
	std::hash_map<wchar_t, LPDIRECT3DTEXTURE9> m_data;

	//����
	enum Align;
	Align m_align;

	//�X�P�[��
	D3DXVECTOR3 m_scale;

private:
	//�`��
	//����1:�J����
	virtual void Render( const CameraBaseWP camera );

public:
	//����
	static FontSP Create(){ return std::make_shared< CFont >(); }

	//�R���X�g���N�^
	CFont() : m_align( CFont::LEFT ) , m_scale( 1.0f , 1.0f , 1.0f )
	{
		Init( 1 , 1 );
		m_texs.push_back( CTexture::CreateEmptyTex( 1.0f , 1.0f ) );
	}

	//�f�X�g���N�^
	~CFont();

	//�t�H���g�̐���
	enum Align { LEFT, RIGHT, CENTER };

	//�t�H���g�e�N�X�`���𐶐�
	//size: �傫�� , weight: ���� , type: ���� , fonts: �������镶����
	bool CreateTexture( std::size_t size , std::size_t weight , const std::wstring& type , const std::wstring& fonts );

	//�`�揈��(������̓t�H���g�e�N�X�`������������Ă��Ȃ���Ε`�悳��Ȃ�)
	//camera: �J���� , string: ������ , pos: ���W , color: �F(RGBA)
	void Draw( const CameraBaseWP camera , const std::wstring& string , const D3DXVECTOR2& pos , const D3DXVECTOR4& color );

	//���񏇂�ݒ�
	void SetAlign( const Align& align ){ m_align = align; }

	//�傫����ݒ�
	//width: �� , height: ����
	void SetSize( const float width , const float height ) { m_scale.x = width; m_scale.y = height; }
};

//�������`
namespace FontType
{
	const std::wstring MS_GOTHIC(L"�l�r �S�V�b�N");
	const std::wstring MS_MINCHOU(L"�l�r ����");
	const std::wstring HG_CIRCLE_GOTHIC(L"HG�ۺ޼��M-PRO");
	const std::wstring HGP_POP(L"HGP�n�p�p�߯�ߑ�");
	const std::wstring MEIRIO(L"���C���I");

}

//���p�p�x�̍����������`�i�e�N�X�`�������p�j
namespace FontUseful
{
	const std::wstring NUMBER(L"0123456789");
	const std::wstring ALPHABET_SMALL(L"abcdefghijklmnopqrstuvwxyz");
	const std::wstring ALPHABET_LARGE(L"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	const std::wstring ALPHABET(ALPHABET_SMALL + ALPHABET_LARGE);
	const std::wstring HIRAGANA_A(L"��������������������");
	const std::wstring HIRAGANA_KA(L"��������������������");
	const std::wstring HIRAGANA_SA(L"��������������������");
	const std::wstring HIRAGANA_TA(L"�����ĂƂ����Âł�");
	const std::wstring HIRAGANA_NA(L"�Ȃɂʂ˂�");
	const std::wstring HIRAGANA_HA(L"�͂Ђӂւق΂тԂׂڂς҂Ղ؂�");
	const std::wstring HIRAGANA_MA(L"�܂݂ނ߂�");
	const std::wstring HIRAGANA_YA(L"�������");
	const std::wstring HIRAGANA_RA(L"������");
	const std::wstring HIRAGANA_WA(L"�������");
	const std::wstring HIRAGANA(HIRAGANA_A + HIRAGANA_KA + HIRAGANA_SA + HIRAGANA_TA + HIRAGANA_NA + HIRAGANA_HA + HIRAGANA_MA + HIRAGANA_YA + HIRAGANA_RA + HIRAGANA_WA);
	const std::wstring KATAKANA_A(L"�A�C�E�G�I�@�B�D�F�H");
	const std::wstring KATAKANA_KA(L"�J�L�N�P�R�K�M�O�Q�S");
	const std::wstring KATAKANA_SA(L"�T�V�X�Z�\�U�W�Y�[�]");
	const std::wstring KATAKANA_TA(L"�^�`�c�e�g�_�a�d�f�h");
	const std::wstring KATAKANA_NA(L"�i�j�k�l�m");
	const std::wstring KATAKANA_HA(L"�n�q�t�w�z�o�r�u�x�{�p�s�v�y�|");
	const std::wstring KATAKANA_MA(L"�}�~������");
	const std::wstring KATAKANA_YA(L"������������");
	const std::wstring KATAKANA_RA(L"����������");
	const std::wstring KATAKANA_WA(L"����������");
	const std::wstring KATAKANA(KATAKANA_A + KATAKANA_KA + KATAKANA_SA + KATAKANA_TA + KATAKANA_NA + KATAKANA_HA + KATAKANA_MA + KATAKANA_YA + KATAKANA_RA + KATAKANA_WA);

}

//�F���`
namespace Color
{
	const D3DXVECTOR3 WHITE(1.0f, 1.0f, 1.0f);
	const D3DXVECTOR3 BLACK(0.0f, 0.0f, 0.0f);
	const D3DXVECTOR3 YELLOW(1.0f, 1.0f, 0.0f);
	const D3DXVECTOR3 GREEN(0.0f, 1.0f, 0.0f);
	const D3DXVECTOR3 RED(1.0f, 0.0f, 0.0f);
	const D3DXVECTOR3 BLUE(0.0f, 0.0f, 1.0f);
	const D3DXVECTOR3 LIGHT_BLUE(0.0f, 1.0f, 1.0f);
	const D3DXVECTOR3 PURPLE(1.0f, 0.0f, 1.0f);
}

class CDebugFont;
typedef std::shared_ptr< CDebugFont > DebugFontSP;

//�t�H���g�N���X
class CDebugFont{
private:
	//�t�H���g
	ID3DXFont* m_font;

public:
	//�t�H���g�J���[
	enum FontColor{
		//��
		BLACK = 0xFF000000 ,

		//��
		WHITE = 0xFFFFFFFF ,

		//��
		RED = 0xFFFF0000 ,

		//��
		GREEN = 0xFF00FF00 ,

		//��
		BLUE = 0xFF0000FF ,

		//��
		YELLOW = 0xFFFFF000
	};

public:
	//�R���X�g���N�^
	CDebugFont(){}

	//�f�X�g���N�^
	~CDebugFont(){ SAFE_RELEASE( m_font ); }

	//����
	//����1:������̕� , ����2:������̍��� , ����3:�t�H���g�^�C�v(0:�S�V�b�N , 1:����)
	static DebugFontSP Create( const int width , const int height , const int type = 0 );

	//������
	//����1:������̕� , ����2:������̍��� , ����3:�t�H���g�^�C�v(0:�S�V�b�N , 1:����)
	void Init( const int width , const int height , const int type );

	//�`��
	//����1:�`�悷�镶���� , ����2:�`����W(x) , ����3:�`����W(y) , ����4:�����̐F
	void DrawFont( const std::string str , const int posX , const int posY , const FontColor fontcolor );
};
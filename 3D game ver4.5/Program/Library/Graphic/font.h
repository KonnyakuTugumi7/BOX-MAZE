#pragma once

#include "../../GameEngine/Graphic/board.h"
#include <D3dx9core.h>
#include <string>
#include <memory>

#pragma warning ( disable : 4996 )

class CFont;
typedef std::shared_ptr< CFont > FontSP;

//�t�H���g�N���X
class CFont{
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
	CFont(){}

	//�f�X�g���N�^
	~CFont(){ SAFE_RELEASE( m_font ); }

	//����
	//����1:������̕� , ����2:������̍��� , ����3:�t�H���g�^�C�v(0:�S�V�b�N , 1:����)
	static FontSP Create( const int width , const int height , const int type = 0 );

	//������
	//����1:������̕� , ����2:������̍��� , ����3:�t�H���g�^�C�v(0:�S�V�b�N , 1:����)
	void Init( const int width , const int height , const int type );

	//�`��
	//����1:�`�悷�镶���� , ����2:�`����W(x) , ����3:�`����W(y) , ����4:�����̐F
	void DrawFont( const std::string str , const int posX , const int posY , const FontColor fontcolor );
};

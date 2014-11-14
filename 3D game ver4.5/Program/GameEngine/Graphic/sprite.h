#pragma once

#include "DXUT.h"
#include "../../Library/Graphic/animation.h"
#include "../../Library/Graphic/graphic_base.h"
#include "../../Library/Graphic/graphics_manager.h"
#include <string>
#include <memory>

#pragma warning ( disable : 4390 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4238 )

class CCameraBase;
typedef std::weak_ptr< CCameraBase > CameraBaseWP;

class CSprite;
typedef std::shared_ptr< CSprite > SpriteSP;

//�X�v���C�g�N���X
class CSprite : public CAnimation , public CGraphicBase{
private:
	//�X�v���C�g(DirectX��2D�������N���X)
	LPD3DXSPRITE m_sprite;

	//�t�@�C����
	std::string m_name;

public:
	//�R���X�g���N�^
	CSprite(){}

	//�f�X�g���N�^
	~CSprite()
	{
		//�X�v���C�g�̉��
		SAFE_RELEASE( m_sprite );
	}

	//����
	//����1:�t�@�C���̖��O , ����2:�����_�����O���X�g�̎�� , ����3:���ɕ������鐔 , ����4:�c�ɕ������鐔
	static SpriteSP Create( const std::string file_name , const CGraphicsManager::RENDERLIST_STATE list , const int x_num = 1 , const int y_num = 1 );

	//������
	//����1:�t�@�C���̖��O , ����2:���ɕ������鐔 , ����3:�c�ɕ������鐔
	void Init( const std::string file_name , const int x_num = 1 , const int y_num = 1 );

	//�`��
	//����1:�J����
	void Render( const CameraBaseWP camera );
};
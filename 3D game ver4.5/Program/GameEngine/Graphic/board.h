#pragma once

#include "DXUT.h"
#include "../../Library/Graphic/graphic_base.h"
#include "../../Library/Graphic/graphics_manager.h"
#include <memory>
#include <string>

#pragma warning ( disable : 4244 )

class CCube;
//typedef std::weak_ptr< CCube > CubeWP;

class CBoard;
typedef std::shared_ptr< CBoard > BoardSP;

//�|���N���X
class CBoard : public CGraphicBase{
protected:
	//�r���{�[�h����t���O
	bool m_IsBillboard;

	//�|���T�C�Y
	D3DXVECTOR3 m_BoardSize;

	//�L���[�u�̃E�B�N�|�E�E�ECube(�e)�̃��[���h�}�g���b�N�X���擾���邽��
	std::weak_ptr< CCube > m_Cube;

public:
	//�R���X�g���N�^
	CBoard(){}

	//�R���X�g���N�^
	//����1:�{�[�h�̕� , ����2:�{�[�h�̍��� , ����3:�e�N�X�`���p�X
	CBoard( const int width , const int height , const std::string path = "" ){ Init( width , height , path ); }

	//�f�X�g���N�^
	virtual ~CBoard(){}

	//����
	//����1:�{�[�h�̕� , ����2:�{�[�h�̍��� , ����3:�e�N�X�`���p�X , ����4:�����_�����O���X�g�̎��
	static BoardSP Create( const int width , const int height , const std::string path = "" , const CGraphicsManager::RENDERLIST_STATE list = CGraphicsManager::NORMAL_3D );

	//������
	//����1:�{�[�h�̕� , ����2:�{�[�h�̍��� , ����3:�e�N�X�`���p�X
	void Init( const int width , const int height , const std::string path = "" );

	//�`��
	//����1:�J����
	virtual void Render( const CameraBaseWP camera );

	//���[���h�}�g���b�N�X�ݒ�(billboard�p) //�J�����̕����������p
	//����1:�J���� , ����2:�e�̃��[���h�}�g���b�N�X
	void SetWldMtxBillBoard( const CameraBaseWP camera );

	//�L���[�u�E�B�[�N�|�C���^�Z�b�^�[
	inline void SetCubeWP( const std::weak_ptr< CCube > cube ){ m_Cube = cube; }

	//�e�N�X�`���Z�b�^�[
	//����1:�e�N�X�`��(�|���ɂ͍��̂Ƃ���ꖇ�ɂ���̃e�N�X�`�������ݒ肵�Ă��Ȃ��̂ł���ł�k)
	void SetTex( const TextureSP tex );
};
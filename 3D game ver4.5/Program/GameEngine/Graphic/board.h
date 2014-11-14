#pragma once

#include "DXUT.h"
#include "../../Library/Graphic/graphic_base.h"
#include <memory>
#include <string>

#pragma warning ( disable : 4244 )

class CGraphicBase;
class CCube;
typedef std::weak_ptr< CCube > CubeWP;
class CCameraBase;
typedef std::weak_ptr< CCameraBase > CameraBaseWP;

class CBoard;
typedef std::shared_ptr< CBoard > BoardSP;

//�|���N���X
class CBoard : public CGraphicBase{
protected:
	//�r���{�[�h����t���O
	bool m_is_billboard;

	//�|���T�C�Y
	D3DXVECTOR3 m_board_size;

	//�L���[�u�̃E�B�N�|�E�E�ECube(�e)�̃��[���h�}�g���b�N�X���擾���邽��
	CubeWP m_cube;

public:
	//�R���X�g���N�^
	CBoard(){}

	//�R���X�g���N�^
	//����1:�{�[�h�̕� , ����2:�{�[�h�̍��� , ����3:�e�N�X�`���p�X
	CBoard( const int width , const int height , const std::string path = "" ){ Init( width , height , path ); }

	//�f�X�g���N�^
	~CBoard(){}

	//����
	//����1:�{�[�h�̕� , ����2:�{�[�h�̍��� , ����3:�e�N�X�`���p�X
	static BoardSP Create( const int width , const int height , const std::string path = "" );

	//������
	//����1:�{�[�h�̕� , ����2:�{�[�h�̍��� , ����3:�e�N�X�`���p�X
	void Init( const int width , const int height , const std::string path = "" );

	//�`��
	//����1:�J����
	virtual void Render( const CameraBaseWP camera );

	//�L���[�u�E�B�[�N�|�C���^�Z�b�^�[
	inline void SetCubeWP( const CubeWP cube ){ m_cube = cube; }

	//�e�N�X�`���Z�b�^�[
	//����1:�e�N�X�`��(�|���ɂ͍��̂Ƃ���ꖇ�ɂ���̃e�N�X�`�������ݒ肵�Ă��Ȃ��̂ł���ł�k)
	inline void SetTex( const TextureSP tex ){ m_texs.clear();m_texs.push_back( tex ); }
};
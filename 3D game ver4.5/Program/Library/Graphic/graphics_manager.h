#pragma once

#include "DXUT.h"
#include "../../Library/Graphic/graphic_base.h"
#include "../../Library/Graphic/shader_base.h"
#include "../../Library/Graphic/shader_depth_buffer_shadow.h"
#include "../../Library/Graphic/shader_directional_light.h"
#include "../../Library/Graphic/shader_lambert.h"
#include "../../Game/Camera/view_camera.h"
#include <list>
#include <vector>

#pragma warning ( disable : 4238 ) //��W���̊g���@�\���g�p����Ă��܂� : �E�Ӓl�̃N���X�����Ӓl�Ɏg�p����܂��B
#pragma warning ( disable : 4482 ) //��W���̊g���@�\���g�p����Ă��܂�: enum 'CGraphicsManager::RENDERLIST_STATE' �����薼�Ŏg�p����܂�

//�O���t�B�b�N�X�֘A�����}�l�[�W��
class CGraphicsManager{
public:
	//�`��Ɏg�p����DirectX�̃f�o�C�X�ւ̃|�C���^
	static IDirect3DDevice9* m_pD3dDevice;

	//�g�p����V�F�[�_
	static ShaderBaseSP m_SdrUse;

	//��{�V�F�[�_
	static ShaderBaseSP m_SdrBase;

	//���s�����V�F�[�_
	static ShaderDirectionalLightSP m_SdrDirLight;

	//�����_�[���X�g
	static std::vector< std::list< GraphicBaseSP > > m_RenderList;

	//3D�J����
	static CameraBaseSP m_3dCamera;

	//2D�J����
	static CameraBaseSP m_2dCamera;

	//�����_�����O���X�g�̎��
	static enum RENDERLIST_STATE{
		BACK_2D ,
		NORMAL_3D ,
		ALPHA_3D ,
		FRONT_2D ,
		STATE_NUM
	};

	//�E�B���h�E��
	static float m_WindowWidth;

	//�E�B���h�E����
	static float m_WindowHeight;

private:
	//�R���X�g���N�^
	CGraphicsManager(){}

	//�f�X�g���N�^
	~CGraphicsManager(){}

	//�R�s�[�R���X�g���N�^
	CGraphicsManager( const CGraphicsManager& obj ){}

	// = ���Z�q�̃I�[�o�[���[�h
	CGraphicsManager& operator = ( const CGraphicsManager& obj ){}

public:
	//������
	//����1:DirectX�̃f�o�C�X�̃|�C���^
	static void Initialize( IDirect3DDevice9* pd3dDevice );
	
	//���
	static void Destroy();

	//�`��
	static void SysRender( const CameraBaseWP camera );

	//�X�V
	static void Update();

	//�I�u�W�F�N�g�o�^
	//����1:�o�^����I�u�W�F�N�g , ����2:
	static void RegistObj( const GraphicBaseSP obj , const RENDERLIST_STATE list );
};


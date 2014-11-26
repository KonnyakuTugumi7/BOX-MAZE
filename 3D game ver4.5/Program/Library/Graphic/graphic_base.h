#pragma once 

#include "../../Library/Graphic/transform.h"
#include "../../Library/Graphic/material.h"
#include "../../Library/Graphic/mesh.h"
#include "../../Library/Graphic/texture.h"
#include "../../GameEngine/Camera/camera_base.h"
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <memory>

#pragma warning ( disable : 4018 ) //signed �� unsigned �̐��l���r���悤�Ƃ��܂����B
#pragma warning ( disable : 4238 ) //��W���̊g���@�\���g�p����Ă��܂� : �E�Ӓl�̃N���X�����Ӓl�Ɏg�p����܂��B

class CGraphicBase;
typedef std::shared_ptr< CGraphicBase > GraphicBaseSP;
typedef std::weak_ptr< CGraphicBase > GraphicBaseWP;

//�O���t�B�b�N�x�[�X�N���X
class CGraphicBase : public CTransform , public CMaterial{
public:
//�t�����h�N���X
friend class CGraphicsManager;

protected:
	//�`��t���O
	bool m_is_render;

	//�����蔻��t���O(�����蔻����s�����ǂ���)
	bool m_is_collision;

	//�\�[�g�̊�ƂȂ�l(�J��������̋���)
	float m_camera_distance;

	//�J�����O�̃X�e�[�^�X
	D3DCULL m_cull_state;

	//���b�V��
	MeshSP m_mesh;

	//�e�N�X�`��
	std::vector< TextureSP > m_texs;

public:
	//�`��Ɏg���V�F�[�_�̎��
	enum SHADER_STATE{
		//��{�V�F�[�_
		SDR_BASE = 0 , 

		//���s�����V�F�[�_
		SDR_DIR_LIGHT = 1
	} m_sdr_state;

public:
	//�R���X�g���N�^
	CGraphicBase() : m_is_render( true ) ,
					 m_is_collision( true ) , 
					 m_camera_distance( 1.0f ) ,
					 m_cull_state( D3DCULL_CCW ) , 
					 m_sdr_state( SDR_DIR_LIGHT ) ,
					 m_mesh( NULL )
	{}

	//�f�X�g���N�^
	virtual ~CGraphicBase(){ m_texs.clear(); m_mesh.reset(); }

	//�`��
	//����1:�J����
	virtual void Render( const CameraBaseWP camera );

	//���W�b�N
	virtual void Move(){}

	//�V�F�[�_�ݒ�
	void SetSDR();

	// std::list �̃\�[�g�Ŏg�p����(3D)
	static bool comp3D( const GraphicBaseSP riLeft , const GraphicBaseSP riRight )
	{
		return ( riLeft->m_camera_distance > riRight->m_camera_distance );
	}

	// std::list �̃\�[�g�Ŏg�p����(3D)
	static bool comp3DAlpha( const GraphicBaseSP riLeft , const GraphicBaseSP riRight )
	{
		return ( riLeft->m_camera_distance < riRight->m_camera_distance );
	}

	// std::list �̃\�[�g�Ŏg�p����(2D)
	static bool comp2D(const GraphicBaseSP riLeft, const GraphicBaseSP riRight)
	{
		return ( riLeft->m_camera_distance > riRight->m_camera_distance );
    }

	//���b�V���Q�b�^�[
	inline MeshSP GetMeshSP(){ return m_mesh; }

	//�`��t���O�Q�b�^�[
	inline bool GetIsRender(){ return m_is_render; }

	//�`��t���O�Z�b�^�[
	inline void SetIsRender( const bool render ){ m_is_render = render; }

	//�����蔻��t���O�Q�b�^�[
	inline bool GetIsCollision(){ return m_is_collision; }

	//�����蔻��t���O�Z�b�^�[
	inline void SetIsCollision( const bool flg ){ m_is_collision = flg; }

	//�J��������̋����Z�b�^�[
	inline void SetCameraDistance( const float distance ){ m_camera_distance = distance; }
};
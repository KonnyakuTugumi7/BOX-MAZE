#include "DXUT.h"
#include "graphics_manager.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/vertex.h"
#include "../../GameEngine/Camera/camera_base.h"
#include "../../GameEngine/Collision/collision_manager.h"

//graphics_manager�N���X�̐ÓI�����o�̎���
IDirect3DDevice9* CGraphicsManager::m_pd3dDevice;
ShaderBaseSP CGraphicsManager::m_sdr_use = NULL;
ShaderBaseSP CGraphicsManager::m_sdr_base = NULL;
ShaderDirectionalLightSP CGraphicsManager::m_sdr_dir_light = NULL;
std::vector< std::list< GraphicBaseSP > > CGraphicsManager::m_render_list;
float CGraphicsManager::m_window_width = 800.0f;
float CGraphicsManager::m_window_height = 600.0f;

//������
void CGraphicsManager::Initialize( IDirect3DDevice9* pd3dDevice )
{
	//�`��g�p����f�o�C�X�̃|�C���^
	m_pd3dDevice = pd3dDevice;

	//��{�V�F�[�_�쐬
	m_sdr_base = CShaderBase::Create( "Content/hlsl/simple.fx" );

	//���s�����V�F�[�_�쐬
	m_sdr_dir_light = CShaderDirectionalLight::Create( "Content/hlsl/directional_light.fx" );

	//�����_�����O���X�g�쐬
	for( int i = 0 ; i < STATE_NUM ; ++i )
	{
		std::list< GraphicBaseSP > list;
		m_render_list.push_back( list );
	}
}

//���
void CGraphicsManager::Destroy()
{
	for( int i = 0 ; i < RENDERLIST_STATE::STATE_NUM ; ++i )
	{
		//�����_�[���X�g���
		std::list< GraphicBaseSP >::iterator it = m_render_list[ i ].begin();
		while( it != m_render_list[ i ].end() )
		{
			( *it ).reset();
			++it;
		}
		m_render_list[ i ].clear();
	}
	m_sdr_use.reset();
	m_sdr_base.reset();
	m_sdr_dir_light.reset();
}

//�`��
void CGraphicsManager::SysRender( const CameraBaseWP camera )
{
	//�C�e���[�^
	std::list< GraphicBaseSP >::iterator it;

	//�����_�����O���X�g�̒��g���\�[�g(2D)
	m_render_list[ RENDERLIST_STATE::BACK_2D ].sort( CGraphicBase::comp2D );
	m_render_list[ RENDERLIST_STATE::FRONT_2D ].sort( CGraphicBase::comp2D );

	for( int i = 0 ; i < 2 ; ++i )
	{
		it = m_render_list[ i + 2 ].begin();

		//�J�����ƃI�u�W�F�N�g�̋������v�Z
		while( it != m_render_list[ i + 2 ].end() )
		{
			//�����v�Z
			( *it )->m_camera_distance = D3DXVec3Length( &( ( *it )->m_pos - camera.lock()->GetPosition() ) );

			++it;
		}
	}

	//�����_�����O���X�g�̒��g���\�[�g(3D)
	m_render_list[ RENDERLIST_STATE::NORMAL_3D ].sort( CGraphicBase::comp3D );
	m_render_list[ RENDERLIST_STATE::ALPHA_3D ].sort( CGraphicBase::comp3DAlpha );

	//�����_�����O���X�g�̒��g��`��
	for( int i = 0 ; i < RENDERLIST_STATE::STATE_NUM ; ++i )
	{
		it = m_render_list[ i ].begin();

		while( it != m_render_list[ i ].end() )
		{
			if( ( *it )->m_is_render == true )
			{
				( *it )->Render( camera );
			}
			++it;
		}
	}
}

//�X�V
void CGraphicsManager::Update()
{
	for( int i = 0 ; i < RENDERLIST_STATE::STATE_NUM ; ++i )
	{
		std::list< GraphicBaseSP >::iterator it = m_render_list[ i ].begin();
		while( it != m_render_list[ i ].end() )
		{
			//�J�E���g����1(list��scene�̃I�u�W�F�N�g��shared_ptr�ɂ��Ă���̂�)�̃I�u�W�F�N�g�����
			if( ( *it ).use_count() == 1 )
			{
				( *it ).reset();
				it = m_render_list[ i ].erase( it );
				continue;
			}
			++it;
		}
	}
}

//�I�u�W�F�N�g�o�^
void CGraphicsManager::RegistObj( const GraphicBaseSP obj , const RENDERLIST_STATE list )
{
	m_render_list[ list ].push_back( obj );
}
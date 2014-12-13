#include "DXUT.h"
#include "graphics_manager.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/vertex.h"
#include "../../GameEngine/Camera/camera_base.h"
#include "../../GameEngine/Collision/collision_manager.h"
#include "../../Game/Camera/fp_camera.h"

//graphics_manager�N���X�̐ÓI�����o�̎���
IDirect3DDevice9* CGraphicsManager::m_pD3dDevice;
ShaderBaseSP CGraphicsManager::m_SdrUse = NULL;
ShaderBaseSP CGraphicsManager::m_SdrBase = NULL;
ShaderDirectionalLightSP CGraphicsManager::m_SdrDirLight = NULL;
std::vector< std::list< GraphicBaseSP > > CGraphicsManager::m_RenderList;
float CGraphicsManager::m_WindowWidth = 800.0f;
float CGraphicsManager::m_WindowHeight = 600.0f;
CameraBaseSP CGraphicsManager::m_2dCamera;
CameraBaseSP CGraphicsManager::m_3dCamera;

//������
void CGraphicsManager::Initialize( IDirect3DDevice9* pd3dDevice )
{
	//�`��g�p����f�o�C�X�̃|�C���^
	m_pD3dDevice = pd3dDevice;

	//��{�V�F�[�_�쐬
	m_SdrBase = CShaderBase::Create( "Content/hlsl/simple.fx" );

	//���s�����V�F�[�_�쐬
	m_SdrDirLight = CShaderDirectionalLight::Create( "Content/hlsl/directional_light.fx" );

	//�����_�����O���X�g�쐬
	for( int i = 0 ; i < STATE_NUM ; ++i )
	{
		std::list< GraphicBaseSP > list;
		m_RenderList.push_back( list );
	}

	//�J����
	m_2dCamera = CViewCamera::Create();
	m_3dCamera = CFPCamera::Create();
}

//���
void CGraphicsManager::Destroy()
{
	for( int i = 0 ; i < RENDERLIST_STATE::STATE_NUM ; ++i )
	{
		//�����_�[���X�g���
		std::list< GraphicBaseSP >::iterator it = m_RenderList[ i ].begin();
		while( it != m_RenderList[ i ].end() )
		{
			( *it ).reset();
			++it;
		}
		m_RenderList[ i ].clear();
	}
	m_SdrUse.reset();
	m_SdrBase.reset();
	m_SdrDirLight.reset();
}

//�`��
void CGraphicsManager::SysRender( const CameraBaseWP camera )
{
	//�C�e���[�^
	std::list< GraphicBaseSP >::iterator it;

	//�����_�����O���X�g�̒��g���\�[�g(2D)
	m_RenderList[ RENDERLIST_STATE::BACK_2D ].sort( CGraphicBase::comp2D );
	m_RenderList[ RENDERLIST_STATE::FRONT_2D ].sort( CGraphicBase::comp2D );

	for( int i = 0 ; i < 2 ; ++i )
	{
		it = m_RenderList[ i + 1 ].begin();

		//�J�����ƃI�u�W�F�N�g�̋������v�Z
		while( it != m_RenderList[ i + 1 ].end() )
		{
			//�����v�Z
			( *it )->m_CameraDistance = D3DXVec3Length( &( ( *it )->m_Pos - camera.lock()->GetPosition() ) );

			++it;
		}
	}

	//�����_�����O���X�g�̒��g���\�[�g(3D)
	m_RenderList[ RENDERLIST_STATE::NORMAL_3D ].sort( CGraphicBase::comp3D );
	m_RenderList[ RENDERLIST_STATE::ALPHA_3D ].sort( CGraphicBase::comp3DAlpha );

	//�����_�����O���X�g�̒��g��`��
	for( int i = 0 ; i < RENDERLIST_STATE::STATE_NUM ; ++i )
	{
		it = m_RenderList[ i ].begin();

		if( i == RENDERLIST_STATE::ALPHA_3D || i == RENDERLIST_STATE::FRONT_2D )
		{
			HRESULT hr;
			//z�o�b�t�@�̂݃N���A
			V( CGraphicsManager::m_pD3dDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255 , 0 , 0 , 0/*255, 0, 0, 255*/ ), 1.0f, 0 ) );
		}

		if( i == RENDERLIST_STATE::NORMAL_3D || i == RENDERLIST_STATE::ALPHA_3D ) CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
		else CGraphicsManager::m_pD3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );

		while( it != m_RenderList[ i ].end() )
		{
			if( ( *it )->m_IsRender == true )
			{
				if( i == BACK_2D || i == FRONT_2D ) ( *it )->Render( m_2dCamera );
				else ( *it )->Render( m_3dCamera );
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
		std::list< GraphicBaseSP >::iterator it = m_RenderList[ i ].begin();
		while( it != m_RenderList[ i ].end() )
		{
			//�J�E���g����1(list��scene�̃I�u�W�F�N�g��shared_ptr�ɂ��Ă���̂�)�̃I�u�W�F�N�g�����
			if( ( *it ).use_count() == 1 )
			{
				( *it ).reset();
				it = m_RenderList[ i ].erase( it );
				continue;
			}
			++it;
		}
	}
}

//�I�u�W�F�N�g�o�^
void CGraphicsManager::RegistObj( const GraphicBaseSP obj , const RENDERLIST_STATE list )
{
	m_RenderList[ list ].push_back( obj );
}
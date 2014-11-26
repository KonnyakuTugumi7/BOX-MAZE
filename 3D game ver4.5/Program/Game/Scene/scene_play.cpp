#include "DXUT.h"
#include "scene_play.h"
#include "../../Library/Support/common.h"
#include "../../Library/Input/input.h"
#include "../../Library/Sound/sound.h"
#include "../../Library/Sound/sound_manager.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Graphic/font.h"
#include "../../GameEngine/Collision/collision_manager.h"
#include "../../GameEngine/Scene/scene_manager.h"
#include "../../GameEngine/Graphic/sprite.h"
#include "../../Game/Scene/scene_title.h"
#include "../../Game/Camera/fp_camera.h"
#include "../../Game/Camera/tp_camera.h"
#include <sstream>

//������
void CScenePlay::SceneInit()
{
	//CSV
	m_csv = CCsv::Create( "Content/csv/StageData.csv" );

	//BGM���[�h
	CSoundManager::Load( "Content/sound/play.wav" );
	CSoundManager::Load( "Content/sound/fade.wav" );
	//�T�E���hID
	m_sound_id = 0;

	//�J����
	m_camera = CFPCamera::Create();

	//�L���[�u�}�l�[�W��������
	m_cube_manager = CCubeManager::Create( m_csv->GetToken< std::string >( 0 , 0 ) );

	//NON�L���[�u�̓����蔻���S�ăI�t�ɂ���
	for( int x = 0 ; x < m_cube_manager->m_cubes.size() ; ++x )
	{
		for( int y = 0 ; y < m_cube_manager->m_cubes[ x ].size() ; ++y )
		{
			for( int z = 0 ; z < m_cube_manager->m_cubes[ x ][ y ].size() ; ++z )
			{
				if ( m_cube_manager->NON == m_cube_manager->m_cubes[ x ][ y ][ z ]->GetCubeState() ) m_cube_manager->m_cubes[ x ][ y ][ z ]->SetIsCollision( false );
			}
		}
	}

	//�J�������X�^�[�g�L���[�u��
	CameraToStartCube();

	//UI������
	m_ui = CUserInterface::Create();

	//�}�E�X�J�[�\����\��
	MOUSE->DisplayCursor( false );
}

//�������
void CScenePlay::SceneDelete()
{
	//�X�e�[�W�f�[�^�̏����o��
	//m_cube_manager->m_csv_platinum->ExportFile();

	//���
	m_cube_manager.reset();
	m_ui.reset();
}

//���W�b�N����
void CScenePlay::SceneFrameMove( const float elapsed_time )
{	
	//UI�X�V
	m_ui->Update( m_cube_manager->GetGoalPos() - m_camera->GetPosition() , m_camera->GetViewDir() , elapsed_time );

	//����
	PlayInput();
	
	//�J�����X�V
	m_camera->UpdateCamera( elapsed_time );

	//���S
	if( m_camera->m_eye.y <= -100 )
	{
		//�J�������X�^�[�g�L���[�u��
		CameraToStartCube();
	}
	
	//�����蔻���S�t�B�[���h�u���b�N�Ǝ��
	for( int x = 0 ; x < m_cube_manager->m_cubes.size() ; ++x )
	{
		for( int y = 0 ; y < m_cube_manager->m_cubes[ x ].size() ; ++y )
		{
			for( int z = 0 ; z < m_cube_manager->m_cubes[ x ][ y ].size() ; ++z )
			{
				//�����蔻��t���O���I�t�Ȃ�ȉ��̏������s��Ȃ�
				if( m_cube_manager->m_cubes[ x ][ y ][ z ]->GetIsCollision() == false ) continue;

				CCollisionManager::HIT_DIRECTION dir = CCollisionManager::HIT_DIRECTION::NONE;
				dir = CCollisionManager::CollisionAABBAABB( m_cube_manager->m_cubes[ x ][ y ][ z ]->m_pos , m_cube_manager->m_cubes[ x ][ y ][ z ]->m_prev_pos , m_cube_manager->m_cubes[ x ][ y ][ z ]->GetSize() , m_camera->m_eye , m_camera->m_prev_eye , m_camera->GetSize() , m_camera->m_eye );
				
				//�N���A
				if( dir != CCollisionManager::NONE && m_cube_manager->m_cubes[ x ][ y ][ z ]->GetCubeState() == m_cube_manager->GOAL/*CCube::GOAL*/ )
				{
					CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );

					//�X�e�[�W01
					if( m_cube_manager->GetStageName() == "Content/csv/CubeManager01.csv" )
					{
						//�L���[�u�}�l�[�W��������
						m_cube_manager = std::make_shared< CCubeManager >( m_csv->GetToken< std::string >( 1 , 0 ) );

						//�J�������X�^�[�g�L���[�u��
						CameraToStartCube();

						return;
					}
					else
					{
						CSceneManager::GetInstance()->SetNextScene( &CSceneTitle::CreateScene );

						return;
					}
				}

				if( dir == CCollisionManager::HIT_DIRECTION::TOP )
				{
					//�u���b�N�̏�ɏՓ˂����ꍇ�A�ڒn�t���O���I����
					m_camera->SetIsOnGround( true );

					//Y���ړ��ʂ̐���
					//if( m_camera->GetJumpMovement() < -1.0f ) m_camera->SetJumpMovement( -1.0f );
				}
				if( dir == CCollisionManager::HIT_DIRECTION::BOTTOM )
				{
					//�u���b�N�̉��ɏՓ˂����ꍇ�Ay���̈ړ��ʂ�0��
					m_camera->SetJumpMovement( 0.0f );
				}
			}
		}
	}
}

//�`�揈��
void CScenePlay::SceneFrameRender( const float elapsed_time )
{
	HRESULT hr;

	// ��ʂ̃N���A																	  //��ʂ̐F
	V( CGraphicsManager::m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255 , 0 , 0 , 0/*255, 0, 0, 255*/ ), 1.0f, 0 ) );

	//�`��
	CGraphicsManager::SysRender( m_camera );
}

//�T�E���h�Đ�
void CScenePlay::ScenePlaySound()
{
	if( m_sound_id == 0 )
	{
		m_sound_id = CSoundManager::Play( "Content/sound/play.wav" , CSoundManager::MODE_LOOP );
	}
	else
	{
		CSoundManager::Play( m_sound_id );
	}
}

//�T�E���h��~
void CScenePlay::SceneStopSound()
{
	CSoundManager::Stop( m_sound_id );
}

//����
void CScenePlay::PlayInput()
{
	//�^�C�g����
	if( K_B->KeyPressed( K_B->KEY_ENTER ) )
	{
		CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
		CSceneManager::GetInstance()->SetNextScene( &CSceneTitle::CreateScene );
	}

	//�X�^�[�g�ʒu��
	if( K_B->KeyPressed( K_B->KEY_P ) )
	{
		CameraToStartCube();
	}

	//�S�[���ʒu��
	if( K_B->KeyPressed( K_B->KEY_O ) )
	{
		m_camera->m_eye = m_cube_manager->GetGoalPos();
	}
}

//�J�������X�^�[�g�L���[�u��
void CScenePlay::CameraToStartCube()
{
	//�J����������
	m_camera->Init();
	//���W
	m_camera->m_eye = m_cube_manager->GetStartPos();
}
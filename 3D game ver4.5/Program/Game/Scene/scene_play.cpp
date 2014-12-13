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
	m_Csv = CCsv::Create( "Content/csv/StageData.csv" );

	//BGM���[�h
	CSoundManager::Load( "Content/sound/play.wav" );
	CSoundManager::Load( "Content/sound/fade.wav" );
	//�T�E���hID
	m_SoundId = 0;

	//�J����
	m_Camera = CFPCamera::Create();
	//�O���t�B�b�N�}�l�[�W���̃J�����ɐݒ�
	//CGraphicsManager::m_2dCamera = m_ViewCamera;
	CGraphicsManager::m_3dCamera = m_Camera;

	//�L���[�u�}�l�[�W��������
	m_CubeManager = CCubeManager::Create( m_Csv->GetToken< std::string >( 0 , 0 ) );

	//NON�L���[�u�̓����蔻���S�ăI�t�ɂ���
	for( int x = 0 ; x < m_CubeManager->m_Cubes.size() ; ++x )
	{
		for( int y = 0 ; y < m_CubeManager->m_Cubes[ x ].size() ; ++y )
		{
			for( int z = 0 ; z < m_CubeManager->m_Cubes[ x ][ y ].size() ; ++z )
			{
				if ( m_CubeManager->NON == m_CubeManager->m_Cubes[ x ][ y ][ z ]->GetCubeState() ) m_CubeManager->m_Cubes[ x ][ y ][ z ]->SetIsCollision( false );
			}
		}
	}

	//�J�������X�^�[�g�L���[�u��
	CameraToStartCube();

	//UI������
	m_Ui = CUserInterface::Create();

	//�}�E�X�J�[�\����\��
	MOUSE->DisplayCursor( false );
}

//�������
void CScenePlay::SceneDelete()
{
	//�X�e�[�W�f�[�^�̏����o��
	//m_CubeManager->m_CsvPlatinum->ExportFile();

	//���
	m_CubeManager.reset();
	m_Ui.reset();
}

//���W�b�N����
void CScenePlay::SceneFrameMove( const float elapsed_time )
{	
	//UI�X�V
	m_Ui->Update( m_CubeManager->GetGoalPos() - m_Camera->GetPosition() , m_Camera->GetViewDir() , elapsed_time );

	//����
	PlayInput();
	
	//�J�����X�V
	m_Camera->UpdateCamera( elapsed_time );

	//���S
	if( m_Camera->m_Eye.y <= -100 )
	{
		//�J�������X�^�[�g�L���[�u��
		CameraToStartCube();
	}
	
	//�����蔻���S�t�B�[���h�u���b�N�Ǝ��
	for( int x = 0 ; x < m_CubeManager->m_Cubes.size() ; ++x )
	{
		for( int y = 0 ; y < m_CubeManager->m_Cubes[ x ].size() ; ++y )
		{
			for( int z = 0 ; z < m_CubeManager->m_Cubes[ x ][ y ].size() ; ++z )
			{
				//�����蔻��t���O���I�t�Ȃ�ȉ��̏������s��Ȃ�
				if( m_CubeManager->m_Cubes[ x ][ y ][ z ]->GetIsCollision() == false ) continue;

				CCollisionManager::HIT_DIRECTION dir = CCollisionManager::HIT_DIRECTION::NONE;
				dir = CCollisionManager::CollisionAABBAABB( m_CubeManager->m_Cubes[ x ][ y ][ z ]->m_Pos , m_CubeManager->m_Cubes[ x ][ y ][ z ]->m_PrevPos , m_CubeManager->m_Cubes[ x ][ y ][ z ]->GetSize() , m_Camera->m_Eye , m_Camera->m_PrevEye , m_Camera->GetSize() , m_Camera->m_Eye );
				
				//�N���A
				if( dir != CCollisionManager::NONE && m_CubeManager->m_Cubes[ x ][ y ][ z ]->GetCubeState() == m_CubeManager->GOAL/*CCube::GOAL*/ )
				{
					CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );

					//�X�e�[�W01
					if( m_CubeManager->GetStageName() == "Content/csv/CubeManager01.csv" )
					{
						//�L���[�u�}�l�[�W��������
						m_CubeManager = std::make_shared< CCubeManager >( m_Csv->GetToken< std::string >( 1 , 0 ) );

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
					m_Camera->SetIsOnGround( true );

					//Y���ړ��ʂ̐���
					//if( m_Camera->GetJumpMovement() < -1.0f ) m_Camera->SetJumpMovement( -1.0f );
				}
				if( dir == CCollisionManager::HIT_DIRECTION::BOTTOM )
				{
					//�u���b�N�̉��ɏՓ˂����ꍇ�Ay���̈ړ��ʂ�0��
					m_Camera->SetJumpMovement( 0.0f );
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
	V( CGraphicsManager::m_pD3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255 , 0 , 0 , 0/*255, 0, 0, 255*/ ), 1.0f, 0 ) );

	//�`��
	CGraphicsManager::SysRender( m_Camera );
}

//�T�E���h�Đ�
void CScenePlay::ScenePlaySound()
{
	if( m_SoundId == 0 )
	{
		m_SoundId = CSoundManager::Play( "Content/sound/play.wav" , CSoundManager::MODE_LOOP );
	}
	else
	{
		CSoundManager::Play( m_SoundId );
	}
}

//�T�E���h��~
void CScenePlay::SceneStopSound()
{
	CSoundManager::Stop( m_SoundId );
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
		m_Camera->m_Eye = m_CubeManager->GetGoalPos();
	}
}

//�J�������X�^�[�g�L���[�u��
void CScenePlay::CameraToStartCube()
{
	//�J����������
	m_Camera->Init();
	//���W
	m_Camera->m_Eye = m_CubeManager->GetStartPos();
}
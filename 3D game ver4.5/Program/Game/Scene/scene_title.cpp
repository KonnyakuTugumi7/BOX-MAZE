#include "DXUT.h"
#include "scene_title.h"
#include "../../Library/Support/common.h"
#include "../../Library/Input/input.h"
#include "../../Library/Sound/sound.h"
#include "../../Library/Sound/sound_manager.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../GameEngine/Scene/scene_manager.h"
#include "../../GameEngine/Camera/camera_base.h"
#include "../../Game/Scene/scene_play.h"
#include "../../Game/Scene/scene_edit.h"
#include "../../Game/Camera/tp_camera.h"

//������
void CSceneTitle::SceneInit()
{
	//BGM���[�h
	CSoundManager::Load( "Content/sound/title.wav" );
	CSoundManager::Load( "Content/sound/cursor.wav" );
	CSoundManager::Load( "Content/sound/fade.wav" );
	//�T�E���hID
	m_sound_id = 0;

	//CSV
	m_csv = CCsv::Create( "Content/csv/TitleData.csv" );

	//�J����
	m_camera = CTPCamera::Create();

	//------------------------------�摜------------------------------

	//�w�i
	m_back = CSprite::Create( "Content/graphics/title.jpg" , CGraphicsManager::BACK_2D );
	m_back->m_pos = D3DXVECTOR3( m_csv->GetToken< int >( 0 , 0 ) , m_csv->GetToken< int >( 0 , 1 ) , 0 );
	//m_board = CBoard::Create( /*50*/80 , /*50*/60 , "Content/graphics/title.jpg" , CGraphicsManager::BACK_2D );
	//m_board->m_pos = D3DXVECTOR3( /*m_csv->GetToken< int >( 0 , 0 )*/0 , /*m_csv->GetToken< int >( 0 , 1 )10*/10 , 25 );

	//���S(�^�C�g��)
	//m_logo[ 0 ] = CSprite::Create( "Content/graphics/titlelogo.png" , CGraphicsManager::FRONT_2D );
	//m_logo[ 0 ]->m_pos = D3DXVECTOR3( m_csv->GetToken< int >( 1 , 0 ) , m_csv->GetToken< int >( 1 , 1 ) , 0 );
	m_logo[ 0 ] = CFont::Create( 65 , 50 , 0 );
	m_logo[ 2 ] = CFont::Create( 65 , 50 , 0 );

	//���S1(START)
	//m_logo[ 1 ] = CSprite::Create( "Content/graphics/titlelogo1.png" , CGraphicsManager::FRONT_2D );
	//m_logo[ 1 ]->m_pos = D3DXVECTOR3( m_csv->GetToken< int >( 2 , 0 ) , m_csv->GetToken< int >( 2 , 1 ) , 0 );
	m_logo[ 1 ] = CFont::Create( 50 , 70 , 1 );

	//���S2(EDIT)
	//m_logo[ 2 ] = CSprite::Create( "Content/graphics/titlelogo2.png" , CGraphicsManager::FRONT_2D );
	//m_logo[ 2 ]->m_pos = D3DXVECTOR3( m_csv->GetToken< int >( 3 , 0 ) , m_csv->GetToken< int >( 3 , 1 ) , 0 );
	//m_logo[ 2 ] = CFont::Create( 70 , 70 , 1 );

	//���S3(EXIT)
	//m_logo[ 3 ] = CSprite::Create( "Content/graphics/titlelogo3.png" , CGraphicsManager::FRONT_2D );
	//m_logo[ 3 ]->m_pos = D3DXVECTOR3( m_csv->GetToken< int >( 4 , 0 ) , m_csv->GetToken< int >( 4 , 1 ) , 0 );
	//m_logo[ 3 ] = CFont::Create( 70 , 70 , 1 );

	//�V�[���i���o�[
	m_scene_num = SCENE_NUM::PLAY;

	//�}�E�X�J�[�\����\��
	//MOUSE->DisplayCursor( false ); //�R�����g�A�E�g���Ȃ��ƃJ�����̓��������������Ȃ�;;

	//�X�e�[�W�I����ʂ��ǂ���
	m_is_stage_select = false;
}

//�������
void CSceneTitle::SceneDelete()
{
	//���
	m_back.reset();
	m_logo[ 0 ].reset();
	m_logo[ 1 ].reset();
	//m_logo[ 2 ].reset();
	//m_logo[ 3 ].reset();
}

//���W�b�N����
void CSceneTitle::SceneFrameMove( const float elapsed_time )
{
	//--------------------------------------�I���ړ�--------------------------------------
	if( K_B->KeyPressed( K_B->KEY_UP ) )
	{
		CSoundManager::Play( "Content/sound/cursor.wav" , CSoundManager::MODE_ONCE );
		m_scene_num -= 1;
	}
		
	if( K_B->KeyPressed( K_B->KEY_DOWN ) )
	{
		CSoundManager::Play( "Content/sound/cursor.wav" , CSoundManager::MODE_ONCE );
		m_scene_num += 1;
	}

	//���j���[��ʂ��Ƃ̓��͏���
	if( m_is_stage_select == false )
	{
		TitleInput();
	}
	else
	{
		StageSelectInput();
	}

	//for( int i = 0 ; i < SCENE_NUM::SCENE_MAX ; ++i )
	//{
	//	//�I���������x�ύX
	//	//�I��
	//	if( i == m_scene_num )
	//	{
	//		m_logo[ i + 1 ]->m_col = D3DXVECTOR4( 1 , 1 , 0 , 1 );
	//	}
	//	else
	//	{
	//		m_logo[ i + 1 ]->m_col = D3DXVECTOR4( m_csv->GetToken< float >( 5 , 0 ) , m_csv->GetToken< float >( 5 , 0 ) , m_csv->GetToken< float >( 5 , 0 ) , m_csv->GetToken< float >( 5 , 0 ) );
	//	}
	//}

	////--------------------------------------�I���ړ�--------------------------------------
	//if( K_B->KeyPressed( K_B->KEY_UP ) )
	//{
	//	CSoundManager::Play( "Content/sound/cursor.wav" , CSoundManager::MODE_ONCE );
	//	m_scene_num -= 1;
	//}
	//	
	//if( K_B->KeyPressed( K_B->KEY_DOWN ) )
	//{
	//	CSoundManager::Play( "Content/sound/cursor.wav" , CSoundManager::MODE_ONCE );
	//	m_scene_num += 1;
	//}
	////�I���ʒu�����E�܂ŒB������߂�
	//if( m_scene_num <= SCENE_NUM::SCENE_MIN ) m_scene_num = SCENE_NUM::SCENE_MAX + SCENE_NUM::SCENE_MIN;	
	//if( m_scene_num >= SCENE_NUM::SCENE_MAX ) m_scene_num = SCENE_NUM::PLAY;
	//
	////-------------------------------------------------���s(�V�[���ړ�)-------------------------------------------------
	////�C���X�^���X�쐬
	////�v���C��
	//if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_scene_num == SCENE_NUM::PLAY )
	//{
	//	CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
	//	CSceneManager::GetInstance()->SetNextScene( &CScenePlay::CreateScene );
	//}
	////�G�f�B�b�g��
	//if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_scene_num == SCENE_NUM::EDIT )
	//{
	//	CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
	//	CSceneManager::GetInstance()->SetNextScene( &CSceneEdit::CreateScene );
	//}
	////�I��
	//if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_scene_num == SCENE_NUM::EXIT )
	//{
	//	PostQuitMessage( WM_QUIT );
	//}
}

//�`�揈��
void CSceneTitle::SceneFrameRender( const float elapsed_time )
{
	HRESULT hr;

	// ��ʂ̃N���A																	  //��ʂ̐F
	V( CGraphicsManager::m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 ) );

	//�`��
	CGraphicsManager::SysRender( m_camera );

	if( m_is_stage_select == false )
	{
		//�^�C�g�����j���[�`��
		TitleRender();
	}
	else
	{
		//�X�e�[�W�I�����j���[�`��
		StageSelectRender();
	}
}

//�T�E���h�Đ�
void CSceneTitle::ScenePlaySound()
{
	if( m_sound_id == 0 )
	{
		m_sound_id = CSoundManager::Play( "Content/sound/title.wav" , CSoundManager::MODE_LOOP );
	}
	else
	{
		CSoundManager::Play( m_sound_id );
	}
}

//�T�E���h��~
void CSceneTitle::SceneStopSound()
{
	CSoundManager::Stop( m_sound_id );
}

//�v���C���[�̓���
void CSceneTitle::TitleInput()
{
	////--------------------------------------�I���ړ�--------------------------------------
	//if( K_B->KeyPressed( K_B->KEY_UP ) )
	//{
	//	CSoundManager::Play( "Content/sound/cursor.wav" , CSoundManager::MODE_ONCE );
	//	m_scene_num -= 1;
	//}
	//	
	//if( K_B->KeyPressed( K_B->KEY_DOWN ) )
	//{
	//	CSoundManager::Play( "Content/sound/cursor.wav" , CSoundManager::MODE_ONCE );
	//	m_scene_num += 1;
	//}
	//�I���ʒu�����E�܂ŒB������߂�
	if( m_scene_num <= SCENE_NUM::SCENE_MIN ) m_scene_num = SCENE_NUM::SCENE_MAX + SCENE_NUM::SCENE_MIN;	
	if( m_scene_num >= SCENE_NUM::SCENE_MAX ) m_scene_num = SCENE_NUM::PLAY;
	
	//-------------------------------------------------���s(�V�[���ړ�)-------------------------------------------------
	//�C���X�^���X�쐬
	//�v���C��
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_scene_num == SCENE_NUM::PLAY )
	{
		CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
		CSceneManager::GetInstance()->SetNextScene( &CScenePlay::CreateScene );
	}
	//�X�e�[�W�I����
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_scene_num == SCENE_NUM::EDIT )
	{
		CSoundManager::Play( "Content/sound/cursor.wav" , CSoundManager::MODE_ONCE );
		m_is_stage_select = true;
		m_scene_num = STAGE_SELECT_NUM::STAGE01;
		//CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
		//CSceneManager::GetInstance()->SetNextScene( &CSceneEdit::CreateScene );
	}
	//�I��
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_scene_num == SCENE_NUM::EXIT )
	{
		PostQuitMessage( WM_QUIT );
	}
}

//�v���C���[�̓���
void CSceneTitle::StageSelectInput()
{
	//�I���ʒu�����E�܂ŒB������߂�
	if( m_scene_num <= STAGE_SELECT_NUM::STAGE_MIN ) m_scene_num = STAGE_SELECT_NUM::STAGE_MAX + STAGE_SELECT_NUM::STAGE_MIN;
	if( m_scene_num >= STAGE_SELECT_NUM::STAGE_MAX ) m_scene_num = STAGE_SELECT_NUM::STAGE01;
	
	//-------------------------------------------------���s(�V�[���ړ�)-------------------------------------------------
	//�C���X�^���X�쐬
	//Stage01�̃G�f�B�b�g��
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_scene_num == STAGE_SELECT_NUM::STAGE01 )
	{
		m_csv = CCsv::Create( "Content/csv/EditData.csv" );
		m_csv->EraseToken( 2 , 0 );
		m_csv->SetToken( 2 , 0 , "Content/csv/CubeManager01.csv" );
		m_csv->ExportFile();
		CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
		CSceneManager::GetInstance()->SetNextScene( &CSceneEdit::CreateScene );
	}
	//Stage01�̃G�f�B�b�g��
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_scene_num == STAGE_SELECT_NUM::STAGE02 )
	{
		m_csv = CCsv::Create( "Content/csv/EditData.csv" );
		m_csv->EraseToken( 2 , 0 );
		m_csv->SetToken( 2 , 0 , "Content/csv/CubeManager02.csv" );
		m_csv->ExportFile();
		CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
		CSceneManager::GetInstance()->SetNextScene( &CSceneEdit::CreateScene );
	}
	//�^�C�g����ʂ�
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_scene_num == STAGE_SELECT_NUM::TITLE )
	{
		m_scene_num = SCENE_NUM::PLAY;
		m_is_stage_select = false;
	}
}

//�^�C�g����ʕ`��
void CSceneTitle::TitleRender()
{
	m_logo[ 0 ]->DrawFont( "BOX MAZE" , 150 , 100 , m_logo[ 0 ]->RED );
	m_logo[ 1 ]->DrawFont( "START" , 250 , 200 , ( m_scene_num == SCENE_NUM::PLAY ? m_logo[ 1 ]->YELLOW : m_logo[ 1 ]->RED ) );
	m_logo[ 1 ]->DrawFont( "EDIT" , 250 , 300 , ( m_scene_num == SCENE_NUM::EDIT ? m_logo[ 1 ]->YELLOW : m_logo[ 1 ]->RED ) );
	m_logo[ 1 ]->DrawFont( "EXIT" , 250 , 400 , ( m_scene_num == SCENE_NUM::EXIT ? m_logo[ 1 ]->YELLOW : m_logo[ 1 ]->RED ) );
}

//�X�e�[�W�Z���N�g��ʕ`��
void CSceneTitle::StageSelectRender()
{
	m_logo[ 2 ]->DrawFont( "STAGE SELECT" , 0 , 50 , m_logo[ 0 ]->RED );
	m_logo[ 1 ]->DrawFont( "STAGE01" , 200 , 200 , ( m_scene_num == SCENE_NUM::PLAY ? m_logo[ 1 ]->YELLOW : m_logo[ 1 ]->RED ) );
	m_logo[ 1 ]->DrawFont( "STAGE02" , 200 , 300 , ( m_scene_num == SCENE_NUM::EDIT ? m_logo[ 1 ]->YELLOW : m_logo[ 1 ]->RED ) );
	m_logo[ 1 ]->DrawFont( "TITLE" , 200 , 400 , ( m_scene_num == SCENE_NUM::EXIT ? m_logo[ 1 ]->YELLOW : m_logo[ 1 ]->RED ) );
}
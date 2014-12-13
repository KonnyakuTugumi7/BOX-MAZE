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
#include "../../Game/Camera/view_camera.h"

//������
void CSceneTitle::SceneInit()
{
	//BGM���[�h
	CSoundManager::Load( "Content/sound/title.wav" );
	CSoundManager::Load( "Content/sound/cursor.wav" );
	CSoundManager::Load( "Content/sound/fade.wav" );
	//�T�E���hID
	m_SoundId = 0;

	//CSV
	m_Csv = CCsv::Create( "Content/csv/TitleData.csv" );

	//�J����
	m_Camera = CViewCamera::Create();

	//------------------------------�摜------------------------------

	//�w�i
	m_Back = CBoard::Create( CGraphicsManager::m_WindowWidth , CGraphicsManager::m_WindowHeight , "Content/graphics/title.jpg" , CGraphicsManager::BACK_2D );

	//�V�[���i���o�[
	m_SceneNum = SCENE_NUM::PLAY;

	//�}�E�X�J�[�\����\��
	//MOUSE->DisplayCursor( false ); //�R�����g�A�E�g���Ȃ��ƃJ�����̓��������������Ȃ�;;

	//�X�e�[�W�I����ʂ��ǂ���
	m_IsStageSelect = false;

	for( int i = 0 ; i < m_Csv->GetNumLine() ; ++i )
	{
		WCHAR wc_buff[ 255 ] = { 0 };
		CCommon::DXconvAnsiToWide( wc_buff , m_Csv->GetToken< std::string >( i , 0 ).c_str() , 255 );
		m_Logo.push_back( wc_buff );
	}
	m_Csv = CCsv::Create( "Content/csv/TitleSelectData.csv" );
	for( int i = 0 ; i < m_Csv->GetNumLine() ; ++i )
	{
		WCHAR wc_buff[ 255 ] = { 0 };
		CCommon::DXconvAnsiToWide( wc_buff , m_Csv->GetToken< std::string >( i , 0 ).c_str() , 255 );
		m_SelectLogo.push_back( wc_buff );
	}

	//�t�H���g�𐶐�
	m_Font = CFont::Create();
	if (m_Font)
	{
		m_Font->SetSize( 80.0f , 50.0f );
		m_Font->SetAlign( CFont::CENTER );
		for( int i = 0 ; i < m_Logo.size() ; ++i ) m_Font->CreateTexture( 128 , 512 , ( i == 0 ) ? FontType::MS_GOTHIC : FontType::MS_MINCHOU , m_Logo[ i ] );
		for( int i = 0 ; i < m_SelectLogo.size() ; ++i ) m_Font->CreateTexture( 128 , 512 , ( i == 0 ) ? FontType::MS_GOTHIC : FontType::MS_MINCHOU , m_SelectLogo[ i ] );
	}
}

//�������
void CSceneTitle::SceneDelete()
{
	//���
	m_Back.reset();
	m_Font.reset();
}

//���W�b�N����
void CSceneTitle::SceneFrameMove( const float elapsed_time )
{
	//--------------------------------------�I���ړ�--------------------------------------
	if( K_B->KeyPressed( K_B->KEY_UP ) )
	{
		CSoundManager::Play( "Content/sound/cursor.wav" , CSoundManager::MODE_ONCE );
		m_SceneNum -= 1;
	}
		
	if( K_B->KeyPressed( K_B->KEY_DOWN ) )
	{
		CSoundManager::Play( "Content/sound/cursor.wav" , CSoundManager::MODE_ONCE );
		m_SceneNum += 1;
	}

	//���j���[��ʂ��Ƃ̏���
	if( m_IsStageSelect == false )
	{
		TitleInput();
	}
	else
	{
		StageSelectInput();
	}
}

//�`�揈��
void CSceneTitle::SceneFrameRender( const float elapsed_time )
{
	HRESULT hr;

	//��ʂ̃N���A																	  //��ʂ̐F
	V( CGraphicsManager::m_pD3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 ) );

	//�`��
	CGraphicsManager::SysRender( m_Camera );

	D3DXVECTOR4 color(Color::WHITE, 1.0f);

	//���j���[��ʂ��Ƃ̏���
	if( m_IsStageSelect == false )
	{
		for( int i = 0 ; i < m_Logo.size() ; ++i )
		{
			( m_SceneNum == i - 1 ) ? color = D3DXVECTOR4( Color::YELLOW , 1.0f ) : color = D3DXVECTOR4( Color::RED , 1.0f );
			( i == 0 ) ? m_Font->SetSize( 100.0f , 70.0f ) : m_Font->SetSize( 80.0f , 50.0f );
			m_Font->Draw( m_Camera , m_Logo[ i ] , D3DXVECTOR2( 400 , ( i * 120 ) + 100 ) , color );
		}
	}
	else
	{
		for( int i = 0 ; i < m_SelectLogo.size() ; ++i )
		{
			( m_SceneNum == i - 1 ) ? color = D3DXVECTOR4( Color::YELLOW , 1.0f ) : color = D3DXVECTOR4( Color::RED , 1.0f );
			( i == 0 ) ? m_Font->SetSize( 100.0f , 70.0f ) : m_Font->SetSize( 80.0f , 50.0f );
			m_Font->Draw( m_Camera , m_SelectLogo[ i ] , D3DXVECTOR2( 400 , ( i * 120 ) + 100 ) , color );
		}
	}
}

//�T�E���h�Đ�
void CSceneTitle::ScenePlaySound()
{
	if( m_SoundId == 0 )
	{
		m_SoundId = CSoundManager::Play( "Content/sound/title.wav" , CSoundManager::MODE_LOOP );
	}
	else
	{
		CSoundManager::Play( m_SoundId );
	}
}

//�T�E���h��~
void CSceneTitle::SceneStopSound()
{
	CSoundManager::Stop( m_SoundId );
}

//�v���C���[�̓���
void CSceneTitle::TitleInput()
{
	//�I���ʒu�����E�܂ŒB������߂�
	if( m_SceneNum <= SCENE_NUM::SCENE_MIN ) m_SceneNum = SCENE_NUM::SCENE_MAX + SCENE_NUM::SCENE_MIN;	
	if( m_SceneNum >= SCENE_NUM::SCENE_MAX ) m_SceneNum = SCENE_NUM::PLAY;
	
	//-------------------------------------------------���s(�V�[���ړ�)-------------------------------------------------
	//�C���X�^���X�쐬
	//�v���C��
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_SceneNum == SCENE_NUM::PLAY )
	{
		CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
		CSceneManager::GetInstance()->SetNextScene( &CScenePlay::CreateScene );
	}
	//�X�e�[�W�I����
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_SceneNum == SCENE_NUM::EDIT )
	{
		CSoundManager::Play( "Content/sound/cursor.wav" , CSoundManager::MODE_ONCE );
		m_IsStageSelect = true;
		m_SceneNum = STAGE_SELECT_NUM::STAGE01;
		//CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
		//CSceneManager::GetInstance()->SetNextScene( &CSceneEdit::CreateScene );
	}
	//�I��
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_SceneNum == SCENE_NUM::EXIT )
	{
		PostQuitMessage( WM_QUIT );
	}
}

//�v���C���[�̓���
void CSceneTitle::StageSelectInput()
{
	//�I���ʒu�����E�܂ŒB������߂�
	if( m_SceneNum <= STAGE_SELECT_NUM::STAGE_MIN ) m_SceneNum = STAGE_SELECT_NUM::STAGE_MAX + STAGE_SELECT_NUM::STAGE_MIN;
	if( m_SceneNum >= STAGE_SELECT_NUM::STAGE_MAX ) m_SceneNum = STAGE_SELECT_NUM::STAGE01;
	
	//-------------------------------------------------���s(�V�[���ړ�)-------------------------------------------------
	//�C���X�^���X�쐬
	if( K_B->KeyPressed( K_B->KEY_ENTER ) )
	{
		if( m_SceneNum == STAGE_SELECT_NUM::TITLE )
		{
			m_SceneNum = SCENE_NUM::PLAY;
			m_IsStageSelect = false;
		}
		else
		{
			m_Csv = CCsv::Create( "Content/csv/EditData.csv" );
			m_Csv->EraseToken( 2 , 0 );
			std::stringstream ss;
			ss << "Content/csv/CubeManager0" << m_SceneNum + 1 << ".csv";
			m_Csv->SetToken( 2 , 0 , ss.str() );
			m_Csv->ExportFile();
			CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
			CSceneManager::GetInstance()->SetNextScene( &CSceneEdit::CreateScene );
		}
	}
}
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
	m_sound_id = 0;

	//CSV
	m_csv = CCsv::Create( "Content/csv/TitleData.csv" );

	//�J����
	m_camera = CViewCamera::Create();

	//------------------------------�摜------------------------------

	//�w�i
	//m_back = CSprite::Create( "Content/graphics/title.jpg" , CGraphicsManager::BACK_2D );
	//m_back->m_pos = D3DXVECTOR3( m_csv->GetToken< int >( 0 , 0 ) , m_csv->GetToken< int >( 0 , 1 ) , 0 );
	m_board = CBoard::Create( CGraphicsManager::m_window_width , CGraphicsManager::m_window_height , "Content/graphics/title.jpg" , CGraphicsManager::BACK_2D );

	//�V�[���i���o�[
	m_scene_num = SCENE_NUM::PLAY;

	//�}�E�X�J�[�\����\��
	//MOUSE->DisplayCursor( false ); //�R�����g�A�E�g���Ȃ��ƃJ�����̓��������������Ȃ�;;

	//�X�e�[�W�I����ʂ��ǂ���
	m_is_stage_select = false;

	for( int i = 0 ; i < m_csv->GetNumLine() ; ++i )
	{
		WCHAR wc_buff[ 255 ] = { 0 };
		CCommon::DXconvAnsiToWide( wc_buff , m_csv->GetToken< std::string >( i , 0 ).c_str() , 255 );
		m_logo.push_back( wc_buff );
	}
	m_csv = CCsv::Create( "Content/csv/TitleSelectData.csv" );
	for( int i = 0 ; i < m_csv->GetNumLine() ; ++i )
	{
		WCHAR wc_buff[ 255 ] = { 0 };
		CCommon::DXconvAnsiToWide( wc_buff , m_csv->GetToken< std::string >( i , 0 ).c_str() , 255 );
		m_select_logo.push_back( wc_buff );
	}

	//�t�H���g�𐶐�
	m_font = CFont::Create();
	if (m_font)
	{
		m_font->SetSize( 80.0f , 50.0f );
		m_font->SetAlign( CFont::CENTER );
		for( int i = 0 ; i < m_logo.size() ; ++i ) m_font->CreateTexture( 128 , 512 , ( i == 0 ) ? FontType::MS_GOTHIC : FontType::MS_MINCHOU , m_logo[ i ] );
		for( int i = 0 ; i < m_select_logo.size() ; ++i ) m_font->CreateTexture( 128 , 512 , ( i == 0 ) ? FontType::MS_GOTHIC : FontType::MS_MINCHOU , m_select_logo[ i ] );
	}
}

//�������
void CSceneTitle::SceneDelete()
{
	//���
	m_back.reset();
	m_board.reset();
	m_font.reset();
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

	//���j���[��ʂ��Ƃ̏���
	if( m_is_stage_select == false )
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
	V( CGraphicsManager::m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 0, 0, 0 ), 1.0f, 0 ) );

	//�`��
	CGraphicsManager::SysRender( m_camera );

	D3DXVECTOR4 color(Color::WHITE, 1.0f);

	//���j���[��ʂ��Ƃ̏���
	if( m_is_stage_select == false )
	{
		for( int i = 0 ; i < m_logo.size() ; ++i )
		{
			( m_scene_num == i - 1 ) ? color = D3DXVECTOR4( Color::YELLOW , 1.0f ) : color = D3DXVECTOR4( Color::RED , 1.0f );
			( i == 0 ) ? m_font->SetSize( 100.0f , 70.0f ) : m_font->SetSize( 80.0f , 50.0f );
			m_font->Draw( m_camera , m_logo[ i ] , D3DXVECTOR2( 400 , ( i * 120 ) + 100 ) , color );
		}
	}
	else
	{
		for( int i = 0 ; i < m_select_logo.size() ; ++i )
		{
			( m_scene_num == i - 1 ) ? color = D3DXVECTOR4( Color::YELLOW , 1.0f ) : color = D3DXVECTOR4( Color::RED , 1.0f );
			( i == 0 ) ? m_font->SetSize( 100.0f , 70.0f ) : m_font->SetSize( 80.0f , 50.0f );
			m_font->Draw( m_camera , m_select_logo[ i ] , D3DXVECTOR2( 400 , ( i * 120 ) + 100 ) , color );
		}
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
	if( K_B->KeyPressed( K_B->KEY_ENTER ) )
	{
		if( m_scene_num == STAGE_SELECT_NUM::TITLE )
		{
			m_scene_num = SCENE_NUM::PLAY;
			m_is_stage_select = false;
		}
		else
		{
			m_csv = CCsv::Create( "Content/csv/EditData.csv" );
			m_csv->EraseToken( 2 , 0 );
			std::stringstream ss;
			ss << "Content/csv/CubeManager0" << m_scene_num + 1 << ".csv";
			m_csv->SetToken( 2 , 0 , ss.str() );
			m_csv->ExportFile();
			CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
			CSceneManager::GetInstance()->SetNextScene( &CSceneEdit::CreateScene );
		}
	}
}
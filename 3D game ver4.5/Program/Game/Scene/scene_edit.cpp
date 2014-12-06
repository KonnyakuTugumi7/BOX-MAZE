#include "DXUT.h"
#include "scene_edit.h"
#include "../../Library/Support/common.h"
#include "../../Library/Input/input.h"
#include "../../Library/Sound/sound.h"
#include "../../Library/Sound/sound_manager.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Graphic/font.h"
#include "../../GameEngine/Collision/collision_manager.h"
#include "../../GameEngine/Scene/scene_manager.h"
#include "../../Game/Camera/fp_camera.h"
#include "../../Game/Camera/tp_camera.h"
#include "../../Game/Scene/scene_title.h"
#include "../../GameEngine/Graphic/cube.h"
#include <sstream>
#include <map>

//������
void CSceneEdit::SceneInit()
{
	//CSV
	m_csv = CCsv::Create( "Content/csv/EditData.csv" );

	//��ʒ����̏\���J�[�\��
	m_center_carsor = CSprite::Create( "Content/graphics/center_carsor.png" , CGraphicsManager::FRONT_2D );
	m_center_carsor->m_pos = D3DXVECTOR3( 400 , 300 , 0 );

	//�J����
	m_camera = CFPCamera::Create();
	m_camera->SetIsGravity( false );
	m_view_camera = CViewCamera::Create();

	//BGM���[�h
	//CSoundManager::Load( "Content/sound/play.wav" );
	CSoundManager::Load( "Content/sound/fade.wav" );
	CSoundManager::Load( "Content/sound/cube_create.wav" );
	CSoundManager::Load( "Content/sound/cube_destroy.wav" );

	//�T�E���hID
	m_sound_id = 0;

	//�L���[�u�}�l�[�W��������
	m_cube_manager = CCubeManager::Create( m_csv->GetToken< std::string >( 2 , 0 ) );

	//�^�[�Q�b�g���W
	m_pos = D3DXVECTOR3( 0 , m_csv->GetToken< float >( 0 , 0 ) , 0 );

	//�^�[�Q�b�g�ړ����x
	m_move_speed = m_csv->GetToken< float >( 1 , 0 );

	//�I���L���[�u
	m_select_cube = CCube::Create( D3DXVECTOR3( 5 , 5 , 5 ) );
	m_select_cube->m_pos = D3DXVECTOR3( 0 , 0 , 0 );
	m_select_cube->m_col = D3DXVECTOR4( 1 , 0 , 0 , 1 );
	m_select_cube->SetIsRender( true );
	m_select_cube->SetCubeState( CCubeManager::NORMAL );

	//�I���L���[�u�Ǘ��i���o�[
	m_select_cube_state = CCubeManager::NORMAL;

	//�}�E�X�J�[�\����\��
	MOUSE->DisplayCursor( false );

	m_csv = CCsv::Create( "Content/csv/EditCubeData.csv" );
	for( int i = 0 ; i < m_csv->GetNumLine() ; ++i )
	{
		WCHAR wc_buff[ 255 ] = { 0 };
		CCommon::DXconvAnsiToWide( wc_buff , m_csv->GetToken< std::string >( i , 0 ).c_str() , 255 );
		m_logo.push_back( wc_buff );
	}
	//�t�H���g�𐶐�
	m_font = CFont::Create();
	if (m_font)
	{
		m_font->SetSize( 30.0f , 20.0f );
		m_font->SetAlign( CFont::LEFT );
		for( int i = 0 ; i < m_logo.size() ; ++i ) m_font->CreateTexture( 128 , 512 , ( i == 0 ) ? FontType::MS_GOTHIC : FontType::MS_MINCHOU , m_logo[ i ] );
	}

	m_csv = CCsv::Create( "Content/csv/EditData.csv" );

	m_debug_font = CDebugFont::Create( 30 , 30 );
}

//�������
void CSceneEdit::SceneDelete()
{
	//�X�e�[�W�f�[�^�����o��
	SceneExport();
	m_cube_manager.reset();
}

//���W�b�N����
void CSceneEdit::SceneFrameMove( const float elapsed_time )
{
	//�J�����X�V
	m_camera->UpdateCamera( elapsed_time );
	m_pos = m_camera->GetViewTarget();

	//�^�C�g����
	//�X�^�[�g�u���b�N�ƃS�[���u���b�N�̐���1�Ȃ�
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_cube_manager->FindSpecifiedCubeNum( CCubeManager::CUBE_STATE::START ) == 1 && m_cube_manager->FindSpecifiedCubeNum( CCubeManager::CUBE_STATE::GOAL ) == 1 )
	{
		CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
		CSceneManager::GetInstance()->SetNextScene( &CSceneTitle::CreateScene );
	}

	if( K_B->KeyPressed( K_B->KEY_Z ) ) --m_select_cube_state;
	if( K_B->KeyPressed( K_B->KEY_X ) ) ++m_select_cube_state;
	if( m_select_cube_state < 0 ) m_select_cube_state = CCubeManager::NORMAL;
	if( m_select_cube_state > CCubeManager::NORMAL ) m_select_cube_state = 0;

	//�����蔻��
	SceneCollision();
}

//�`�揈��
void CSceneEdit::SceneFrameRender( const float elapsed_time )
{
	HRESULT hr;

	//��ʂ̃N���A																	  //��ʂ̐F
	V( CGraphicsManager::m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255 , 0 , 0 , 0/*255, 0, 0, 255*/ ), 1.0f, 0 ) );

	//for( int i = 0 ; i < m_logo.size() ; ++i ) if( i == m_select_cube_state ) m_font->Draw( m_view_camera , m_logo[ i ] , D3DXVECTOR2( 50.0f , 20.0f ) , D3DXVECTOR4( Color::RED , 1.0f ) );

	//�`��
	CGraphicsManager::SysRender( m_camera );

	for( int i = 0 ; i < m_logo.size() ; ++i ) if( i == m_select_cube_state ) m_font->Draw( m_view_camera , m_logo[ i ] , D3DXVECTOR2( 50.0f , 20.0f ) , D3DXVECTOR4( Color::RED , 1.0f ) );

	//std::stringstream ss;
	//
	//if( m_select_cube_state == CCubeManager::NON )
	//{
	//	ss << "NON CUBE";
	//}
	//else if( m_select_cube_state == CCubeManager::START )
	//{
	//	ss << "START CUBE";
	//}
	//else if( m_select_cube_state == CCubeManager::GOAL )
	//{
	//	ss << "GOAL CUBE";
	//}
	//else if( m_select_cube_state == CCubeManager::NORMAL )
	//{
	//	ss << "NORMAL CUBE";
	//}
	//
	//m_debug_font->DrawFont( ss.str() , 20 , 20 , m_debug_font->RED );
}

//�T�E���h�Đ�
void CSceneEdit::ScenePlaySound()
{
	//if( m_sound_id == 0 )
	//{
	//	m_sound_id = CSoundManager::Play( "Content/sound/play.wav" , CSoundManager::MODE_LOOP );
	//}
	//else
	//{
	//	CSoundManager::Play( m_sound_id );
	//}
}

//�T�E���h��~
void CSceneEdit::SceneStopSound()
{
	CSoundManager::Stop( m_sound_id );
}

//�����蔻��
void CSceneEdit::SceneCollision()
{
	//�������Ă���NON�L���[�u�ȊO�̃L���[�u����U�i�[����
	std::map< float , CubeWP > hit_cube_map;
	//NON�L���[�u�ȊO�̃L���[�u�ƃ��C���������Ă�����W����U�i�[����
	std::map< float , D3DXVECTOR3 > hit_cube_pos_map;
	//�������Ă���NON�L���[�u����U�i�[����
	std::map< float , CubeWP > hit_non_cube_map;

	//�����蔻���S�t�B�[���h�u���b�N�Ǝ��
	for( int x = 0 ; x < m_cube_manager->m_cubes.size() ; ++x )
	{
		for( int y = 0 ; y < m_cube_manager->m_cubes[ x ].size() ; ++y )
		{
			for( int z = 0 ; z < m_cube_manager->m_cubes[ x ][ y ].size() ; ++z )
			{
				
				//�}�E�X�|�C���^�̈ʒu���烌�C���΂�
				D3DXVECTOR3 ray_pos , ray_dir;
				CCollisionManager::ScreenToRay( ray_pos , ray_dir , 400/*MOUSE->GetPos().x*/ , 300/*MOUSE->GetPos().y*/ , m_camera->GetViewMatrix() , m_camera->GetProjectionMatrix() );

				float t = 0.0f;
				D3DXVECTOR3 temp( 0 , 0 , 0 );
				if( true == CCollisionManager::CollisionRayAABB( ray_pos , ray_dir , m_cube_manager->m_cubes[ x ][ y ][ z ]->m_pos , m_cube_manager->m_cubes[ x ][ y ][ z ]->GetSize() , m_cube_manager->m_cubes[ x ][ y ][ z ]->GetWldMtx() , t , temp ) )
				{
					float distance = CCollisionManager::CalcDistance( m_camera->m_eye , m_cube_manager->m_cubes_pos[ x ][ y ][ z ] );
					if( m_cube_manager->m_cubes[ x ][ y ][ z ]->GetCubeState() == CCubeManager::CUBE_STATE::NON  )
					{
						//�J��������̋����Ń\�[�g�����}�b�v�Ɉ�U�i�[
						hit_non_cube_map.insert( std::make_pair( distance , m_cube_manager->m_cubes[ x ][ y ][ z ] ) );
					}
					else
					{
						//�J��������̋����Ń\�[�g�����}�b�v�Ɉ�U�i�[
						hit_cube_map.insert( std::make_pair( distance , m_cube_manager->m_cubes[ x ][ y ][ z ] ) );
						hit_cube_pos_map.insert( std::make_pair( distance , temp ) );
					}
				}
			}
		}
	}
	
	if( ( hit_cube_map.size() == 0 && m_select_cube_state == CCubeManager::CUBE_STATE::NON ) ) 
	{
		//���ɂ��������Ă��Ȃ��Ȃ�Z���N�g�L���[�u�̃����_�[�t���O���I�t�ɂ��Ĉȉ��̏������s��Ȃ�
		m_select_cube->SetIsRender( false );
		return;
	}
	else
	{
		//�Z���N�g�L���[�u�̃����_�[�t���O���I����
		m_select_cube->SetIsRender( true );
	}
	if( ( hit_non_cube_map.size() == 0 && m_select_cube_state != CCubeManager::CUBE_STATE::NON ) )
	{
		//���ɂ��������Ă��Ȃ��Ȃ�Z���N�g�L���[�u�̃����_�[�t���O���I�t�ɂ��Ĉȉ��̏������s��Ȃ�
		m_select_cube->SetIsRender( false );
		return;
	}
	else
	{
		//�Z���N�g�L���[�u�̃����_�[�t���O���I����
		m_select_cube->SetIsRender( true );
	}
	
	auto it = hit_cube_map.begin();
	auto it_non = hit_non_cube_map.begin();
	//�ҏW����L���[�u�̎�ނ�NON�L���[�u�̎�(���ɕ`�悳��Ă���L���[�u����������)
	if( m_select_cube_state == CCubeManager::CUBE_STATE::NON )
	{
		if( it != hit_cube_map.end() && ( *it ).second.expired() == false ) m_select_cube->m_pos = ( *it ).second.lock()->m_pos;
		if( MOUSE->ButtonPressed( MOUSE->BUTTON_LEFT ) == true )
		{
			//�����_�[�t���O���I�t�ɂ���NON�L���[�u�ɂ���
			( *it ).second.lock()->SetIsRender( false );
			( *it ).second.lock()->SetCubeState( CCubeManager::NON );

			//�j�󂵂����ɉ���炷
			CSoundManager::Play( "Content/sound/cube_destroy.wav" , CSoundManager::MODE_ONCE );
		}
	}
	//����ȊO�̃L���[�u�̎�(�L���[�u��ݒu���鏈��)
	else
	{
		if( it != hit_cube_map.end() )
		{
			auto it_pos = hit_cube_pos_map.find( ( *it ).first );
			while( it_non != hit_non_cube_map.end() )
			{
				if( true == CCollisionManager::CollisionPointAABB( ( *it_non ).second.lock()->m_pos , ( *it_non ).second.lock()->GetSize() , ( *it_pos ).second ) ) break;
				if( CCollisionManager::HIT_DIRECTION::NONE != CCollisionManager::CollisionAABBAABB( ( *it_non ).second.lock()->m_pos , ( *it_non ).second.lock()->m_prev_pos , ( *it_non ).second.lock()->GetSize() , ( *it_pos ).second , ( *it_pos ).second , D3DXVECTOR3( 0.1f , 0.1f , 0.1f ) ) ) break;

				++it_non;
			}
		}

		if( it_non != hit_non_cube_map.end() && ( *it_non ).second.expired() == false )
		{
			m_select_cube->m_pos = ( *it_non ).second.lock()->m_pos;
		}
		else
		{
			m_select_cube->SetIsRender( false );
			return;
		}

		if( MOUSE->ButtonPressed( MOUSE->BUTTON_LEFT ) == true )
		{
			//�|���S�ĂɃe�N�X�`�����Đݒ�
			if( m_select_cube_state == CCubeManager::START )
			{
				for( int i = 0 ; i < 6 ; ++i )
				{
					( *it_non ).second.lock()->SetBoardTex( CTexture::Create( "Content/graphics/start.png" ) , i );
				}
			}
			else if( m_select_cube_state == CCubeManager::GOAL )
			{
				for( int i = 0 ; i < 6 ; ++i )
				{
					( *it_non ).second.lock()->SetBoardTex( CTexture::Create( "Content/graphics/goal.png" ) , i );
				}
			}
			else if( m_select_cube_state == CCubeManager::NORMAL )
			{
				for( int i = 0 ; i < 6 ; ++i )
				{
					( *it_non ).second.lock()->SetBoardTex( CTexture::Create( "Content/graphics/yuka.jpg" ) , i );
				}
			}
			//�ݒu�������ɉ���炷
			CSoundManager::Play( "Content/sound/cube_create.wav" , CSoundManager::MODE_ONCE );
			( *it_non ).second.lock()->m_pos = m_select_cube->m_pos;
			//�L���[�u�̎�ނ�ݒ�
			( *it_non ).second.lock()->SetCubeState( m_select_cube_state );
			//NON�L���[�u����Ȃ��������_�[�t���O���I����
			( *it_non ).second.lock()->SetIsRender( true );
		}
	}
	
	{
		//���
		auto it = hit_cube_map.begin();
		while( it != hit_cube_map.end() )
		{
			( *it ).second.reset();
			++it;
		}
		hit_cube_map.clear();
		//���
		auto it_non = hit_non_cube_map.begin();
		while( it_non != hit_non_cube_map.end() )
		{
			( *it_non ).second.reset();
			++it_non;
		}	
		hit_non_cube_map.clear();
		hit_cube_pos_map.clear();
	}
}

//�����o��
void CSceneEdit::SceneExport()
{
	//�X�e�[�W�f�[�^�����o��
	for( int x = 0 ; x < m_cube_manager->m_cubes.size() ; ++x )
	{
		for( int y = 0 ; y < m_cube_manager->m_cubes[ x ].size() ; ++y )
		{
			for( int z = 0 ; z < m_cube_manager->m_cubes[ x ][ y ].size() ; ++z )
			{
				m_cube_manager->m_csv_platinum->EraseToken( m_cube_manager->m_csv_platinum->GetLayerIdx( y ) + z , x );
				std::stringstream ss;
				ss << m_cube_manager->m_cubes[ x ][ y ][ z ]->GetCubeState();
				m_cube_manager->m_csv_platinum->SetToken( m_cube_manager->m_csv_platinum->GetLayerIdx( y ) + z , x , ss.str() );
			}
		}
	}
	m_cube_manager->m_csv_platinum->ExportFile();
}
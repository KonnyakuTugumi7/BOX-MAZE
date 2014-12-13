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
	m_Csv = CCsv::Create( "Content/csv/EditData.csv" );

	//��ʒ����̏\���J�[�\��
	m_CenterCarsor = CBoard::Create( 30 , 30 , "Content/graphics/center_carsor.png" , CGraphicsManager::RENDERLIST_STATE::FRONT_2D );
	//m_CenterCarsor = std::make_shared< CBoard >();
	//m_CenterCarsor->Init( 30 , 30 , "Content/graphics/center_carsor.png" );
	//m_CenterCarsor->m_Pos = D3DXVECTOR3( 0 , 0 , 0 );

	//�J����
	m_Camera = CFPCamera::Create();
	m_Camera->SetIsGravity( false );
	m_ViewCamera = CViewCamera::Create();
	//�O���t�B�b�N�}�l�[�W���̃J�����ɐݒ�
	CGraphicsManager::m_2dCamera = m_ViewCamera;
	CGraphicsManager::m_3dCamera = m_Camera;

	//BGM���[�h
	//CSoundManager::Load( "Content/sound/play.wav" );
	CSoundManager::Load( "Content/sound/fade.wav" );
	CSoundManager::Load( "Content/sound/cube_create.wav" );
	CSoundManager::Load( "Content/sound/cube_destroy.wav" );

	//�T�E���hID
	m_SoundId = 0;

	//�L���[�u�}�l�[�W��������
	m_CubeManager = CCubeManager::Create( m_Csv->GetToken< std::string >( 2 , 0 ) );

	//�^�[�Q�b�g���W
	m_Pos = D3DXVECTOR3( 0 , m_Csv->GetToken< float >( 0 , 0 ) , 0 );

	////�^�[�Q�b�g�ړ����x
	//m_MoveSpeed = m_Csv->GetToken< float >( 1 , 0 );

	//�I���L���[�u
	m_SelectCube = CCube::Create( D3DXVECTOR3( 5 , 5 , 5 ) );
	m_SelectCube->m_Pos = D3DXVECTOR3( 0 , 0 , 0 );
	m_SelectCube->m_Col = D3DXVECTOR4( 0 , 0 , 1 , 0.5f );
	m_SelectCube->SetIsRender( true );
	m_SelectCube->SetCubeState( CCubeManager::NORMAL );

	//�I���L���[�u�Ǘ��i���o�[
	m_SelectCubeState = CCubeManager::NORMAL;

	//�}�E�X�J�[�\�����\��
	MOUSE->DisplayCursor( false );

	m_Csv = CCsv::Create( "Content/csv/EditCubeData.csv" );
	for( int i = 0 ; i < m_Csv->GetNumLine() ; ++i )
	{
		WCHAR wc_buff[ 255 ] = { 0 };
		CCommon::DXconvAnsiToWide( wc_buff , m_Csv->GetToken< std::string >( i , 0 ).c_str() , 255 );
		m_Logo.push_back( wc_buff );
	}
	//�t�H���g�𐶐�
	m_Font = CFont::Create();
	if (m_Font)
	{
		m_Font->SetSize( 30.0f , 20.0f );
		m_Font->SetAlign( CFont::LEFT );
		for( int i = 0 ; i < m_Logo.size() ; ++i ) m_Font->CreateTexture( 128 , 512 , ( i == 0 ) ? FontType::MS_GOTHIC : FontType::MS_MINCHOU , m_Logo[ i ] );
	}

	m_Csv = CCsv::Create( "Content/csv/EditData.csv" );
}

//�������
void CSceneEdit::SceneDelete()
{
	//�X�e�[�W�f�[�^�����o��
	SceneExport();
	m_CubeManager.reset();
}

//���W�b�N����
void CSceneEdit::SceneFrameMove( const float elapsed_time )
{
	//�J�����X�V
	m_Camera->UpdateCamera( elapsed_time );
	m_Pos = m_Camera->GetViewTarget();

	//�^�C�g����
	//�X�^�[�g�u���b�N�ƃS�[���u���b�N�̐���1�Ȃ�
	if( K_B->KeyPressed( K_B->KEY_ENTER ) && m_CubeManager->FindSpecifiedCubeNum( CCubeManager::CUBE_STATE::START ) == 1 && m_CubeManager->FindSpecifiedCubeNum( CCubeManager::CUBE_STATE::GOAL ) == 1 )
	{
		CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
		CSceneManager::GetInstance()->SetNextScene( &CSceneTitle::CreateScene );
	}

	if( K_B->KeyPressed( K_B->KEY_Z ) ) --m_SelectCubeState;
	if( K_B->KeyPressed( K_B->KEY_X ) ) ++m_SelectCubeState;
	if( m_SelectCubeState < 0 ) m_SelectCubeState = CCubeManager::NORMAL;
	if( m_SelectCubeState > CCubeManager::NORMAL ) m_SelectCubeState = 0;

	//�����蔻��
	SceneCollision();
}

//�`�揈��
void CSceneEdit::SceneFrameRender( const float elapsed_time )
{
	HRESULT hr;

	//��ʂ̃N���A																	  //��ʂ̐F
	V( CGraphicsManager::m_pD3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255 , 0 , 0 , 0/*255, 0, 0, 255*/ ), 1.0f, 0 ) );

	//�`��
	CGraphicsManager::SysRender( m_Camera );

	for( int i = 0 ; i < m_Logo.size() ; ++i ) if( i == m_SelectCubeState ) m_Font->Draw( m_ViewCamera , m_Logo[ i ] , D3DXVECTOR2( 50.0f , 20.0f ) , D3DXVECTOR4( Color::RED , 1.0f ) );

	////z�o�b�t�@�̂݃N���A
	//V( CGraphicsManager::m_pD3dDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255 , 0 , 0 , 0/*255, 0, 0, 255*/ ), 1.0f, 0 ) );
	//m_CenterCarsor->Render( m_ViewCamera );
}

//�T�E���h�Đ�
void CSceneEdit::ScenePlaySound()
{
	//if( m_SoundId == 0 )
	//{
	//	m_SoundId = CSoundManager::Play( "Content/sound/play.wav" , CSoundManager::MODE_LOOP );
	//}
	//else
	//{
	//	CSoundManager::Play( m_SoundId );
	//}
}

//�T�E���h��~
void CSceneEdit::SceneStopSound()
{
	CSoundManager::Stop( m_SoundId );
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
	for( int x = 0 ; x < m_CubeManager->m_Cubes.size() ; ++x )
	{
		for( int y = 0 ; y < m_CubeManager->m_Cubes[ x ].size() ; ++y )
		{
			for( int z = 0 ; z < m_CubeManager->m_Cubes[ x ][ y ].size() ; ++z )
			{
				
				//�}�E�X�|�C���^�̈ʒu���烌�C���΂�
				D3DXVECTOR3 ray_pos , ray_dir;
				CCollisionManager::ScreenToRay( ray_pos , ray_dir , 400/*MOUSE->GetPos().x*/ , 300/*MOUSE->GetPos().y*/ , m_Camera->GetViewMatrix() , m_Camera->GetProjectionMatrix() );

				float t = 0.0f;
				D3DXVECTOR3 temp( 0 , 0 , 0 );
				if( true == CCollisionManager::CollisionRayAABB( ray_pos , ray_dir , m_CubeManager->m_Cubes[ x ][ y ][ z ]->m_Pos , m_CubeManager->m_Cubes[ x ][ y ][ z ]->GetSize() , m_CubeManager->m_Cubes[ x ][ y ][ z ]->GetWldMtx() , t , temp ) )
				{
					float distance = CCollisionManager::CalcDistance( m_Camera->m_Eye , m_CubeManager->m_CubesPos[ x ][ y ][ z ] );
					if( m_CubeManager->m_Cubes[ x ][ y ][ z ]->GetCubeState() == CCubeManager::CUBE_STATE::NON  )
					{
						//�J��������̋����Ń\�[�g�����}�b�v�Ɉ�U�i�[
						hit_non_cube_map.insert( std::make_pair( distance , m_CubeManager->m_Cubes[ x ][ y ][ z ] ) );
					}
					else
					{
						//�J��������̋����Ń\�[�g�����}�b�v�Ɉ�U�i�[
						hit_cube_map.insert( std::make_pair( distance , m_CubeManager->m_Cubes[ x ][ y ][ z ] ) );
						hit_cube_pos_map.insert( std::make_pair( distance , temp ) );
					}
				}
			}
		}
	}
	
	if( ( hit_cube_map.size() == 0 && m_SelectCubeState == CCubeManager::CUBE_STATE::NON ) ) 
	{
		//���ɂ��������Ă��Ȃ��Ȃ�Z���N�g�L���[�u�̃����_�[�t���O���I�t�ɂ��Ĉȉ��̏������s��Ȃ�
		m_SelectCube->SetIsRender( false );
		return;
	}
	else
	{
		//�Z���N�g�L���[�u�̃����_�[�t���O���I����
		m_SelectCube->SetIsRender( true );
	}
	if( ( hit_non_cube_map.size() == 0 && m_SelectCubeState != CCubeManager::CUBE_STATE::NON ) )
	{
		//���ɂ��������Ă��Ȃ��Ȃ�Z���N�g�L���[�u�̃����_�[�t���O���I�t�ɂ��Ĉȉ��̏������s��Ȃ�
		m_SelectCube->SetIsRender( false );
		return;
	}
	else
	{
		//�Z���N�g�L���[�u�̃����_�[�t���O���I����
		m_SelectCube->SetIsRender( true );
	}
	
	auto it = hit_cube_map.begin();
	auto it_non = hit_non_cube_map.begin();
	//�ҏW����L���[�u�̎�ނ�NON�L���[�u�̎�(���ɕ`�悳��Ă���L���[�u����������)
	if( m_SelectCubeState == CCubeManager::CUBE_STATE::NON )
	{
		if( it != hit_cube_map.end() && ( *it ).second.expired() == false ) m_SelectCube->m_Pos = ( *it ).second.lock()->m_Pos;
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
				if( true == CCollisionManager::CollisionPointAABB( ( *it_non ).second.lock()->m_Pos , ( *it_non ).second.lock()->GetSize() , ( *it_pos ).second ) ) break;
				if( CCollisionManager::HIT_DIRECTION::NONE != CCollisionManager::CollisionAABBAABB( ( *it_non ).second.lock()->m_Pos , ( *it_non ).second.lock()->m_PrevPos , ( *it_non ).second.lock()->GetSize() , ( *it_pos ).second , ( *it_pos ).second , D3DXVECTOR3( 0.1f , 0.1f , 0.1f ) ) ) break;

				++it_non;
			}
		}

		if( it_non != hit_non_cube_map.end() && ( *it_non ).second.expired() == false )
		{
			m_SelectCube->m_Pos = ( *it_non ).second.lock()->m_Pos;
		}
		else
		{
			m_SelectCube->SetIsRender( false );
			return;
		}

		if( MOUSE->ButtonPressed( MOUSE->BUTTON_LEFT ) == true )
		{
			//�|���S�ĂɃe�N�X�`�����Đݒ�
			if( m_SelectCubeState == CCubeManager::START )
			{
				for( int i = 0 ; i < 6 ; ++i )
				{
					( *it_non ).second.lock()->SetBoardTex( CTexture::Create( "Content/graphics/start.png" ) , i );
				}
			}
			else if( m_SelectCubeState == CCubeManager::GOAL )
			{
				for( int i = 0 ; i < 6 ; ++i )
				{
					( *it_non ).second.lock()->SetBoardTex( CTexture::Create( "Content/graphics/goal.png" ) , i );
				}
			}
			else if( m_SelectCubeState == CCubeManager::NORMAL )
			{
				for( int i = 0 ; i < 6 ; ++i )
				{
					( *it_non ).second.lock()->SetBoardTex( CTexture::Create( "Content/graphics/yuka.jpg" ) , i );
				}
			}
			//�ݒu�������ɉ���炷
			CSoundManager::Play( "Content/sound/cube_create.wav" , CSoundManager::MODE_ONCE );
			( *it_non ).second.lock()->m_Pos = m_SelectCube->m_Pos;
			//�L���[�u�̎�ނ�ݒ�
			( *it_non ).second.lock()->SetCubeState( m_SelectCubeState );
			//�J���[�ݒ�
			( *it_non ).second.lock()->m_Col = D3DXVECTOR4( 1 , 1 , 1 , 1 );
			volatile D3DXVECTOR4 col =( *it_non ).second.lock()->m_Col;
			col.w++;
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
	for( int x = 0 ; x < m_CubeManager->m_Cubes.size() ; ++x )
	{
		for( int y = 0 ; y < m_CubeManager->m_Cubes[ x ].size() ; ++y )
		{
			for( int z = 0 ; z < m_CubeManager->m_Cubes[ x ][ y ].size() ; ++z )
			{
				m_CubeManager->m_CsvPlatinum->EraseToken( m_CubeManager->m_CsvPlatinum->GetLayerIdx( y ) + z , x );
				std::stringstream ss;
				ss << m_CubeManager->m_Cubes[ x ][ y ][ z ]->GetCubeState();
				m_CubeManager->m_CsvPlatinum->SetToken( m_CubeManager->m_CsvPlatinum->GetLayerIdx( y ) + z , x , ss.str() );
			}
		}
	}
	m_CubeManager->m_CsvPlatinum->ExportFile();
}
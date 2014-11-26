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

//初期化
void CScenePlay::SceneInit()
{
	//CSV
	m_csv = CCsv::Create( "Content/csv/StageData.csv" );

	//BGMロード
	CSoundManager::Load( "Content/sound/play.wav" );
	CSoundManager::Load( "Content/sound/fade.wav" );
	//サウンドID
	m_sound_id = 0;

	//カメラ
	m_camera = CFPCamera::Create();

	//キューブマネージャ初期化
	m_cube_manager = CCubeManager::Create( m_csv->GetToken< std::string >( 0 , 0 ) );

	//NONキューブの当たり判定を全てオフにする
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

	//カメラをスタートキューブへ
	CameraToStartCube();

	//UI初期化
	m_ui = CUserInterface::Create();

	//マウスカーソルを表示
	MOUSE->DisplayCursor( false );
}

//解放処理
void CScenePlay::SceneDelete()
{
	//ステージデータの書き出し
	//m_cube_manager->m_csv_platinum->ExportFile();

	//解放
	m_cube_manager.reset();
	m_ui.reset();
}

//ロジック処理
void CScenePlay::SceneFrameMove( const float elapsed_time )
{	
	//UI更新
	m_ui->Update( m_cube_manager->GetGoalPos() - m_camera->GetPosition() , m_camera->GetViewDir() , elapsed_time );

	//入力
	PlayInput();
	
	//カメラ更新
	m_camera->UpdateCamera( elapsed_time );

	//死亡
	if( m_camera->m_eye.y <= -100 )
	{
		//カメラをスタートキューブへ
		CameraToStartCube();
	}
	
	//当たり判定を全フィールドブロックと取る
	for( int x = 0 ; x < m_cube_manager->m_cubes.size() ; ++x )
	{
		for( int y = 0 ; y < m_cube_manager->m_cubes[ x ].size() ; ++y )
		{
			for( int z = 0 ; z < m_cube_manager->m_cubes[ x ][ y ].size() ; ++z )
			{
				//当たり判定フラグがオフなら以下の処理を行わない
				if( m_cube_manager->m_cubes[ x ][ y ][ z ]->GetIsCollision() == false ) continue;

				CCollisionManager::HIT_DIRECTION dir = CCollisionManager::HIT_DIRECTION::NONE;
				dir = CCollisionManager::CollisionAABBAABB( m_cube_manager->m_cubes[ x ][ y ][ z ]->m_pos , m_cube_manager->m_cubes[ x ][ y ][ z ]->m_prev_pos , m_cube_manager->m_cubes[ x ][ y ][ z ]->GetSize() , m_camera->m_eye , m_camera->m_prev_eye , m_camera->GetSize() , m_camera->m_eye );
				
				//クリア
				if( dir != CCollisionManager::NONE && m_cube_manager->m_cubes[ x ][ y ][ z ]->GetCubeState() == m_cube_manager->GOAL/*CCube::GOAL*/ )
				{
					CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );

					//ステージ01
					if( m_cube_manager->GetStageName() == "Content/csv/CubeManager01.csv" )
					{
						//キューブマネージャ初期化
						m_cube_manager = std::make_shared< CCubeManager >( m_csv->GetToken< std::string >( 1 , 0 ) );

						//カメラをスタートキューブへ
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
					//ブロックの上に衝突した場合、接地フラグをオンに
					m_camera->SetIsOnGround( true );

					//Y軸移動量の制限
					//if( m_camera->GetJumpMovement() < -1.0f ) m_camera->SetJumpMovement( -1.0f );
				}
				if( dir == CCollisionManager::HIT_DIRECTION::BOTTOM )
				{
					//ブロックの下に衝突した場合、y軸の移動量を0に
					m_camera->SetJumpMovement( 0.0f );
				}
			}
		}
	}
}

//描画処理
void CScenePlay::SceneFrameRender( const float elapsed_time )
{
	HRESULT hr;

	// 画面のクリア																	  //画面の色
	V( CGraphicsManager::m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255 , 0 , 0 , 0/*255, 0, 0, 255*/ ), 1.0f, 0 ) );

	//描画
	CGraphicsManager::SysRender( m_camera );
}

//サウンド再生
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

//サウンド停止
void CScenePlay::SceneStopSound()
{
	CSoundManager::Stop( m_sound_id );
}

//入力
void CScenePlay::PlayInput()
{
	//タイトルへ
	if( K_B->KeyPressed( K_B->KEY_ENTER ) )
	{
		CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );
		CSceneManager::GetInstance()->SetNextScene( &CSceneTitle::CreateScene );
	}

	//スタート位置へ
	if( K_B->KeyPressed( K_B->KEY_P ) )
	{
		CameraToStartCube();
	}

	//ゴール位置へ
	if( K_B->KeyPressed( K_B->KEY_O ) )
	{
		m_camera->m_eye = m_cube_manager->GetGoalPos();
	}
}

//カメラをスタートキューブへ
void CScenePlay::CameraToStartCube()
{
	//カメラ初期化
	m_camera->Init();
	//座標
	m_camera->m_eye = m_cube_manager->GetStartPos();
}
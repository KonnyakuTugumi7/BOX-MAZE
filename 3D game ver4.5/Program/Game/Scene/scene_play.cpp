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
	m_Csv = CCsv::Create( "Content/csv/StageData.csv" );

	//BGMロード
	CSoundManager::Load( "Content/sound/play.wav" );
	CSoundManager::Load( "Content/sound/fade.wav" );
	//サウンドID
	m_SoundId = 0;

	//カメラ
	m_Camera = CFPCamera::Create();
	//グラフィックマネージャのカメラに設定
	//CGraphicsManager::m_2dCamera = m_ViewCamera;
	CGraphicsManager::m_3dCamera = m_Camera;

	//キューブマネージャ初期化
	m_CubeManager = CCubeManager::Create( m_Csv->GetToken< std::string >( 0 , 0 ) );

	//NONキューブの当たり判定を全てオフにする
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

	//カメラをスタートキューブへ
	CameraToStartCube();

	//UI初期化
	m_Ui = CUserInterface::Create();

	//マウスカーソルを表示
	MOUSE->DisplayCursor( false );
}

//解放処理
void CScenePlay::SceneDelete()
{
	//ステージデータの書き出し
	//m_CubeManager->m_CsvPlatinum->ExportFile();

	//解放
	m_CubeManager.reset();
	m_Ui.reset();
}

//ロジック処理
void CScenePlay::SceneFrameMove( const float elapsed_time )
{	
	//UI更新
	m_Ui->Update( m_CubeManager->GetGoalPos() - m_Camera->GetPosition() , m_Camera->GetViewDir() , elapsed_time );

	//入力
	PlayInput();
	
	//カメラ更新
	m_Camera->UpdateCamera( elapsed_time );

	//死亡
	if( m_Camera->m_Eye.y <= -100 )
	{
		//カメラをスタートキューブへ
		CameraToStartCube();
	}
	
	//当たり判定を全フィールドブロックと取る
	for( int x = 0 ; x < m_CubeManager->m_Cubes.size() ; ++x )
	{
		for( int y = 0 ; y < m_CubeManager->m_Cubes[ x ].size() ; ++y )
		{
			for( int z = 0 ; z < m_CubeManager->m_Cubes[ x ][ y ].size() ; ++z )
			{
				//当たり判定フラグがオフなら以下の処理を行わない
				if( m_CubeManager->m_Cubes[ x ][ y ][ z ]->GetIsCollision() == false ) continue;

				CCollisionManager::HIT_DIRECTION dir = CCollisionManager::HIT_DIRECTION::NONE;
				dir = CCollisionManager::CollisionAABBAABB( m_CubeManager->m_Cubes[ x ][ y ][ z ]->m_Pos , m_CubeManager->m_Cubes[ x ][ y ][ z ]->m_PrevPos , m_CubeManager->m_Cubes[ x ][ y ][ z ]->GetSize() , m_Camera->m_Eye , m_Camera->m_PrevEye , m_Camera->GetSize() , m_Camera->m_Eye );
				
				//クリア
				if( dir != CCollisionManager::NONE && m_CubeManager->m_Cubes[ x ][ y ][ z ]->GetCubeState() == m_CubeManager->GOAL/*CCube::GOAL*/ )
				{
					CSoundManager::Play( "Content/sound/fade.wav" , CSoundManager::MODE_ONCE );

					//ステージ01
					if( m_CubeManager->GetStageName() == "Content/csv/CubeManager01.csv" )
					{
						//キューブマネージャ初期化
						m_CubeManager = std::make_shared< CCubeManager >( m_Csv->GetToken< std::string >( 1 , 0 ) );

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
					m_Camera->SetIsOnGround( true );

					//Y軸移動量の制限
					//if( m_Camera->GetJumpMovement() < -1.0f ) m_Camera->SetJumpMovement( -1.0f );
				}
				if( dir == CCollisionManager::HIT_DIRECTION::BOTTOM )
				{
					//ブロックの下に衝突した場合、y軸の移動量を0に
					m_Camera->SetJumpMovement( 0.0f );
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
	V( CGraphicsManager::m_pD3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255 , 0 , 0 , 0/*255, 0, 0, 255*/ ), 1.0f, 0 ) );

	//描画
	CGraphicsManager::SysRender( m_Camera );
}

//サウンド再生
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

//サウンド停止
void CScenePlay::SceneStopSound()
{
	CSoundManager::Stop( m_SoundId );
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
		m_Camera->m_Eye = m_CubeManager->GetGoalPos();
	}
}

//カメラをスタートキューブへ
void CScenePlay::CameraToStartCube()
{
	//カメラ初期化
	m_Camera->Init();
	//座標
	m_Camera->m_Eye = m_CubeManager->GetStartPos();
}
#include "DXUT.h"
#include "scene_fade.h"
#include "../../Library/Support/common.h"
#include "../../Library/Input/input.h"
#include "../../Library/Sound/sound.h"
#include "../../Library/Sound/sound_manager.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../GameEngine/Scene/scene_manager.h"
#include "../../GameEngine/Graphic/sprite.h"
#include "../../Game/Scene/scene_title.h"
#include "../../Game/Camera/fp_camera.h"
#include "../../Game/Camera/tp_camera.h"
#include "../../GameEngine/Scene/scene_manager.h"

//初期化
void CSceneFade::SceneInit()
{
	//CSV
	m_Csv = CCsv::Create( "Content/csv/FadeData.csv" );

	//カメラ
	m_Camera = CTPCamera::Create();

	//フェード状態管理ナンバー
	m_FadeMode = FADE_NON;
	
	//フェード画像
	//m_Fade = CBoard::Create( CGraphicsManager::m_WindowWidth , CGraphicsManager::m_WindowHeight , "Content/graphics/fade_img.bmp" , CGraphicsManager::RENDERLIST_STATE::FRONT_2D );
	m_Fade = std::make_shared< CBoard >();
	m_Fade->Init( CGraphicsManager::m_WindowWidth , CGraphicsManager::m_WindowHeight , "Content/graphics/fade_img.bmp" );
	m_Fade->m_Col = D3DXVECTOR4( 1 , 1 , 1 , 0 );
	m_Fade->SetCameraDistance( 0.1f );
	m_Fade->SetIsRender( false );
}

//解放処理
void CSceneFade::SceneDelete()
{
	//フェード画像解放
	m_Fade.reset();
}

//ロジック処理
void CSceneFade::SceneFrameMove( const float elapsed_time )
{	
	//フェード中でなければ以下の処理を行わない
	if( m_FadeMode == FADE_NON )
	{
		m_Fade->SetIsRender( false );
		return;
	}
	else
	{
		m_Fade->SetIsRender( true );
	}

	//少しずつアルファ値を入れていく
	m_Fade->m_Col.w += 1.0f / m_FadeMode * elapsed_time;

	//フェードイン
	if( m_FadeMode == FADE_IN )
	{
		//予約されているシーケンスに移行
		if( 0.0f > m_Fade->m_Col.w )
		{
			//念のためリミット
			m_Fade->m_Col.w = 0.0f;

			//通常状態へ
			m_FadeMode = FADE_NON;
		}
	}
	//フェードアウト
	else if( m_FadeMode == FADE_OUT )
	{
		if( 1.0f < m_Fade->m_Col.w )
		{	
			//念のためリミット
			m_Fade->m_Col.w = 1.0f;

			//フェードインへ
			m_FadeMode = FADE_IN;
		}
	}
}

//描画処理
void CSceneFade::SceneFrameRender( const float elapsed_time )
{
	//フェード中でなければ以下の処理を行わない
	if( m_FadeMode == FADE_NON ) return;

	//描画
	m_Fade->Render( m_Camera );
}

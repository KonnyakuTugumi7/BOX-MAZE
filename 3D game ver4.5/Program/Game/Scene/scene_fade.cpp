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
	m_csv = CCsv::Create( "Content/csv/FadeData.csv" );

	//カメラ
	m_camera = CTPCamera::Create();

	//フェード状態管理ナンバー
	m_fade_mode = FADE_NON;

	//フェード用画像ロード
	//m_fade = CSprite::Create( "Content/graphics/fade_img.bmp" , CGraphicsManager::FRONT_2D );
	m_fade = std::make_shared< CSprite >();
	m_fade->Init( "Content/graphics/fade_img.bmp" );
	m_fade->m_pos = D3DXVECTOR3( m_csv->GetToken< int >( 0 , 0 ) , m_csv->GetToken< int >( 0 , 1 ) , 0 );
	m_fade->m_col = D3DXVECTOR4( 1 , 1 , 1 , 0 );
	m_fade->SetCameraDistance( 0.1f );
}

//解放処理
void CSceneFade::SceneDelete()
{
	//フェード画像解放
	m_fade.reset();
}

//ロジック処理
void CSceneFade::SceneFrameMove( const float elapsed_time )
{	
	//フェード中でなければ以下の処理を行わない
	if( m_fade_mode == FADE_NON ) return;

	//少しずつアルファ値を入れていく
	m_fade->m_col.w += 1.0f / m_fade_mode * elapsed_time;

	//フェードイン
	if( m_fade_mode == FADE_IN )
	{
		//予約されているシーケンスに移行
		if( 0.0f > m_fade->m_col.w )
		{
			//念のためリミット
			m_fade->m_col.w = 0.0f;

			//通常状態へ
			m_fade_mode = FADE_NON;
		}
	}
	//フェードアウト
	else if( m_fade_mode == FADE_OUT )
	{
		if( 1.0f < m_fade->m_col.w )
		{	
			//念のためリミット
			m_fade->m_col.w = 1.0f;

			//フェードインへ
			m_fade_mode = FADE_IN;
		}
	}
}

//描画処理
void CSceneFade::SceneFrameRender( const float elapsed_time )
{
	//フェード中でなければ以下の処理を行わない
	if( m_fade_mode == FADE_NON ) return;

	//描画
	m_fade->Render( m_camera );
}

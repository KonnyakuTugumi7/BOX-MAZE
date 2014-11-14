#include "DXUT.h"
#include "animation.h"
#include "../../Library/Support/time.h"

//コンストラクタ
CAnimation::CAnimation() : 
	m_img_size( 0 , 0 , 0 ) , 
	m_center( 0 , 0 , 0 ) , 
	m_animation_frame( 0 ) ,
	m_is_loop( true ) ,
	m_is_active( true ) 
{
	m_animation_timer = std::make_shared< CTime >( 2 );
}

//デストラクタ
CAnimation::~CAnimation()
{
	//解放
	SAFE_RELEASE( m_texture );
	m_rect.clear();
	m_animation_timer.reset();
}

//再生
void CAnimation::Play()
{
	//描画フラグがオフの時以下の処理を行わない
	if( m_is_active == false ) return;

	//分割なしで読み込んだ時以下の処理を行わない
	if( m_rect.size() == 1 ) return;

	//アニメーションカウントを進める
	if( m_animation_timer->Update() == true )
	{
		//次の画像に進む(画像の分割数まで)
		++m_animation_frame;

		//カウントをリセット
		m_animation_timer->Start();
	}

	//単発
	if( m_is_loop == false && m_rect.size() == m_animation_frame )
	{
		//描画フラグをオフにする
		m_is_active = false;

		//アニメーションフレームを最初に戻す
		m_animation_frame = 0;
			 
		//カウントをリセット
		m_animation_timer->Start();
	}

	//アニメフレーム番号をループさせる
	m_animation_frame %= m_rect.size();
}
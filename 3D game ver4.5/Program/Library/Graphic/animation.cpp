#include "DXUT.h"
#include "animation.h"
#include "../../Library/Support/time.h"

//コンストラクタ
CAnimation::CAnimation() : 
	m_ImgSize( 0 , 0 , 0 ) , 
	m_Center( 0 , 0 , 0 ) , 
	m_AnimationFrame( 0 ) ,
	m_IsLoop( true ) ,
	m_IsActive( true ) 
{
	m_AnimationTimer = std::make_shared< CTime >( 2 );
}

//デストラクタ
CAnimation::~CAnimation()
{
	//解放
	SAFE_RELEASE( m_Texture );
	m_Rect.clear();
	m_AnimationTimer.reset();
}

//再生
void CAnimation::Play()
{
	//描画フラグがオフの時以下の処理を行わない
	if( m_IsActive == false ) return;

	//分割なしで読み込んだ時以下の処理を行わない
	if( m_Rect.size() == 1 ) return;

	//アニメーションカウントを進める
	if( m_AnimationTimer->Update() == true )
	{
		//次の画像に進む(画像の分割数まで)
		++m_AnimationFrame;

		//カウントをリセット
		m_AnimationTimer->Start();
	}

	//単発
	if( m_IsLoop == false && m_Rect.size() == m_AnimationFrame )
	{
		//描画フラグをオフにする
		m_IsActive = false;

		//アニメーションフレームを最初に戻す
		m_AnimationFrame = 0;
			 
		//カウントをリセット
		m_AnimationTimer->Start();
	}

	//アニメフレーム番号をループさせる
	m_AnimationFrame %= m_Rect.size();
}
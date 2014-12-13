#pragma once

#include <memory>
#include <time.h>
#include <Windows.h>
#include <string>
#include <sstream>

class CTime;
typedef std::shared_ptr< CTime > TimeSP;

//タイムクラス
class CTime{
private:
	//計測時間
	int m_MeasuringTime;

	//計測開始時間
    float m_Start;

	//計測経過時間
	float m_Elapsed;

public:
	//コンストラクタ
	//単純に経過時間を計測する用
    CTime() : m_Start( 0.0f ) , 
			  m_Elapsed( 0.0f )
	{}

	//コンストラクタ
	//時間制限を設けたタイマー用
	//引数1：計測時間
	CTime( const int time ) : m_MeasuringTime( time ) , 
							   m_Start( 0.0f ) , 
							   m_Elapsed( 0.0f )
	{}

	//デストラクタ
	~CTime(){}

	//生成
	static TimeSP Create(){ return std::make_shared< CTime >(); }
	//引数1：計測時間
	static TimeSP Create( const int time ){ return std::make_shared< CTime >( time ); }

	//スタート
    void Start()
    {
		//計測開始時間を保存
		m_Start = ( float )GetTickCount();
		//計測経過時間をリセット
		m_Elapsed = 0.0f;
    }

	//時間を計測する
	bool Update()
	{
		float end = ( float )GetTickCount();
		m_Elapsed = ( end - m_Start ) / 1000;

		//計測時間まで達したかどうか
		return ( m_Elapsed > m_MeasuringTime );
	}

	//経過時間をstringにして返す
	std::string ElapsedToString()
	{
		std::stringstream ss;
		ss << m_Elapsed;
		return ss.str();
	}

	//-------------------------------セッター------------------------------

	inline void SetTimer( const int time ){ m_MeasuringTime = time; }

	//------------------------------ゲッター-------------------------------

	inline const int& GetMeasuringTime() const { return m_MeasuringTime; }

	inline const float& GetElapsed() const { return m_Elapsed; }

};
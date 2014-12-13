#pragma once

#include <memory>
#include <time.h>
#include <Windows.h>
#include <string>
#include <sstream>

class CTime;
typedef std::shared_ptr< CTime > TimeSP;

//�^�C���N���X
class CTime{
private:
	//�v������
	int m_MeasuringTime;

	//�v���J�n����
    float m_Start;

	//�v���o�ߎ���
	float m_Elapsed;

public:
	//�R���X�g���N�^
	//�P���Ɍo�ߎ��Ԃ��v������p
    CTime() : m_Start( 0.0f ) , 
			  m_Elapsed( 0.0f )
	{}

	//�R���X�g���N�^
	//���Ԑ�����݂����^�C�}�[�p
	//����1�F�v������
	CTime( const int time ) : m_MeasuringTime( time ) , 
							   m_Start( 0.0f ) , 
							   m_Elapsed( 0.0f )
	{}

	//�f�X�g���N�^
	~CTime(){}

	//����
	static TimeSP Create(){ return std::make_shared< CTime >(); }
	//����1�F�v������
	static TimeSP Create( const int time ){ return std::make_shared< CTime >( time ); }

	//�X�^�[�g
    void Start()
    {
		//�v���J�n���Ԃ�ۑ�
		m_Start = ( float )GetTickCount();
		//�v���o�ߎ��Ԃ����Z�b�g
		m_Elapsed = 0.0f;
    }

	//���Ԃ��v������
	bool Update()
	{
		float end = ( float )GetTickCount();
		m_Elapsed = ( end - m_Start ) / 1000;

		//�v�����Ԃ܂ŒB�������ǂ���
		return ( m_Elapsed > m_MeasuringTime );
	}

	//�o�ߎ��Ԃ�string�ɂ��ĕԂ�
	std::string ElapsedToString()
	{
		std::stringstream ss;
		ss << m_Elapsed;
		return ss.str();
	}

	//-------------------------------�Z�b�^�[------------------------------

	inline void SetTimer( const int time ){ m_MeasuringTime = time; }

	//------------------------------�Q�b�^�[-------------------------------

	inline const int& GetMeasuringTime() const { return m_MeasuringTime; }

	inline const float& GetElapsed() const { return m_Elapsed; }

};
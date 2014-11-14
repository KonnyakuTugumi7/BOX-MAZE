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
	int m_measuring_time;

	//�v���J�n����
    float m_start;

	//�v���o�ߎ���
	float m_elapsed;

public:
	//�R���X�g���N�^
	//�P���Ɍo�ߎ��Ԃ��v������p
    CTime() : m_start( 0.0f ) , 
			  m_elapsed( 0.0f )
	{}

	//�R���X�g���N�^
	//���Ԑ�����݂����^�C�}�[�p
	//����1�F�v������
	CTime( const int time ) : m_measuring_time( time ) , 
							   m_start( 0.0f ) , 
							   m_elapsed( 0.0f )
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
		m_start = ( float )GetTickCount();
		//�v���o�ߎ��Ԃ����Z�b�g
		m_elapsed = 0.0f;
    }

	//���Ԃ��v������
	bool Update()
	{
		float end = ( float )GetTickCount();
		m_elapsed = ( end - m_start ) / 1000;

		//�v�����Ԃ܂ŒB�������ǂ���
		return ( m_elapsed > m_measuring_time );
	}

	//�o�ߎ��Ԃ�string�ɂ��ĕԂ�
	std::string ElapsedToString()
	{
		std::stringstream ss;
		ss << m_elapsed;
		return ss.str();
	}

	//-------------------------------�Z�b�^�[------------------------------

	inline void SetTimer( const int time ){ m_measuring_time = time; }

	//------------------------------�Q�b�^�[-------------------------------

	inline const int& GetMeasuringTime() const { return m_measuring_time; }

	inline const float& GetElapsed() const { return m_elapsed; }

};
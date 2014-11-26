#pragma once

#include "DXUT.h"
#include "../../Library/Support/csv_platinum.h"
#include "../../GameEngine/Graphic/cube.h"
#include <memory>
#include <vector>

#pragma warning( disable:4482 ) //��W���̊g���@�\���g�p����Ă��܂�: enum 'CCubeManager::CUBE_STATE' �����薼�Ŏg�p����܂�
#pragma warning( disable:4244 ) //'int' ���� 'FLOAT' �ւ̕ϊ��ł��B�f�[�^��������\��������܂��B

class CCubeManager;
typedef std::shared_ptr< CCubeManager > CubeManagerSP;

//typedef std::vector< CubeSP > CUBE1;
//typedef std::vector< std::vector< CubeSP > > CUBE2;
//typedef std::vector< std::vector< std::vector< CubeSP > > > CUBE3;
typedef std::vector< CubeSP > CUBE1;
typedef std::vector< CUBE1 > CUBE2;
typedef std::vector< CUBE2 > CUBE3;

//�L���[�u�}�l�[�W��
class CCubeManager{
private:
	//���݂̃X�e�[�W��(�S�X�e�[�W�N���A����Ŏg��)
	std::string m_stage_name;

	//�X�^�[�g�L���[�u�̍��W�ۑ��p
	D3DXVECTOR3 m_start_pos;

	//�S�[���L���[�u�̍��W�ۑ��p
	D3DXVECTOR3 m_goal_pos;

public:
	//�L���[�u�̂R�����z��
	CUBE3 m_cubes;

	std::vector< std::vector<std::vector< D3DXVECTOR3 > > > m_cubes_pos;

	//CSVPlatinum
	CsvPlatinumSP m_csv_platinum;

	//�L���[�u�̎��
	enum CUBE_STATE{
		NON = 0 ,
		START = 1 ,
		GOAL = 2 ,
		NORMAL = 3 ,
		CUBE_STATE_NUM = 4
	};

public:
	//�R���X�g���N�^
	//����1:�t�@�C���p�X
	CCubeManager( const std::string file_path );

	//�f�X�g���N�^
	~CCubeManager(){};

	//����
	//����1:�t�@�C���p�X
	static CubeManagerSP Create( const std::string file_path ){ return std::make_shared< CCubeManager >( file_path ); }

	//�w�肵����ނ̃L���[�u���������邩����
	int FindSpecifiedCubeNum( const CUBE_STATE cube );

	//�X�e�[�W���Q�b�^�[
	std::string GetStageName(){ return m_stage_name; }

	//�X�^�[�g�L���[�u���W�Q�b�^�[
	D3DXVECTOR3 GetStartPos(){ return m_start_pos; }

	//�S�[���L���[�u���W�Q�b�^�[
	D3DXVECTOR3 GetGoalPos(){ return m_goal_pos; }
};
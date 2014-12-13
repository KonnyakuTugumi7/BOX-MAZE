#pragma once

#include "../../Library/Support/csv.h"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <memory>

class CCsvPlatinum;
typedef std::shared_ptr< CCsvPlatinum > CsvPlatinumSP;

//CSV_Platinum�N���X(CSV��Platinum��p�N���X)
//CSV�t�@�C�����ɕK�v�ɂȂ�(�g���܂킷�Ȃ璀��CSV�t�@�C����ǂݍ��ނׂ�)
//�܂��R���X�g���N�^(�܂���ImportFile�֐�)��CSV�t�@�C����ǂݍ���
//�e�[�u���Ɋi�[���ꂽ�g�[�N����F�X����
//�����������l��ExportFile�֐��ŏ����o��
class CCsvPlatinum : public CCsv{
private:
	//�}�b�v�̉���
	int m_Width;

	//�}�b�v�̉��s
	int m_Depth;

	//�}�b�v�`�b�v1�̕�(pixel)
	int m_ChipWidth;

	//�}�b�v�`�b�v1�̍���(pixel)
	int m_ChipHeight;

	//���C���[�̐�
	int m_LayerNum;

	//���C���[�C���f�b�N�X
	//���C���[0�𔲂������C���f�b�N�X�������Ŋi�[����Ă���
	std::vector< int > m_LayerIdx;

public:
	//�R���X�g���N�^(�V�����X�e�[�W�����Ƃ�)
	CCsvPlatinum(){}

	//�R���X�g���N�^(���ɂ���X�e�[�W�̎�)
	//����1:�t�@�C���p�X
	CCsvPlatinum( const std::string& file_path )
	{
		//�t�@�C���p�X
		m_Path = file_path;

		//CSV�t�@�C���ǂݍ���
		ImportFile( file_path );

		//�}�b�v�̕�
		m_Width = GetToken< int >( 0 , 0 );

		//�}�b�v�̉��s
		m_Depth = GetToken< int >( 0 , 1 );

		//���C���[�̐�
		m_LayerNum = GetToken< int >( 0 , 4 );

		for( int i = 0 ; i < GetNumLine() ; i++ )
		{
			if( GetToken< std::string >( i , 0 ) == "" )
			{
				m_LayerIdx.push_back( i );
			}
		}
	}

	//�f�X�g���N�^
	~CCsvPlatinum()
	{
		//���
		EraseToken();
		m_LayerIdx.clear();
	}

	//����
	//����1:�t�@�C���p�X
	static CsvPlatinumSP Create( const std::string& file_path ){ return std::make_shared< CCsvPlatinum >( file_path ); }

	//�g�[�N������(�w��̃g�[�N�����������݂��邩)
	//����1:��������g�[�N��
	//�߂�l:�w��̃g�[�N���̐�
	virtual int FindSpecifiedTokenNum( const std::string token )
	{
		int num = 0;
		for( int line = 1 ; line < GetNumLine() ; ++line ) num += std::count( m_CsvTable[ line ].begin() , m_CsvTable[ line ].end() , token );
		
		return num;
	}

	//���C���[�C���f�b�N�X�Q�b�^�[
	//����1:���C���[�ԍ�
	inline int GetLayerIdx( const int layer = 0 )
	{
		//���C���[�͋󔒂ŋ�؂��Ă�̂Ń��C���[0�̃C���f�b�N�X�͕ێ����ĂȂ�
		//����ă��C���[0������O�Ńx�^�ł�
		if( layer == 0 )
		{
			return 1;
		}
		else
		{
			return m_LayerIdx[ layer - 1 ] + 1;
		}
	}

	//�}�b�v���Q�b�^�[
	inline int GetMapWidth(){ return m_Width; }

	//�}�b�v���s�Q�b�^�[
	inline int GetMapDepth(){ return m_Depth; }

	//���C���[���Q�b�^�[
	inline int GetLayerNum(){ return m_LayerNum; }
};
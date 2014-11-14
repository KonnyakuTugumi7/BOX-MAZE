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
	int m_width;

	//�}�b�v�̉��s
	int m_depth;

	//�}�b�v�`�b�v1�̕�(pixel)
	int m_chip_width;

	//�}�b�v�`�b�v1�̍���(pixel)
	int m_chip_height;

	//���C���[�̐�
	int m_layer_num;

	//���C���[�C���f�b�N�X
	//���C���[0�𔲂������C���f�b�N�X�������Ŋi�[����Ă���
	std::vector< int > m_layer_idx;

public:
	////�R���X�g���N�^
	//CCsvPlatinum() : m_width( 0 ) , 
	//				 m_depth( 0 ) , 
	//				 m_chip_width( 0 ) , 
	//				 m_chip_height( 0 ) , 
	//				 m_layer_num( 0 )
	//{
	//	m_path = ""; 
	//}

	//�R���X�g���N�^
	//����1:�t�@�C���p�X
	CCsvPlatinum( const std::string& file_path )
	{
		//�t�@�C���p�X
		m_path = file_path;

		//CSV�t�@�C���ǂݍ���
		ImportFile( file_path );

		//�}�b�v�̕�
		m_width = GetToken< int >( 0 , 0 );

		//�}�b�v�̉��s
		m_depth = GetToken< int >( 0 , 1 );

		//���C���[�̐�
		m_layer_num = GetToken< int >( 0 , 4 );

		for( int i = 0 ; i < GetNumLine() ; i++ )
		{
			if( GetToken< std::string >( i , 0 ) == "" )
			{
				m_layer_idx.push_back( i );
			}
		}
	}

	//�f�X�g���N�^
	~CCsvPlatinum()
	{
		//���
		EraseToken();
		m_layer_idx.clear();
	}

	//����
	//����1:�t�@�C���p�X
	static CsvPlatinumSP Create( const std::string& file_path ){ return std::make_shared< CCsvPlatinum >( file_path ); }

	////�g�[�N���Z�b�^�[
	////����1:�s , ����2:�� , ����3:���C���[�ԍ� ����4:�g�[�N��
	//void SetToken( const int line , const int row , const int layer , const std::string& token )
	//{
	//	int layer_idx = GetLayerIdx( layer );

	//	//�w��Z�������������ǂ���
	//	if( !IsValidPos( line , layer_idx + row ) )
	//	{
	//		//�Z�����m��
	//		//���݂̃��C�����w�胉�C�����傫��������
	//		if( m_csv_table.size() < line + 1 ) m_csv_table.resize( ( line + 1 ) );
	//		m_csv_table[ line ].resize( layer_idx + row + 1 );
	//	}

	//	//�C�e���[�^
	//	std::vector< std::string >::iterator it = m_csv_table[ line ].begin() + layer_idx + row;

	//	//�g�[�N�����Z�b�g
	//	m_csv_table[ line ].insert( it , 1 , token );

	//	//std::vector��insert�֐��̓C�e���[�^���w���ꏊ�̒��O�ɒl��}������̂�
	//	//�}�����ꂽ�Z����1���̃Z�����ɃG���v�e�B�[�g�[�N���������Ă�����폜����
	//	std::string empty = m_csv_table[ line ][ layer_idx + row + 1 ];
	//	if( empty == "" )
	//	{
	//		EraseToken( line , row + 1 , layer );
	//	}
	//}

	////�g�[�N���I�[���C���[�T�[
	//void EraseToken()
	//{
	//	m_csv_table.clear();
	//}

	////�g�[�N���C���[�T�[
	////����1:�s , ����2:�� , ����3:���C���[�ԍ�
	//void EraseToken( const int line , const int row , const int layer )
	//{
	//	int layer_idx = GetLayerIdx( layer );

	//	//�C�e���[�^
	//	std::vector< std::string >::iterator it = m_csv_table[ line ].begin() + layer_idx + row;

	//	//�폜
	//	it = m_csv_table[ line ].erase( it );
	//}

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
			return m_layer_idx[ layer - 1 ] + 1;
		}
	}

	//�}�b�v���Q�b�^�[
	inline int GetMapWidth(){ return m_width; }

	//�}�b�v���s�Q�b�^�[
	inline int GetMapDepth(){ return m_depth; }

	//���C���[���Q�b�^�[
	inline int GetLayerNum(){ return m_layer_num; }
};
#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <memory>
#include <map>

#pragma warning( disable : 4018 )
#pragma warning( disable : 4127 )

class CCsv;
typedef std::shared_ptr< CCsv > CsvSP;

//CSV�N���X
//CSV�t�@�C�����ɕK�v�ɂȂ�(�g���܂킷�Ȃ璀��CSV�t�@�C����ǂݍ��ނׂ�)
//�܂��R���X�g���N�^(�܂���ImportFile�֐�)��CSV�t�@�C����ǂݍ���
//�e�[�u���Ɋi�[���ꂽ�g�[�N����F�X����
//�����������l��ExportFile�֐��ŏ����o��
//CSV�t�@�C����//����ꂽ�s�̓R�����g�����ƂȂ�A�f�[�^�e�[�u���ɂ͓ǂݍ��܂�Ȃ�
//�������A�R�����g�͑Ή�����l��1��̍s�ɏ����ׂ�
class CCsv{
protected:
	//�t�@�C���p�X
	std::string m_Path;

	//CSV�t�@�C���̃f�[�^���i�[����e�[�u��
	std::vector< std::vector< std::string > > m_CsvTable;

	//CSV�t�@�C���̃R�����g���i�[����}�b�v
	//key:�f�[�^�e�[�u���̉��s�ڂ̒l�ɑΉ����邩(�R�����g�͏���) , value:�R�����g
	std::map< int , std::string > m_CommentMap;

public:
	//�R���X�g���N�^
	CCsv(): m_Path( "" ){}

	//�R���X�g���N�^
	//����1:�t�@�C���p�X
	CCsv( const std::string& file_path ) : m_Path( file_path )
	{
		//CSV�t�@�C���ǂݍ���
		ImportFile( file_path );
	}

	//�f�X�g���N�^
	virtual ~CCsv()
	{
		//�g�[�N���e�[�u�����
		EraseToken();

		//�R�����g���
		m_CommentMap.clear();
	}

	//����
	//����1:�t�@�C���p�X
	static CsvSP Create( const std::string& file_path ){ return std::make_shared< CCsv >( file_path ); }
	
	//CSV�t�@�C���ǂݍ���
	//����1:�t�@�C���p�X
	virtual void ImportFile( const std::string& file_path );

	//CSV�t�@�C�������o��
	void ExportFile();

	//�t�@�C�����ǂݍ��܂ꂽ���ǂ���
	//�߂�l:�t�@�C���Ǎ��ς�:true , �t�@�C�����Ǎ�:false
	bool IsImportFile() const
	{
		//��������r���ċ󂩂ǂ������ׂ�
		if( strcmp( m_Path.c_str() , "" ) == 0 ) return false;

		return true;
	}

	//�Z���̗L������
	//����1:�s , ����2:��
	//�߂�l:�L���ȃZ���w��:true , �����ȃZ���w��:false
	bool IsValidPos( const int line , const int row ) const
	{
		//�ǂݍ��܂�Ă��邩�ǂ���
		if( !IsImportFile() ) return false;

		//�g�[�N���e�[�u�����󂩂ǂ���
		if( m_CsvTable.empty() ) return false;

		//�w��̍s���L�����ǂ���
		if( ( line < 0 ) || ( GetNumLine() <= line ) ) return false;

		//�w��̗񂪗L�����ǂ���
		if( ( row < 0 ) || ( ( int ) m_CsvTable[ line ].size() <= row ) ) return false;

		return true;
	}

	//�g�[�N������(�w��̃g�[�N�����������݂��邩)
	//����1:��������g�[�N��
	//�߂�l:�w��̃g�[�N���̐�
	virtual int FindSpecifiedTokenNum( const std::string token )
	{
		int num = 0;
		for( int line = 0 ; line < GetNumLine() ; ++line ) num += std::count( m_CsvTable[ line ].begin() , m_CsvTable[ line ].end() , token );
		return num;
	}

	//�g�[�N���Q�b�^�[
	//����1:�s , ����2:��
	//�߂�l:�g�[�N��
	template < typename T >
	T GetToken( const int line , const int row ) const
	{
		//����int,float,string�ɂ����Ή����Ă��Ȃ��̂ł���ȊO�̎���assert���o��
		assert( 0 );
		return;
	}
	//int�p
	template <>
	int GetToken< int >( const int line , const int row ) const
	{
		//�w��Z�������������ǂ���
		if( !IsValidPos( line , row ) ) assert( 0 );

		//�e�[�u���̃g�[�N����int�^�ɕϊ����Ď擾
		return strtol( ( const char* )m_CsvTable[ line ][ row ].c_str() , NULL , 10 );
	}
	//float�p
	template <>
	float GetToken< float >( const int line , const int row ) const
	{
		//�w��Z�������������ǂ���
		if( !IsValidPos( line , row ) ) assert( 0 );

		//�e�[�u���̃g�[�N����float�^�ɕϊ����Ď擾
		return ( float )strtod( ( const char* )m_CsvTable[ line ][ row ].c_str() , NULL );
	}
	//string�p
	template<>
	std::string GetToken< std::string >( const int line , const int row ) const
	{
		//�w��Z�������������ǂ���
		if( !IsValidPos( line , row ) ) assert( 0 );

		//�e�[�u���̃g�[�N�����擾
		return m_CsvTable[ line ][ row ];
	}

	//�g�[�N���T�C�Y�Q�b�^�[
	//����1:�s , ����2:��
	//�߂�l:�L���ȃZ���w��:�Z���Ԓn , �����ȃZ���w��:-1
	int GetTokenSize( const int line , const int row ) const
	{
		//�w��Z�������������ǂ���
		if( !IsValidPos( line , row ) ) return -1;

		return ( int )m_CsvTable[ line ][ row ].size();
	}

	//��Q�b�^�[
	//����1:��
	//�߂�l:�L���ȃZ���w��:�w��s�̗� , �����ȃZ���w��:-1
	int GetNumRow( const int line ) const
	{
		//�w��Z�������������ǂ���
		if( !IsValidPos( line , 0 ) ) return -1;

		return ( int )m_CsvTable[ line ].size();
	}

	//�s�Q�b�^�[
	//�߂�l:�s��
	int GetNumLine() const
	{
		return ( int )m_CsvTable.size();
	}

	//�g�[�N���Z�b�^�[
	//����1:�s , ����2:�� , ����3:�g�[�N��
	void SetToken( const int line , const int row , const std::string& token )
	{
		//�w��Z�������������ǂ���
		if( !IsValidPos( line , row ) )
		{
			//�Z�����m��
			//���݂̃��C�����w�胉�C�����傫��������
			if( m_CsvTable.size() < line + 1 ) m_CsvTable.resize( ( line + 1 ) );
			m_CsvTable[ line ].resize( row + 1 );
		}

		//�C�e���[�^
		std::vector< std::string >::iterator it = m_CsvTable[ line ].begin() + row;

		//�g�[�N�����Z�b�g
		m_CsvTable[ line ].insert( it , 1 , token );

		//std::vector��insert�֐��̓C�e���[�^���w���ꏊ�̒��O�ɒl��}������̂�
		//�}�����ꂽ�Z����1���̃Z�����ɃG���v�e�B�[�g�[�N���������Ă�����폜����
		std::string empty = m_CsvTable[ line ][ row + 1 ];
		if( empty == "" )
		{
			EraseToken( line , row + 1 );
		}
	}

	//�g�[�N���I�[���C���[�T�[
	virtual void EraseToken()
	{
		m_CsvTable.clear();
	}

	//�g�[�N���C���[�T�[
	//����1:�s , ����2:��
	void EraseToken( const int line , const int row )
	{
		//�C�e���[�^
		std::vector< std::string >::iterator it = m_CsvTable[ line ].begin() + row;

		//�폜
		it = m_CsvTable[ line ].erase( it );
	}

	//�G���v�e�B�[�g�[�N���C���[�T�[
	void EraseEmptyToken()
	{
		//�g�[�N���e�[�u�����̖����Z��(���������Ă��Ȃ�"")���폜
		for( int i = 0 ; i < GetNumLine() ; ++i )
		{
			std::vector< std::string >::iterator it;
			it = std::remove( m_CsvTable[ i ].begin() , m_CsvTable[ i ].end() , "" );
			m_CsvTable[ i ].erase( it , m_CsvTable[ i ].end() );
		}
	}
};
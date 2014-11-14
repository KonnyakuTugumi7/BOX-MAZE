#include "DXUT.h"
#include "csv.h"
#include <assert.h>

//CSV�t�@�C���ǂݍ���
void CCsv::ImportFile( const std::string& file_path )
{
	//�t�@�C���p�X
	m_path = file_path;

	//���̓X�g���[��
	std::ifstream fin;

	//���P�[�����` //���{��p�X�ɑΉ�
	setlocale( LC_ALL , "");

	//�ǂݎ�胂�[�h�Ńt�@�C�����J��
	fin.open( file_path , std::ios::in );

	//�J���Ă��Ȃ�������G���[��Ԃ�
	if( !fin.is_open() ) assert( 0 );
	////�o�̓X�g���[��
	//std::ofstream fout;
	//fout.open( file_path , std::ios::trunc );
	//return;

	//���̗�
	std::string new_line;
	char* c_new_line;

	//strtok_s���Ԃ����g�[�N��
	char* tk;

	//strtok_s�������g�p����
	char* ctx;

	//1�s�̃g�[�N������U������Ɋi�[
	std::vector< std::string > line_token;

	//�e�[�u���̒��g����|
	m_csv_table.clear();

	//�J�E���^
	int count = 0;

	//�t�@�C���̏I�[�܂Ń��[�v����
	while( !fin.eof() )
	{
		//�g�[�N������|
		line_token.clear();

		//�i�[���E�𒴂��Ȃ��悤�ɁA��Ustring�Ŏ擾
		getline( fin , new_line );

		//�R�����g
		if( new_line.find( "//" ) != new_line.npos )
		{
			//�R�����g���i�[
			m_comment_map.insert( std::make_pair( count , new_line ) );

			continue;
		}

		//1�񕪂̃g�[�N���T�C�Y���m��
		c_new_line = new char[ new_line.size() + 1 ];

		//�g�[�N�����R�s�[
		strcpy_s( c_new_line , new_line.size() + 1 , new_line.c_str() );
		
		while( 1 )
		{
			//��̏�Ԃ̎�				
			if( line_token.empty() )
			{
				//","�ŕ��������؂�
				tk = strtok_s( c_new_line , "," , &ctx );

				//","��������Ȃ���
				if( tk == NULL )
				{
					//�I�[�܂ŗ�����e�[�u���Ƀg�[�N�����i�[���ă��[�v�𔲂���
					line_token.push_back( c_new_line );
					m_csv_table.push_back( line_token );
					break;
				}
			}
			else
			{
				//","�ŕ��������؂�
				tk = strtok_s( NULL , "," , &ctx );

				//","��������Ȃ���
				if( tk == NULL )
				{
					//�I�[�܂ŗ�����e�[�u���Ƀg�[�N�����i�[���ă��[�v�𔲂���
					m_csv_table.push_back( line_token );
					break;
				}
			}
			//�g�[�N�����i�[
			line_token.push_back( tk );
		}
		//�g���I������̂Ńf���[�g
		delete [] c_new_line;

		//�J�E���^��i�߂�
		++count;
	}
}

//CSV�t�@�C�������o��
void CCsv::ExportFile()
{
	//�o�̓X�g���[��
	std::ofstream ofs;

	//�t�@�C�����J�������ɈȑO�̒��g��S�č폜����
	ofs.open( m_path , std::ios::trunc );

	//�J���Ă��Ȃ�������G���[��Ԃ�
	if( !ofs.is_open() ) assert( 0 );

	//�Ō�̃g�[�N���܂Ń��[�v���񂵂ď����o��
	for( int line = 0 ; line < GetNumLine() ; ++line )
	{
		//�R�����g
		std::map< int , std::string >::iterator it = m_comment_map.find( line );
		if( it != m_comment_map.end() )
		{
			//�R�����g�����o��
			ofs << ( *it ).second;
			ofs << std::endl;
		}

		//�s
		int row = 0;
		while( 1 )
		{
			//�����Z���܂ŗ�������s���ă��[�v�𔲂���
			if( IsValidPos( line , row ) == false )
			{
				//���s
				if( GetNumLine() != line + 1 )
				{
					ofs << std::endl;
				}
				break;
			}

			//�g�[�N�������o��
			ofs << m_csv_table[ line ][ row ];

			//�Z�p���[�^�ł�����
			if( IsValidPos( line , row + 1 ) == true )
			{
				ofs << ",";
			}

			//�s��i�߂�
			++row;
		}
	}
}

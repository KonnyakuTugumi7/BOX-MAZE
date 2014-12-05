#pragma once

#include "DXUT.h"
#include "../../Library/Graphic/graphic_base.h"
#include "../../Library/Support/csv.h"
//#include "../../GameEngine/Graphic/board.h"
#include <memory>
#include <string>
#include <vector>

class CBoard;
//typedef std::shared_ptr< CBoard > BoardSP;

class CCube;
typedef std::shared_ptr< CCube > CubeSP;
typedef std::weak_ptr< CCube > CubeWP;

//�L���[�u�N���X
class CCube : public CGraphicBase{
private:
	//�L���[�u�̎��
	int m_cube_state;

	//�L���[�u�T�C�Y
	D3DXVECTOR3 m_length;

	//�|��
	std::vector< std::shared_ptr< CBoard > > m_board_vec;

	//CSV
	CsvSP m_csv;

public:
	//�|���i���o�[
	enum BOARD_NUM{
		TOP = 0 ,
		LEFT ,
		BACK ,
		RIGHT ,
		FRONT ,
		BOTTOM ,
		BOARD_MAX
	};

public:
	//�R���X�g���N�^
	CCube(){}

	//�f�X�g���N�^
	~CCube()
	{
		//���
		auto it = m_board_vec.begin();
		while( it != m_board_vec.end() )
		{
			( *it ).reset();
			++it;
		}
		m_board_vec.clear();
		m_csv.reset();
	}

	//����
	//����1:�ӂ̒��� , ����2:�t�@�C���p�X(���̈����͗^���Ȃ����ƂŃe�N�X�`���Ȃ��̃J���[�݂̂̃L���[�u�ƂȂ�)
	static CubeSP Create( const D3DXVECTOR3 length , const std::string file_path = "" );

	//������
	//����1:�ӂ̒��� , ����2:�t�@�C���p�X
	void Init( const D3DXVECTOR3 length , const std::string file_path = "" );

	//�`��
	//����1:�J����
	virtual void Render( const CameraBaseWP camera );

	//�|���T�C�Y�Q�b�^�[
	D3DXVECTOR3 GetSize(){ return m_length; }

	//�L���[�u�X�e�[�^�X�Q�b�^�[
	inline int GetCubeState(){ return m_cube_state; }

	//�L���[�u�X�e�[�^�X�Z�b�^�[
	inline void SetCubeState( const int state ){ m_cube_state = state; }

	//�|���Q�b�^�[
	inline std::vector< std::shared_ptr< CBoard > > GetBoard(){ return m_board_vec; }

	//�|���Z�b�^�[(�w�肳�ꂽ�|���̃e�N�X�`����ݒ肷��)
	//����1:�e�N�X�`�� , ����2:�ǂ��̔|����0�`5(���Ԃ�Top , Left , Back , Right , Front , Bottom)
	void SetBoardTex( const TextureSP tex , const int num );
};
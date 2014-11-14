#pragma once

#include "DXUT.h"
#include "../../GameEngine/Graphic/sprite.h"
#include "../../Library/Support/csv.h"
#include <memory>

class CUserInterface;
typedef std::shared_ptr< CUserInterface > UserInterfaceSP;

//UI�N���X
class CUserInterface{
private:
	//CSV
	CsvSP m_csv;

	//���̉��̊�
	SpriteSP m_circle;

	//���
	SpriteSP m_arrow;

	//�T�[�N���_��
	int m_flash;

	//�T�[�N���_�Œl
	float m_flash_min , m_flash_max;

public:
	//�R���X�g���N�^
	CUserInterface(){};

	//�f�X�g���N�^
	~CUserInterface();

	//����
	static UserInterfaceSP Create();

	//������
	void Init();

	//�X�V
	//����1:�S�[���L���[�u�ւ̃x�N�g�� , ����2:�J�����̑O�������ւ̃x�N�g�� , ����3:�o�ߎ���
	void Update( D3DXVECTOR3 goal_vec , D3DXVECTOR3 camera_front_vec , const float elapsed_time );

};
#include "DXUT.h"
#include "user_interface.h"

//�f�X�g���N�^
CUserInterface::~CUserInterface()
{
	//���
	m_csv.reset();
	m_circle.reset();
	m_arrow.reset();
}

//����
UserInterfaceSP CUserInterface::Create()
{
	//�C���X�^���X����
	UserInterfaceSP ui = std::make_shared< CUserInterface >();

	//������
	ui->Init();

	return ui;
}

//������
void CUserInterface::Init()
{
	//CSV
	m_csv = CCsv::Create( "Content/csv/UserInterface.csv" );

	//��
	m_circle = CSprite::Create( "Content/graphics/maru.png" , CGraphicsManager::FRONT_2D );
	m_circle->m_pos = D3DXVECTOR3( m_csv->GetToken< int >( 0 , 0 ) , m_csv->GetToken< int >( 0 , 1 ) , 0 );
	m_circle->m_col.w = m_csv->GetToken< float >( 1 , 0 );
	m_circle->m_scl = D3DXVECTOR3( m_csv->GetToken< float >( 2 , 0 ) , m_csv->GetToken< float >( 2 , 0 ) , 1.0f );

	//���
	m_arrow = CSprite::Create( "Content/graphics/yazirushi.png" , CGraphicsManager::FRONT_2D );
	m_arrow->m_pos = D3DXVECTOR3( m_csv->GetToken< int >( 0 , 0 ) , m_csv->GetToken< int >( 0 , 1 ) , 0 );
	m_arrow->m_col.w = m_csv->GetToken< float >( 3 , 0 );

	//�t�F�[�h�C��
	m_flash = 1;
	m_flash_min = m_csv->GetToken< float >( 4 , 0 );
	m_flash_max = m_csv->GetToken< float >( 5 , 0 );
}

//�X�V
void CUserInterface::Update( D3DXVECTOR3 goal_vec , D3DXVECTOR3 camera_front_vec , const float elapsed_time )
{
	//y�����𖳌���
	goal_vec.y = 0.0f;
	camera_front_vec.y = 0.0f;

	//���K��
	D3DXVec3Normalize( &goal_vec , &goal_vec );
	D3DXVec3Normalize( &camera_front_vec , &camera_front_vec );

	//����(�p�x���Z�o)
	float radian = D3DXVec3Dot( &goal_vec , &camera_front_vec );

	//�O��
	D3DXVECTOR3 temp;
	D3DXVec3Cross( &temp , &goal_vec , &camera_front_vec );

	//�E��������������
	if( temp.y < 0.0f )
	{
		m_arrow->UpdateRotMtx( 0 , 0 , D3DXToDegree( acos( radian ) ) );
	}
	else
	{
		m_arrow->UpdateRotMtx( 0 , 0 , -D3DXToDegree( acos( radian ) ) );
	}

	//�������A���t�@�l�����Ă���
	m_circle->m_col.w += 1.0f / m_flash * elapsed_time;

	//�t�F�[�h�C��
	if( m_flash == 1 )
	{
		//�\�񂳂�Ă���V�[�P���X�Ɉڍs
		if( m_flash_max < m_circle->m_col.w )
		{
			//�O�̂��߃��~�b�g
			m_circle->m_col.w = m_flash_max;

			//�t�F�[�h�A�E�g��
			m_flash = -1;
		}
	}
	//�t�F�[�h�A�E�g
	else if( m_flash == -1 )
	{
		if( m_flash_min > m_circle->m_col.w )
		{	
			//�O�̂��߃��~�b�g
			m_circle->m_col.w = m_flash_min;

			//�t�F�[�h�C����
			m_flash = 1;
		}
	}
}
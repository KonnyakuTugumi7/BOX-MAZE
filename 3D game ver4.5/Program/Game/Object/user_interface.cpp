#include "DXUT.h"
#include "user_interface.h"

//�f�X�g���N�^
CUserInterface::~CUserInterface()
{
	//���
	m_Csv.reset();
	m_Circle.reset();
	m_Arrow.reset();
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
	m_Csv = CCsv::Create( "Content/csv/UserInterface.csv" );

	//��
	m_Circle = CBoard::Create( m_Csv->GetToken< float >( 0 , 0 ) , m_Csv->GetToken< float >( 0 , 1 ) , "Content/graphics/maru.png" , CGraphicsManager::RENDERLIST_STATE::FRONT_2D );
	m_Circle->m_Pos = D3DXVECTOR3( m_Csv->GetToken< float >( 1 , 0 ) , m_Csv->GetToken< float >( 1 , 1 ) , 0 );
	m_Circle->m_Col.w = m_Csv->GetToken< float >( 2 , 0 );

	//���
	m_Arrow = CBoard::Create( m_Csv->GetToken< float >( 3 , 0 ) , m_Csv->GetToken< float >( 3 , 1 ) , "Content/graphics/yazirushi.png" , CGraphicsManager::RENDERLIST_STATE::FRONT_2D );
	m_Arrow->m_Pos = D3DXVECTOR3( m_Csv->GetToken< float >( 4 , 0 ) , m_Csv->GetToken< float >( 4 , 1 ) , 0 );
	m_Arrow->m_Col.w = m_Csv->GetToken< float >( 5 , 0 );

	//�t�F�[�h�C��
	m_Flash = 1;
	m_FlashMin = m_Csv->GetToken< float >( 6 , 0 );
	m_FlashMax = m_Csv->GetToken< float >( 7 , 0 );
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
	D3DXVECTOR3 cross;
	D3DXVec3Cross( &cross , &goal_vec , &camera_front_vec );

	//�E��������������
	if( cross.y > 0.0f )
	{
		m_Arrow->UpdateRotMtx( 0 , 0 , D3DXToDegree( acos( radian ) ) );
	}
	else
	{
		m_Arrow->UpdateRotMtx( 0 , 0 , -D3DXToDegree( acos( radian ) ) );
	}

	//�������A���t�@�l�����Ă���
	m_Circle->m_Col.w += 1.0f / m_Flash * elapsed_time;

	//�t�F�[�h�C��
	if( m_Flash == 1 )
	{
		//�\�񂳂�Ă���V�[�P���X�Ɉڍs
		if( m_FlashMax < m_Circle->m_Col.w )
		{
			//�O�̂��߃��~�b�g
			m_Circle->m_Col.w = m_FlashMax;

			//�t�F�[�h�A�E�g��
			m_Flash = -1;
		}
	}
	//�t�F�[�h�A�E�g
	else if( m_Flash == -1 )
	{
		if( m_FlashMin > m_Circle->m_Col.w )
		{	
			//�O�̂��߃��~�b�g
			m_Circle->m_Col.w = m_FlashMin;

			//�t�F�[�h�C����
			m_Flash = 1;
		}
	}
}
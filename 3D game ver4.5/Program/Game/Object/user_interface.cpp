#include "DXUT.h"
#include "user_interface.h"

//デストラクタ
CUserInterface::~CUserInterface()
{
	//解放
	m_Csv.reset();
	m_Circle.reset();
	m_Arrow.reset();
}

//生成
UserInterfaceSP CUserInterface::Create()
{
	//インスタンス生成
	UserInterfaceSP ui = std::make_shared< CUserInterface >();

	//初期化
	ui->Init();

	return ui;
}

//初期化
void CUserInterface::Init()
{
	//CSV
	m_Csv = CCsv::Create( "Content/csv/UserInterface.csv" );

	//丸
	m_Circle = CBoard::Create( m_Csv->GetToken< float >( 0 , 0 ) , m_Csv->GetToken< float >( 0 , 1 ) , "Content/graphics/maru.png" , CGraphicsManager::RENDERLIST_STATE::FRONT_2D );
	m_Circle->m_Pos = D3DXVECTOR3( m_Csv->GetToken< float >( 1 , 0 ) , m_Csv->GetToken< float >( 1 , 1 ) , 0 );
	m_Circle->m_Col.w = m_Csv->GetToken< float >( 2 , 0 );

	//矢印
	m_Arrow = CBoard::Create( m_Csv->GetToken< float >( 3 , 0 ) , m_Csv->GetToken< float >( 3 , 1 ) , "Content/graphics/yazirushi.png" , CGraphicsManager::RENDERLIST_STATE::FRONT_2D );
	m_Arrow->m_Pos = D3DXVECTOR3( m_Csv->GetToken< float >( 4 , 0 ) , m_Csv->GetToken< float >( 4 , 1 ) , 0 );
	m_Arrow->m_Col.w = m_Csv->GetToken< float >( 5 , 0 );

	//フェードイン
	m_Flash = 1;
	m_FlashMin = m_Csv->GetToken< float >( 6 , 0 );
	m_FlashMax = m_Csv->GetToken< float >( 7 , 0 );
}

//更新
void CUserInterface::Update( D3DXVECTOR3 goal_vec , D3DXVECTOR3 camera_front_vec , const float elapsed_time )
{
	//y成分を無効に
	goal_vec.y = 0.0f;
	camera_front_vec.y = 0.0f;

	//正規化
	D3DXVec3Normalize( &goal_vec , &goal_vec );
	D3DXVec3Normalize( &camera_front_vec , &camera_front_vec );

	//内積(角度を算出)
	float radian = D3DXVec3Dot( &goal_vec , &camera_front_vec );

	//外積
	D3DXVECTOR3 cross;
	D3DXVec3Cross( &cross , &goal_vec , &camera_front_vec );

	//右半分か左半分か
	if( cross.y > 0.0f )
	{
		m_Arrow->UpdateRotMtx( 0 , 0 , D3DXToDegree( acos( radian ) ) );
	}
	else
	{
		m_Arrow->UpdateRotMtx( 0 , 0 , -D3DXToDegree( acos( radian ) ) );
	}

	//少しずつアルファ値を入れていく
	m_Circle->m_Col.w += 1.0f / m_Flash * elapsed_time;

	//フェードイン
	if( m_Flash == 1 )
	{
		//予約されているシーケンスに移行
		if( m_FlashMax < m_Circle->m_Col.w )
		{
			//念のためリミット
			m_Circle->m_Col.w = m_FlashMax;

			//フェードアウトへ
			m_Flash = -1;
		}
	}
	//フェードアウト
	else if( m_Flash == -1 )
	{
		if( m_FlashMin > m_Circle->m_Col.w )
		{	
			//念のためリミット
			m_Circle->m_Col.w = m_FlashMin;

			//フェードインへ
			m_Flash = 1;
		}
	}
}
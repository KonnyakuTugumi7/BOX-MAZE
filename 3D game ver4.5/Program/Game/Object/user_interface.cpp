#include "DXUT.h"
#include "user_interface.h"

//デストラクタ
CUserInterface::~CUserInterface()
{
	//解放
	m_csv.reset();
	m_circle.reset();
	m_arrow.reset();
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
	m_csv = CCsv::Create( "Content/csv/UserInterface.csv" );

	//丸
	m_circle = CSprite::Create( "Content/graphics/maru.png" , CGraphicsManager::FRONT_2D );
	m_circle->m_pos = D3DXVECTOR3( m_csv->GetToken< int >( 0 , 0 ) , m_csv->GetToken< int >( 0 , 1 ) , 0 );
	m_circle->m_col.w = m_csv->GetToken< float >( 1 , 0 );
	m_circle->m_scl = D3DXVECTOR3( m_csv->GetToken< float >( 2 , 0 ) , m_csv->GetToken< float >( 2 , 0 ) , 1.0f );

	//矢印
	m_arrow = CSprite::Create( "Content/graphics/yazirushi.png" , CGraphicsManager::FRONT_2D );
	m_arrow->m_pos = D3DXVECTOR3( m_csv->GetToken< int >( 0 , 0 ) , m_csv->GetToken< int >( 0 , 1 ) , 0 );
	m_arrow->m_col.w = m_csv->GetToken< float >( 3 , 0 );

	//フェードイン
	m_flash = 1;
	m_flash_min = m_csv->GetToken< float >( 4 , 0 );
	m_flash_max = m_csv->GetToken< float >( 5 , 0 );
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
	D3DXVECTOR3 temp;
	D3DXVec3Cross( &temp , &goal_vec , &camera_front_vec );

	//右半分か左半分か
	if( temp.y < 0.0f )
	{
		m_arrow->UpdateRotMtx( 0 , 0 , D3DXToDegree( acos( radian ) ) );
	}
	else
	{
		m_arrow->UpdateRotMtx( 0 , 0 , -D3DXToDegree( acos( radian ) ) );
	}

	//少しずつアルファ値を入れていく
	m_circle->m_col.w += 1.0f / m_flash * elapsed_time;

	//フェードイン
	if( m_flash == 1 )
	{
		//予約されているシーケンスに移行
		if( m_flash_max < m_circle->m_col.w )
		{
			//念のためリミット
			m_circle->m_col.w = m_flash_max;

			//フェードアウトへ
			m_flash = -1;
		}
	}
	//フェードアウト
	else if( m_flash == -1 )
	{
		if( m_flash_min > m_circle->m_col.w )
		{	
			//念のためリミット
			m_circle->m_col.w = m_flash_min;

			//フェードインへ
			m_flash = 1;
		}
	}
}
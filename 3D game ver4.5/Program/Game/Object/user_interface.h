#pragma once

#include "DXUT.h"
#include "../../GameEngine/Graphic/sprite.h"
#include "../../GameEngine/Graphic/board.h"
#include "../../Library/Support/csv.h"
#include <memory>

class CUserInterface;
typedef std::shared_ptr< CUserInterface > UserInterfaceSP;

//UIクラス
class CUserInterface{
private:
	//CSV
	CsvSP m_Csv;

	//矢印の下の丸
	BoardSP m_Circle;

	//矢印
	BoardSP m_Arrow;

	//サークル点滅
	int m_Flash;

	//サークル点滅値
	float m_FlashMin , m_FlashMax;

public:
	//コンストラクタ
	CUserInterface(){};

	//デストラクタ
	~CUserInterface();

	//生成
	static UserInterfaceSP Create();

	//初期化
	void Init();

	//更新
	//引数1:ゴールキューブへのベクトル , 引数2:カメラの前方方向へのベクトル , 引数3:経過時間
	void Update( D3DXVECTOR3 goal_vec , D3DXVECTOR3 camera_front_vec , const float elapsed_time );

};
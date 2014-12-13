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

//キューブクラス
class CCube : public CGraphicBase{
private:
	//キューブの種類
	int m_CubeState;

	//キューブサイズ
	D3DXVECTOR3 m_Length;

	//板ポリ
	std::vector< std::shared_ptr< CBoard > > m_BoardVec;

	//CSV
	CsvSP m_Csv;

public:
	//板ポリナンバー
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
	//コンストラクタ
	CCube(){}

	//デストラクタ
	~CCube()
	{
		//解放
		auto it = m_BoardVec.begin();
		while( it != m_BoardVec.end() )
		{
			( *it ).reset();
			++it;
		}
		m_BoardVec.clear();
		m_Csv.reset();
	}

	//生成
	//引数1:辺の長さ , 引数2:ファイルパス(この引数は与えないことでテクスチャなしのカラーのみのキューブとなる)
	static CubeSP Create( const D3DXVECTOR3 length , const std::string file_path = "" );

	//初期化
	//引数1:辺の長さ , 引数2:ファイルパス
	void Init( const D3DXVECTOR3 length , const std::string file_path = "" );

	//描画
	//引数1:カメラ
	virtual void Render( const CameraBaseWP camera );

	//板ポリサイズゲッター
	D3DXVECTOR3 GetSize(){ return m_Length; }

	//キューブステータスゲッター
	inline int GetCubeState(){ return m_CubeState; }

	//キューブステータスセッター
	inline void SetCubeState( const int state ){ m_CubeState = state; }

	//板ポリゲッター
	inline std::vector< std::shared_ptr< CBoard > > GetBoard(){ return m_BoardVec; }

	//板ポリセッター(指定された板ポリのテクスチャを設定する)
	//引数1:テクスチャ , 引数2:どこの板ポリか0～5(順番にTop , Left , Back , Right , Front , Bottom)
	void SetBoardTex( const TextureSP tex , const int num );
};
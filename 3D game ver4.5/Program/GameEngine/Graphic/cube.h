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
	int m_cube_state;

	//キューブサイズ
	D3DXVECTOR3 m_length;

	//板ポリ
	std::vector< std::shared_ptr< CBoard > > m_board_vec;

	//CSV
	CsvSP m_csv;

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
		auto it = m_board_vec.begin();
		while( it != m_board_vec.end() )
		{
			( *it ).reset();
			++it;
		}
		m_board_vec.clear();
		m_csv.reset();
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
	D3DXVECTOR3 GetSize(){ return m_length; }

	//キューブステータスゲッター
	inline int GetCubeState(){ return m_cube_state; }

	//キューブステータスセッター
	inline void SetCubeState( const int state ){ m_cube_state = state; }

	//板ポリゲッター
	inline std::vector< std::shared_ptr< CBoard > > GetBoard(){ return m_board_vec; }

	//板ポリセッター(指定された板ポリのテクスチャを設定する)
	//引数1:テクスチャ , 引数2:どこの板ポリか0～5(順番にTop , Left , Back , Right , Front , Bottom)
	void SetBoardTex( const TextureSP tex , const int num );
};
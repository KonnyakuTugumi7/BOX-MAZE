#pragma once

#include "DXUT.h"
#include <memory>

#pragma warning ( disable : 4238)	//非標準の拡張機能が使用されています : 右辺値のクラスが左辺値に使用されます

//トランスフォームクラス
class CTransform{
protected:
	//回転タイプ
	enum{
		//XYZ
		ROT_TYPE_XYZ , 
		
		//行列
		ROT_TYPE_MATRIX , 

		//クォータニオン
		ROT_TYPE_QUATERNION
	};

	//回転タイプ
	int m_rot_type;

	//半径 //地球のモデルの半径の適当な値(0.5f)
	float m_rad;
	
	//姿勢
	//現状は VEC3 だが姿勢は
	//ベクトル、行列、クォータニオンのどれでも
	//制御できるようにすべし
	D3DXVECTOR3 m_rot;
		
	//回転行列(x)
	D3DXMATRIX m_rot_x;
	
	//回転行列(y)
	D3DXMATRIX m_rot_y;

	//回転行列(z)
	D3DXMATRIX m_rot_z;

	//クォータニオン
	D3DXQUATERNION m_rot_qtn;

	//ワールドマトリックス(ワールド空間を基準とした時の姿勢)
	D3DXMATRIX m_wld_mtx;

public:
	//座標
	D3DXVECTOR3 m_pos;

	//前のフレームの座標
	D3DXVECTOR3 m_prev_pos;

	//スケール
	D3DXVECTOR3 m_scl;

	//回転行列
	D3DXMATRIX m_rot_mtx;

public:
	//コンストラクタ
	CTransform();

	//デストラクタ
	virtual ~CTransform(){}

	//ワールドマトリックスゲッター
	D3DXMATRIX GetWldMtx(){ return m_wld_mtx; }

	//ワールドマトリックスを設定
	//引数1:親のワールドマトリックス
	void SetWldMtx( D3DXMATRIX* parent_mtx = NULL );

	//引数1:回転量(x) , 引数2:回転量(y) , 引数3:回転量(z)
	void UpdateRotMtx( const float& rot_x , const float& rot_y , const float& rot_z );

	//引数1:回転量
	void UpdateRotMtx( const D3DXVECTOR3& rot );
};
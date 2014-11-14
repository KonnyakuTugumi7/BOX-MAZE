#pragma once

#include "../../Library/Support/csv.h"
#include "../../GameEngine/Camera/camera_base.h"

#include <d3dx9.h>
#include <memory>

class CTPCamera;
typedef std::shared_ptr< CTPCamera > TPCameraSP;

//三人称視点カメラクラス
class CTPCamera : public CCameraBase{
private:
	//経度
    float m_longitudeDegrees;

	//緯度
    float m_latitudeDegrees;

	//回転速度
	float m_rot_speed;

	//カメラと注視点のオフセット(ターゲットからカメラへのベクトル)
    D3DXVECTOR3 m_offset;

public:
	//コンストラクタ
    CTPCamera();

	//デストラクタ
	~CTPCamera(){};

	//生成
	static TPCameraSP Create(){ return std::make_shared< CTPCamera >(); }

	//注視点
	//引数1:注視点
    void LookAt( const D3DXVECTOR3& target );

	//注視点から移動
	//引数1:カメラ座標 , 引数2:注視点 , 引数3:上方向
    void LookAt( const D3DXVECTOR3& eye , const D3DXVECTOR3& target , const D3DXVECTOR3& up );

	//遠近投影
	//引数1:画角(x) , 引数2:アスペクト比 , 引数3:近接限界(Z) , 引数4:遠隔限界(Z)
    void Perspective( const float fovx , const float aspect , const float znear , const float zfar );

	//回転
	//引数1:経度 , 引数2:緯度 , 引数3:hoge
    void Rotate( float longitudeDegrees , float latitudeDegrees , float hoge );

	//カメラ初期化
	void Init(){}

	//カメラ更新
	//引数1:経過時間
    void UpdateCamera( const float elapsedTimeSec );
};
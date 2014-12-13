#pragma once

#include "DXUT.h"
#include "../../GameEngine/Camera/camera_base.h"
#include <memory>

class CViewCamera;
typedef std::shared_ptr< CViewCamera > ViewCameraSP;

//フォントなどを撮影するカメラクラス
class CViewCamera : public CCameraBase{
private:
	//z値
	float m_Distance;

public:
	//コンストラクタ
    CViewCamera();

	//デストラクタ
	~CViewCamera(){};

	//生成
	static ViewCameraSP Create(){ return std::make_shared< CViewCamera >(); }

	//注視点から移動
	//引数1:カメラ座標 , 引数2:注視点 , 引数3:上方向
    void LookAt( const D3DXVECTOR3& eye , const D3DXVECTOR3& target , const D3DXVECTOR3& up );

	//遠近投影
	//引数1:画角 , 引数2:アスペクト比 , 引数3:近接限界(Z) , 引数4:遠隔限界(Z)
    void Perspective( const float fov , const float aspect , const float znear , const float zfar );
};
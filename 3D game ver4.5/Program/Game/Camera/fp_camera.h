#pragma once

#include "../../Library/Support/csv.h"
#include "../../Library/Input/input.h"
#include "../../GameEngine/Camera/camera_base.h"

#include <d3dx9.h>
#include <memory>

#pragma warning ( disable : 4240 ) //非標準の拡張機能が使用されています。

class CFPCamera;
typedef std::shared_ptr< CFPCamera > FPCameraSP;

//一人称視点カメラクラス
class CFPCamera : public CCameraBase{
private:
	//傾き度合い
    float m_AccumPitchDegrees;
    
	//回転速度
    float m_RotationSpeed;

	//加速度
    D3DXVECTOR3 m_Acceleration;

	//今の速度
    D3DXVECTOR3 m_CurrentVelocity;

	//速度
    D3DXVECTOR3 m_Velocity;

	//最大限界
	D3DXVECTOR3 m_BoundsMax;

	//最小限界
	D3DXVECTOR3 m_BoundsMin;

private:
	//移動方向
	//引数1:方向
	void GetMovementDirection( D3DXVECTOR3& direction );

	//一人称回転
	//引数1:y軸で回転する方向 , 引数2:x軸で回転する方向
    void RotateFirstPerson( float headingDegrees , float pitchDegrees );

	//速度更新
	//引数1:移動方向 , 引数2:経過時間
    void UpdateVelocity( const D3DXVECTOR3& direction , const float elapsedTimeSec );

	//ビュー行列更新
    void UpdateViewMatrix();

	//座標更新
	//引数1:方向 , 引数2:経過時間
	void UpdatePosition( const D3DXVECTOR3& direction , const float elapsedTimeSec );

	//カメラ移動
	//引数1:移動方向(x) , 引数2:移動方向(y) , 引数3:移動方向(z)
    void Move( const float dx , const float dy , const float dz );

	//カメラ移動
	//引数1:移動方向 , 引数2:移動量
    void Move( const D3DXVECTOR3& direction , const D3DXVECTOR3& amount );

	//挙動の制限
	void PerformCameraCollisionDetection();

	//ジャンプ
	//引数1:経過時間
	void Jump( const float elapsedTimeSec );

	//遠近投影
	//引数1:画角(x) , 引数2:アスペクト比 , 引数3:近接限界(Z) , 引数4:遠隔限界(Z)
   void Perspective( const float fovx , const float aspect , const float znear , const float zfar );

	//回転
	//引数1:y軸で回転する方向 , 引数2:x軸で回転する方向 , 引数3:z軸で回転する方向
    void Rotate( float headingDegrees , float pitchDegrees , float rollDegrees );

public:
	//コンストラクタ
    CFPCamera();

	//デストラクタ
	~CFPCamera(){};

	//生成
	static FPCameraSP Create(){ return std::make_shared< CFPCamera >(); }

	//初期化
	void Init();
    
	//カメラ更新
	//引数1:経過時間
	void UpdateCamera( const float elapsedTimeSec );

    //----------------------------------ゲッター----------------------------------

    inline const D3DXVECTOR3 &GetAcceleration() const { return m_Acceleration; }
    inline const D3DXVECTOR3 &GetCurrentVelocity() const { return m_CurrentVelocity; }
    inline const D3DXVECTOR3 &GetVelocity() const { return m_Velocity; }
	inline const float GetRotationSpeed() const { return m_RotationSpeed; }
    
    //----------------------------------セッター----------------------------------

	inline void SetPosition( const D3DXVECTOR3& eye )
	{
		m_Eye = eye;

		//ビュー行列を更新
		UpdateViewMatrix();
	}
	inline void SetPosition( const float x , const float y , const float z )
	{
		m_Eye.x = x;
		m_Eye.y = y;
		m_Eye.z = z;

		//ビュー行列を更新
		UpdateViewMatrix();
	}
	inline void SetOrientation( const D3DXQUATERNION& orientation )
	{
		D3DXMATRIX m;

		//クォータニオンから回転行列を作成
		D3DXMatrixRotationQuaternion( &m , &orientation );

		//傾き度合い
		m_AccumPitchDegrees = D3DXToDegree( asinf( m( 1 , 2 ) ) );

		//新しい方位をセット
		m_Orientation = orientation;

		LookAt( m_Eye , m_Eye + m_ViewDir , WORLD_YAXIS );

		//ビュー行列を更新
		UpdateViewMatrix();
	}
};

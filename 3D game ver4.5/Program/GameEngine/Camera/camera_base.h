#pragma once

#include "../../Library/Support/csv.h"

#include <d3dx9.h>
#include <memory>

#pragma warning ( disable : 4240 ) //非標準の拡張機能が使用されています。
#pragma warning ( disable : 4238 ) //非標準の拡張機能が使用されています。: 右辺値のクラスが左辺値に使用されます。

////視錐台クラス
//class CViewFrustum{
//public:
//	//視錐台の各面
//	D3DXPLANE m_left;
//	D3DXPLANE m_right;
//	D3DXPLANE m_top;
//	D3DXPLANE m_bottom;
//	D3DXPLANE m_near;
//	D3DXPLANE m_far;
//
//public:
//	//コンストラクタ
//	CViewFrustum(){}
//
//	//デストラクタ
//	~CViewFrustum(){}
//
//	//コピーコンストラクタ
//	CViewFrustum( const CViewFrustum& frustum )
//	{
//		m_left = frustum.m_left;
//		m_right = frustum.m_right;
//		m_top = frustum.m_top;
//		m_bottom = frustum.m_bottom;
//		m_near = frustum.m_near;
//		m_far = frustum.m_far;
//	}
//
//	// = 演算子のオーバーロード
//	inline CViewFrustum& operator = (const CViewFrustum& frustum)
//	{
//		m_left = frustum.m_left;
//		m_right = frustum.m_right;
//		m_top = frustum.m_top;
//		m_bottom = frustum.m_bottom;
//		m_near = frustum.m_near;
//		m_far = frustum.m_far;
//		return ( *this );
//	}
//};

class CCameraBase;
typedef std::shared_ptr< CCameraBase > CameraBaseSP;
typedef std::weak_ptr< CCameraBase > CameraBaseWP;

//カメラクラス
class CCameraBase{
protected:
	//-----------------静的メンバ変数-----------------
	//ワールドY軸
    static const D3DXVECTOR3 WORLD_YAXIS;

public:
	//座標
    D3DXVECTOR3 m_Eye;

	//前フレームの座標
	D3DXVECTOR3 m_PrevEye;

protected:
	//重力をかけるかどうか
	bool m_IsGravity;

	//地面(物体の上面)接地フラグ
	bool m_IsOnGround;

	//Y軸移動量
	float m_JumpMovement;

	//AABBの各軸の長さ(当たり判定)
	D3DXVECTOR3 m_Length;

	//注視点(look)
    D3DXVECTOR3 m_Target;
    
	//X軸
    D3DXVECTOR3 m_XAxis;

	//Y軸
    D3DXVECTOR3 m_YAxis;

	//Z軸
    D3DXVECTOR3 m_ZAxis;

	//視野方向
    D3DXVECTOR3 m_ViewDir;

	//方位
    D3DXQUATERNION m_Orientation;
	
	//ビュー行列
    D3DXMATRIX m_ViewMatrix;

	//プロジェクション(投影)行列
    D3DXMATRIX m_ProjMatrix;

	//CSV
	CsvSP m_Csv;

public:
	//コンストラクタ
    CCameraBase();

	//デストラクタ
	virtual ~CCameraBase(){ m_Csv.reset(); };

	//注視点移動
	//引数1:注視点
	virtual void LookAt( const D3DXVECTOR3& target ){};

	//注視点移動
	//引数1:カメラ座標 , 引数2:注視点 , 引数3:上方向
	virtual void LookAt( const D3DXVECTOR3& eye , const D3DXVECTOR3& target , const D3DXVECTOR3& up ){};

	//遠近投影
	//引数1:画角(x) , 引数2:アスペクト比 , 引数3:近接限界(Z) , 引数4:遠隔限界(Z)
	virtual void Perspective( const float fovx , const float aspect , const float znear , const float zfar ){};

	//回転
	//引数1:y軸で回転する方向 , 引数2:x軸で回転する方向 , 引数3:z軸で回転する方向
	virtual void Rotate( float headingDegrees , float pitchDegrees , float rollDegrees ){};
    
	//カメラ更新
	//引数1:経過時間
	virtual void UpdateCamera( const float elapsedTimeSec ){};

	//カメラ初期化
	virtual void Init(){};

    //----------------------------------ゲッター----------------------------------

    inline const D3DXVECTOR3 &GetPosition() const { return m_Eye; }
	inline const D3DXVECTOR3 GetSize() const { return m_Length; }
	inline const D3DXVECTOR3 GetViewDir() const { return m_ViewDir; }
	inline const D3DXVECTOR3 GetViewTarget() const { return m_Target; }
	inline const D3DXMATRIX &GetViewMatrix() const { return m_ViewMatrix; }
	inline const D3DXMATRIX &GetProjectionMatrix() const { return m_ProjMatrix; }
	inline const float GetJumpMovement() const { return m_JumpMovement; }
	//CViewFrustum GetViewFrustum();

	//----------------------------------セッター----------------------------------

	inline void SetIsGravity( const bool flg ){ m_IsGravity = flg; }
	inline void SetIsOnGround( const bool flg ){ m_IsOnGround = flg; }
	inline void SetJumpMovement( const float jump_movement ){ m_JumpMovement = jump_movement; }
};

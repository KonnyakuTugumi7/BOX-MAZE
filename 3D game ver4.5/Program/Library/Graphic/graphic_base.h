#pragma once 

#include "../../Library/Graphic/transform.h"
#include "../../Library/Graphic/material.h"
#include "../../Library/Graphic/mesh.h"
#include "../../Library/Graphic/texture.h"
#include "../../GameEngine/Camera/camera_base.h"
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <memory>

#pragma warning ( disable : 4018 ) //signed と unsigned の数値を比較しようとしました。
#pragma warning ( disable : 4238 ) //非標準の拡張機能が使用されています : 右辺値のクラスが左辺値に使用されます。

class CGraphicBase;
typedef std::shared_ptr< CGraphicBase > GraphicBaseSP;
typedef std::weak_ptr< CGraphicBase > GraphicBaseWP;

//グラフィックベースクラス
class CGraphicBase : public CTransform , public CMaterial{
public:
//フレンドクラス
friend class CGraphicsManager;

protected:
	//描画フラグ
	bool m_IsRender;

	//当たり判定フラグ(当たり判定を行うかどうか)
	bool m_IsCollision;

	//ソートの基準となる値(カメラからの距離)
	float m_CameraDistance;

	//カリングのステータス
	D3DCULL m_CullState;

	//メッシュ
	MeshSP m_Mesh;

	//テクスチャ
	std::vector< TextureSP > m_Texs;

public:
	//描画に使うシェーダの種類
	enum SHADER_STATE{
		//基本シェーダ
		SDR_BASE = 0 , 

		//平行光源シェーダ
		SDR_DIR_LIGHT = 1
	} m_sdr_state;

public:
	//コンストラクタ
	CGraphicBase() : m_IsRender( true ) ,
					 m_IsCollision( true ) , 
					 m_CameraDistance( 1.0f ) ,
					 m_CullState( D3DCULL_CCW ) , 
					 m_sdr_state( SDR_DIR_LIGHT ) ,
					 m_Mesh( NULL )
	{}

	//デストラクタ
	virtual ~CGraphicBase(){ m_Texs.clear(); m_Mesh.reset(); }

	//描画
	//引数1:カメラ
	virtual void Render( const CameraBaseWP camera );

	//ロジック
	virtual void Move(){}

	//シェーダ設定
	void SetSDR();

	// std::list のソートで使用する(3D)
	static bool comp3D( const GraphicBaseSP riLeft , const GraphicBaseSP riRight )
	{
		return ( riLeft->m_CameraDistance > riRight->m_CameraDistance );
	}

	// std::list のソートで使用する(3D)
	static bool comp3DAlpha( const GraphicBaseSP riLeft , const GraphicBaseSP riRight )
	{
		return ( riLeft->m_CameraDistance < riRight->m_CameraDistance );
	}

	// std::list のソートで使用する(2D)
	static bool comp2D(const GraphicBaseSP riLeft, const GraphicBaseSP riRight)
	{
		return ( riLeft->m_CameraDistance > riRight->m_CameraDistance );
    }

	//メッシュゲッター
	inline MeshSP GetMeshSP(){ return m_Mesh; }

	//描画フラグゲッター
	inline bool GetIsRender(){ return m_IsRender; }

	//描画フラグセッター
	inline void SetIsRender( const bool render ){ m_IsRender = render; }

	//当たり判定フラグゲッター
	inline bool GetIsCollision(){ return m_IsCollision; }

	//当たり判定フラグセッター
	inline void SetIsCollision( const bool flg ){ m_IsCollision = flg; }

	//カメラからの距離セッター
	inline void SetCameraDistance( const float distance ){ m_CameraDistance = distance; }
};
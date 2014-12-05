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
	bool m_is_render;

	//当たり判定フラグ(当たり判定を行うかどうか)
	bool m_is_collision;

	//ソートの基準となる値(カメラからの距離)
	float m_camera_distance;

	//カリングのステータス
	D3DCULL m_cull_state;

	//メッシュ
	MeshSP m_mesh;

	//テクスチャ
	std::vector< TextureSP > m_texs;

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
	CGraphicBase() : m_is_render( true ) ,
					 m_is_collision( true ) , 
					 m_camera_distance( 1.0f ) ,
					 m_cull_state( D3DCULL_CCW ) , 
					 m_sdr_state( SDR_DIR_LIGHT ) ,
					 m_mesh( NULL )
	{}

	//デストラクタ
	virtual ~CGraphicBase(){ m_texs.clear(); m_mesh.reset(); }

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
		return ( riLeft->m_camera_distance > riRight->m_camera_distance );
	}

	// std::list のソートで使用する(3D)
	static bool comp3DAlpha( const GraphicBaseSP riLeft , const GraphicBaseSP riRight )
	{
		return ( riLeft->m_camera_distance < riRight->m_camera_distance );
	}

	// std::list のソートで使用する(2D)
	static bool comp2D(const GraphicBaseSP riLeft, const GraphicBaseSP riRight)
	{
		return ( riLeft->m_camera_distance > riRight->m_camera_distance );
    }

	//メッシュゲッター
	inline MeshSP GetMeshSP(){ return m_mesh; }

	//描画フラグゲッター
	inline bool GetIsRender(){ return m_is_render; }

	//描画フラグセッター
	inline void SetIsRender( const bool render ){ m_is_render = render; }

	//当たり判定フラグゲッター
	inline bool GetIsCollision(){ return m_is_collision; }

	//当たり判定フラグセッター
	inline void SetIsCollision( const bool flg ){ m_is_collision = flg; }

	//カメラからの距離セッター
	inline void SetCameraDistance( const float distance ){ m_camera_distance = distance; }
};
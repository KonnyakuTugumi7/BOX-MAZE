#pragma once

#pragma warning( disable : 4715 ) //値を返さないコントロール パスがあります。
#pragma warning( disable : 4238 ) //非標準の拡張機能が使用されています : 右辺値のクラスが左辺値に使用されます。
#pragma warning( disable : 4482 ) //非標準の拡張機能が使用されています: enum 'CCollisionManager::HIT_DIRECTION' が限定名で使用されます。
#pragma warning( disable : 4239 ) //非標準の拡張機能が使用されています : 'default argument' : 'D3DXVECTOR3' から 'D3DXVECTOR3 &' への変換です。
#pragma warning( disable : 4244 ) //'引数' : 'const int' から 'FLOAT' への変換です。データが失われる可能性があります。

//クラスの前方宣言
//視錐台クラス
class CViewFrustum;

//当たり判定クラス
class CCollisionManager{
public:
	//AABBの
	static enum HIT_DIRECTION
	{
		NONE = 0,
		LEFT = 1,
		RIGHT = 2,
		FRONT = 3,
		BACK = 4,
		TOP = 5,
		BOTTOM = 6,
		EXCEPTION = 7
	};

private:
	//コンストラクタ
	CCollisionManager(){}

	//デストラクタ
	~CCollisionManager(){}

public:
	//AABBとAABBの当たり判定と当たった面を検出し補正する
	//引数1:Aの中心 , 引数2:Aの前フレームの中心 , 引数3:Aの各辺の長さ , 引数4:Bの中心 , 引数5:Bの前フレームの中心 , 引数6:Bの各辺の長さ , 引数7:補正する座標
	//戻り値:当たった面
	static HIT_DIRECTION CollisionAABBAABB( const D3DXVECTOR3 posA , const D3DXVECTOR3 prev_posA , const D3DXVECTOR3 lengthA , const D3DXVECTOR3 posB , const D3DXVECTOR3 prev_posB , const D3DXVECTOR3 lengthB , D3DXVECTOR3& pos = D3DXVECTOR3( 0 , 0 , 0 ) );

	//AABBと光線との交差判定
	//引数1:光線の始点 , 引数2:光線の方向 , 引数3:AABBの座標 , 引数4:AABBの各辺の長さ , 引数5:AABBのワールド変換行列 , 引数6:衝突間隔 , 引数7:衝突位置
	static bool CollisionRayAABB( const D3DXVECTOR3& pos , const D3DXVECTOR3& dir , const D3DXVECTOR3 posAABB , const D3DXVECTOR3 lengthAABB , const D3DXMATRIX& mat , float& t , D3DXVECTOR3& colPos = D3DXVECTOR3( 0 , 0 , 0 ) );

	//AABBと点との交差判定
	//引数1:AABBの座標 , 引数2:AABBの各辺の長さ , 引数3:点の座標
	static bool CollisionPointAABB( const D3DXVECTOR3& pos , const D3DXVECTOR3& length , const D3DXVECTOR3& point );

	//スクリーン座標の指定でレイを作成する
	//引数1:レイ上の座標(計算結果を格納) , 引数2:レイの方向(計算結果を格納する) 引数3:スクリーンX座標 , 引数4:スクリーンY座標 , 引数5:カメラのビュー行列 , 引数6:カメラのプロジェクション行列
	static void ScreenToRay( D3DXVECTOR3& pos , D3DXVECTOR3& dir , const int sx , const int sy , const D3DXMATRIX& view , const D3DXMATRIX& prj );

	//2点間の距離を算出
	//引数1:始点 , 引数2:終点
	static float CalcDistance( const D3DXVECTOR3& v_s , const D3DXVECTOR3& v_e );
};
#include "DXUT.h"
#include "transform.h"

//コンストラクタ
CTransform::CTransform() : 
	m_RotType( ROT_TYPE_MATRIX ) ,
	m_Pos( 0 , 0 , 0 ) ,
	m_PrevPos( 0 , 0 , 0 ) ,
	m_Scl( 1 , 1 , 1 ) ,
	m_Rot( 0 , 0 , 0 ) ,
	m_Rad( 0.5f )
{
	//行列の初期化
	D3DXMatrixIdentity( &m_RotMtx );
	D3DXMatrixIdentity( &m_RotX );
	D3DXMatrixIdentity( &m_RotY );
	D3DXMatrixIdentity( &m_RotZ );
	D3DXMatrixIdentity( &m_WldMtx );

	//クォータニオンの初期化
	D3DXQuaternionIdentity( &m_RotQtn );
}

//ワールドマトリックスを設定
void CTransform::SetWldMtx( D3DXMATRIX* parent_mtx )
{
	//座標を保存
	m_PrevPos = m_Pos;

	//-----------------------------ワールドマトリクス設定(ローカル座標からワールド座標へ)-------------------------------------

	D3DXMATRIX SclMtx , RotMtx , PosMtx;

	//--------------------------回転(※y,x,zの順番)--------------------------
	if( CTransform::ROT_TYPE_XYZ == m_RotType )
	{
		D3DXMatrixRotationYawPitchRoll( &RotMtx, m_Rot.y, m_Rot.x, m_Rot.z );
	}
	else if( CTransform::ROT_TYPE_MATRIX == m_RotType )
	{
		m_RotMtx = m_RotX * m_RotY * m_RotZ; 
		RotMtx = m_RotMtx;
	}
	else if( CTransform::ROT_TYPE_QUATERNION == m_RotType )
	{
		//クォータニオンからマトリクスを生成する
		D3DXMatrixRotationQuaternion( &RotMtx, &m_RotQtn );
	}

	//---------------------------拡大---------------------------
	D3DXMatrixScaling( &SclMtx , m_Scl.x , m_Scl.y , m_Scl.z );

	//--------------------------平行移動--------------------------
	D3DXMatrixTranslation( &PosMtx, m_Pos.x, m_Pos.y, m_Pos.z );

	//親のワールドマトリックスがNULLじゃないなら
	//親がいるので親の姿勢を考慮した自分の姿勢を設定
	if( parent_mtx )
	{
		m_WldMtx = SclMtx * RotMtx * PosMtx * ( *parent_mtx );
	}
	else
	{
		m_WldMtx =  SclMtx * RotMtx * PosMtx;
	}
}

//回転行列更新
void CTransform::UpdateRotMtx( const float& rot_x , const float& rot_y , const float& rot_z )
{
	//---------------各成分から各回転行列を計算---------------
	D3DXMatrixRotationX( &m_RotX , D3DXToRadian( rot_x ) );
	D3DXMatrixRotationY( &m_RotY , D3DXToRadian( rot_y ) );
	D3DXMatrixRotationZ( &m_RotZ , D3DXToRadian( rot_z ) );

	m_RotMtx = m_RotX * m_RotY * m_RotZ;
}

//回転行列更新
void CTransform::UpdateRotMtx( const D3DXVECTOR3& rot )
{
	//---------------各成分から各回転行列を計算---------------
	D3DXMatrixRotationX( &m_RotX, D3DXToRadian( rot.x ) );
	D3DXMatrixRotationY( &m_RotY, D3DXToRadian( rot.y ) );
	D3DXMatrixRotationZ( &m_RotZ, D3DXToRadian( rot.z ) );

	m_RotMtx = m_RotX * m_RotY * m_RotZ;
}
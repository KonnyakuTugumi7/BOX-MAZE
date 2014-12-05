#include "DXUT.h"
#include "transform.h"

//コンストラクタ
CTransform::CTransform() : 
	m_rot_type( ROT_TYPE_MATRIX ) ,
	m_pos( 0 , 0 , 0 ) ,
	m_prev_pos( 0 , 0 , 0 ) ,
	m_scl( 1 , 1 , 1 ) ,
	m_rot( 0 , 0 , 0 ) ,
	m_rad( 0.5f )
{
	//行列の初期化
	D3DXMatrixIdentity( &m_rot_mtx );
	D3DXMatrixIdentity( &m_rot_x );
	D3DXMatrixIdentity( &m_rot_y );
	D3DXMatrixIdentity( &m_rot_z );
	D3DXMatrixIdentity( &m_wld_mtx );

	//クォータニオンの初期化
	D3DXQuaternionIdentity( &m_rot_qtn );
}

//ワールドマトリックスを設定
void CTransform::SetWldMtx( D3DXMATRIX* parent_mtx )
{
	//座標を保存
	m_prev_pos = m_pos;

	//-----------------------------ワールドマトリクス設定(ローカル座標からワールド座標へ)-------------------------------------

	D3DXMATRIX SclMtx , RotMtx , PosMtx;

	//--------------------------回転(※y,x,zの順番)--------------------------
	if( CTransform::ROT_TYPE_XYZ == m_rot_type )
	{
		D3DXMatrixRotationYawPitchRoll( &RotMtx, m_rot.y, m_rot.x, m_rot.z );
	}
	else if( CTransform::ROT_TYPE_MATRIX == m_rot_type )
	{
		m_rot_mtx = m_rot_x * m_rot_y * m_rot_z; 
		RotMtx = m_rot_mtx;
	}
	else if( CTransform::ROT_TYPE_QUATERNION == m_rot_type )
	{
		//クォータニオンからマトリクスを生成する
		D3DXMatrixRotationQuaternion( &RotMtx, &m_rot_qtn );
	}

	//---------------------------拡大---------------------------
	D3DXMatrixScaling( &SclMtx , m_scl.x , m_scl.y , m_scl.z );

	//--------------------------平行移動--------------------------
	D3DXMatrixTranslation( &PosMtx, m_pos.x, m_pos.y, m_pos.z );

	//親のワールドマトリックスがNULLじゃないなら
	//親がいるので親の姿勢を考慮した自分の姿勢を設定
	if( parent_mtx )
	{
		m_wld_mtx = SclMtx * RotMtx * PosMtx * ( *parent_mtx );
	}
	else
	{
		m_wld_mtx =  SclMtx * RotMtx * PosMtx;
	}
}

//回転行列更新
void CTransform::UpdateRotMtx( const float& rot_x , const float& rot_y , const float& rot_z )
{
	//---------------各成分から各回転行列を計算---------------
	D3DXMatrixRotationX( &m_rot_x , D3DXToRadian( rot_x ) );
	D3DXMatrixRotationY( &m_rot_y , D3DXToRadian( rot_y ) );
	D3DXMatrixRotationZ( &m_rot_z , D3DXToRadian( rot_z ) );

	m_rot_mtx = m_rot_x * m_rot_y * m_rot_z;
}

//回転行列更新
void CTransform::UpdateRotMtx( const D3DXVECTOR3& rot )
{
	//---------------各成分から各回転行列を計算---------------
	D3DXMatrixRotationX( &m_rot_x, D3DXToRadian( rot.x ) );
	D3DXMatrixRotationY( &m_rot_y, D3DXToRadian( rot.y ) );
	D3DXMatrixRotationZ( &m_rot_z, D3DXToRadian( rot.z ) );

	m_rot_mtx = m_rot_x * m_rot_y * m_rot_z;
}
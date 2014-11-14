#include "DXUT.h"
#include "collision_manager.h"
#include "../../GameEngine/Camera/camera_base.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Support/common.h"
#include <math.h>

//AABBとAABBの当たり判定と当たった面を検出し補正する
CCollisionManager::HIT_DIRECTION CCollisionManager::CollisionAABBAABB( const D3DXVECTOR3 posA , const D3DXVECTOR3 prev_posA , const D3DXVECTOR3 lengthA , const D3DXVECTOR3 posB , const D3DXVECTOR3 prev_posB , const D3DXVECTOR3 lengthB , D3DXVECTOR3& pos )
{
	//当たっていない
	//X
	if( abs(posB.x - posA.x ) > ( lengthB.x + lengthA.x ) * 0.5f ) return HIT_DIRECTION::NONE;
	//Y
	if( abs(posB.y - posA.y ) > ( lengthB.y + lengthA.y ) * 0.5f ) return HIT_DIRECTION::NONE;
	//Z
	if( abs(posB.z - posA.z ) > ( lengthB.z + lengthA.z ) * 0.5f ) return HIT_DIRECTION::NONE;

	//AとBの最小最大座標を算出
	D3DXVECTOR3 min_posA , max_posA , min_posB , max_posB;
	min_posA = posA - lengthA * 0.5f;
	max_posA = posA + lengthA * 0.5f;
	min_posB = posB - lengthB * 0.5f;
	max_posB = posB + lengthB * 0.5f;
	//AとBの前フレームの最小最大座標を算出
	D3DXVECTOR3 prev_min_posA , prev_max_posA , prev_min_posB , prev_max_posB;
	prev_min_posA = prev_posA - lengthA * 0.5f;
	prev_max_posA = prev_posA + lengthA * 0.5f;
	prev_min_posB = prev_posB - lengthB * 0.5f;
	prev_max_posB = prev_posB + lengthB * 0.5f;

	//補正と当たった面を算出
	//Y
	if( prev_min_posB.y >= max_posA.y )
	{
		pos.y = max_posA.y + ( lengthB.y * 0.5f );
		return HIT_DIRECTION::TOP;
	}
	if( prev_max_posB.y <= min_posA.y )
	{
		pos.y = min_posA.y - ( lengthB.y * 0.5f );
		return HIT_DIRECTION::BOTTOM;
	}

	//X
	if( prev_min_posB.x >= max_posA.x )
	{
		pos.x = max_posA.x + ( lengthB.x * 0.5f );
		return HIT_DIRECTION::RIGHT;
	}
	if( prev_max_posB.x <= min_posA.x )
	{
		pos.x = min_posA.x - ( lengthB.x * 0.5f );
		return HIT_DIRECTION::LEFT;
	}

	//Z
	if( prev_min_posB.z >= max_posA.z )
	{
		pos.z = max_posA.z + ( lengthB.z * 0.5f );
		return HIT_DIRECTION::FRONT;
	}
	if( prev_max_posB.z <= min_posA.z )
	{
		pos.z = min_posA.z - ( lengthB.z * 0.5f );
		return HIT_DIRECTION::BACK;
	}

	//例外
	return HIT_DIRECTION::EXCEPTION;
}

//AABBと光線との交差判定
bool CCollisionManager::CollisionRayAABB( const D3DXVECTOR3& pos , const D3DXVECTOR3& dir , const D3DXVECTOR3 posAABB , const D3DXVECTOR3 lengthAABB , const D3DXMATRIX& mat , float& t , D3DXVECTOR3& colPos )
{
	//光線を境界ボックスの空間へ移動
	D3DXMATRIX invMat;
	D3DXMatrixInverse( &invMat , 0 , &mat );

	D3DXVECTOR3 p_l, dir_l;
	D3DXVec3TransformCoord( &p_l , &pos , &invMat );
	invMat._41 = 0.0f;
	invMat._42 = 0.0f;
	invMat._43 = 0.0f;
	D3DXVec3TransformCoord( &dir_l , &dir , &invMat );

	//AとBの最小最大座標を算出
	D3DXVECTOR3 min_posAABB , max_posAABB;
	min_posAABB = posAABB - lengthAABB * 0.5f;
	max_posAABB = posAABB + lengthAABB * 0.5f;

	//交差判定
	float p[ 3 ] , d[ 3 ] , min[ 3 ] , max[ 3 ];
	//memcpy( p , &p_l , sizeof( D3DXVECTOR3 ) );
	//memcpy( d , &dir_l , sizeof( D3DXVECTOR3 ) );
	memcpy( p , &pos , sizeof( D3DXVECTOR3 ) );
	memcpy( d , &dir , sizeof( D3DXVECTOR3 ) );
	memcpy( min , &min_posAABB , sizeof( D3DXVECTOR3 ) );
	memcpy( max , &max_posAABB , sizeof( D3DXVECTOR3 ) );

	t = -FLT_MAX;
	float t_max = FLT_MAX;

	for( int i = 0 ; i < 3 ; ++i )
	{
		if( abs( d[ i ] ) < FLT_EPSILON )
		{
			//交差していない
			if( p[ i ] < min[ i ] || p[ i ] > max[ i ] ) return false;
		}
		else
		{
			// スラブとの距離を算出
			// t1が近スラブ、t2が遠スラブとの距離
			float odd = 1.0f / d[ i ];
			float t1 = ( min[ i ] - p[ i ] ) * odd;
			float t2 = ( max[ i ] - p[ i ] ) * odd;
			if( t1 > t2 )
			{
				float tmp = t1; t1 = t2; t2 = tmp;
			}

			if ( t1 > t ) t = t1;
			if ( t2 < t_max ) t_max = t2;

			//スラブ交差チェック
			if( t >= t_max ) return false;
		}
	}

	//交差している
	if( colPos )
	{
		colPos = pos + t * dir;
	}
	return true;
}

//AABBと点との交差判定
bool CCollisionManager::CollisionPointAABB( const D3DXVECTOR3& pos , const D3DXVECTOR3& length , const D3DXVECTOR3& point )
{
	//AとBの最小最大座標を算出
	D3DXVECTOR3 min_posAABB , max_posAABB;
	min_posAABB = pos - length * 0.5f;
	max_posAABB = pos + length * 0.5f;

	if( min_posAABB.x > point.x || max_posAABB.x < point.x ) return false;
	if( min_posAABB.y > point.y || max_posAABB.y < point.y ) return false;
	if( min_posAABB.z > point.z || max_posAABB.z < point.z ) return false;

	return true;
}

//スクリーン座標の指定でレイを作成する
void CCollisionManager::ScreenToRay( D3DXVECTOR3& pos , D3DXVECTOR3& dir , const int sx , const int sy , const D3DXMATRIX& view , const D3DXMATRIX& prj )
{
	//ビューポート行列を作成
	D3DXMATRIX vpMat;
	D3DXMatrixIdentity( &vpMat );

	D3DVIEWPORT9 vp;
	CGraphicsManager::m_pd3dDevice->GetViewport( &vp );

	vpMat._11 = ( float )vp.Width * 0.5f;
	vpMat._22 = -1.0f * ( float )( vp.Height * 0.5f );
	vpMat._33 = ( float )vp.MaxZ - vp.MinZ;
	vpMat._41 = ( float )( vp.X + vp.Width * 0.5f );
	vpMat._42 = ( float )( vp.Y + vp.Height * 0.5f );
	vpMat._43 = vp.MinZ;

	// スクリーン位置をワールドへ
	D3DXVECTOR3 vz0, vz1;
	vz0 = D3DXVECTOR3( sx , sy , 0.0f );
	vz1 = D3DXVECTOR3( sx , sy , 1.0f );

	D3DXMATRIX invMat , inv_prj , inv_view;
	D3DXMatrixInverse( &invMat , 0 , &vpMat );
	D3DXMatrixInverse( &inv_prj , 0 , &prj );
	D3DXMatrixInverse( &inv_view , 0 , &view );
	invMat *= inv_prj * inv_view;

	D3DXVec3TransformCoord( &vz0 , &vz0 , &invMat );
	D3DXVec3TransformCoord( &vz1 , &vz1 , &invMat );

	pos = vz0;
	D3DXVec3Normalize( &dir , &( vz1 - vz0 ) );
}

//2点間の距離を算出
float CCollisionManager::CalcDistance( const D3DXVECTOR3& v_s , const D3DXVECTOR3& v_e )
{
	return sqrt( ( ( v_e.x - v_s.x ) * ( v_e.x - v_s.x ) ) + ( ( v_e.y - v_s.y ) * ( v_e.y - v_s.y ) ) + ( ( v_e.z - v_s.z ) * ( v_e.z - v_s.z ) ) );
}
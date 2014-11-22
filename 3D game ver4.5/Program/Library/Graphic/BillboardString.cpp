// 2008. 11. 16 v1.00 -> v1.01
// ・同じ文字列を毎フレーム登録するとフルUpdateが発生してしまうバグを修正
//

#include "DXUT.h"
#include "BillboardString.h"
#include <crtdbg.h>
#include <string.h>
#include <tchar.h>

//コンストラクタ
CBillboardString::CBillboardString( IDirect3DDevice9 *dev ) : m_CurLen( 0 ),
															m_IsUpdatePosition( true ),
															m_IsUpdate( true ),
															m_Color( 0x00ffffff ),
															m_Alpha( 0xff ),
															m_BasePos( 0.0f , 0.0f ),
															m_Scale( 1.0f , 1.0f ),
															m_PixelSize( 32 )
{
	_ASSERT( dev );
	m_Dev = dev;
	m_Dev->AddRef();
	memset( m_FaceName, 0, LF_FACESIZE );
	SetFont( _T("ＭＳ ゴシック") );
}

//デストラクタ
CBillboardString::~CBillboardString()
{
	for ( std::size_t i = 0; i < m_StrAry.size(); i++ )
	{
		delete m_StrAry[ i ];
	}
	m_Dev->Release();
}

//文字列登録
void CBillboardString::RegistString( const TCHAR* str )
{
	//文字数チェックからCBillboardCharオブジェクト配列を手配
	unsigned int len , i;
	unsigned int updateCounter = 0;
#if _UNICODE
	//Unicodeの場合は単純
	len = ( unsigned int )wcslen( str );
	if( m_CurLen < len )
	{
		for ( i = m_CurLen; i < len; i++ ) m_StrAry.push_back( new CBillboardChar( m_Dev ) );
	}
	m_CurLen = len;

	for( i = 0; i < m_CurLen; i++ )
	{
		if( m_StrAry[i]->RegistChar( str + i ) != CBillboardChar::Regist_SameChar ) ++updateCounter;
	}
#else
	//マルチ文字の場合は文字解析が必要
	unsigned int counter = 0;
	len = (unsigned int)strlen( str );
	for( i = 0; i < len; i++ )
	{
		CBillboardChar *p;
		if( counter < m_CurLen )
		{
			p = m_StrAry[ counter ];
		}
		else
		{
			p = new CBillboardChar( m_Dev );
			m_StrAry.push_back( p );
		}

		if( p->RegistChar( str + i ) != CBillboardChar::Regist_SameChar )
		{
			++updateCounter;
		}

		if( IsDBCSLeadByte( str[ i ] ) )
		{
			++i;
		}
		++counter;
	}
	m_CurLen = counter;
#endif

	//文字の変更があった場合は更新し直す
	if( updateCounter > 0 )
	{
		m_IsUpdate = true;
		m_IsUpdatePosition = true;
	}
}

//位置関係更新
void CBillboardString::UpdatePositions()
{
	//登録されている文字列がないなら以下の処理を行わない
	if ( m_CurLen == 0 ) return;

	m_StrAry[ 0 ]->SetPosition( m_BasePos.x, m_BasePos.y );
	for ( std::size_t i = 1; i < m_CurLen; i++ )
	{
		//float x = m_StrAry[ i - 1 ]->GetRightPos();
		//float y = m_BasePos.y;
		m_StrAry[ i ]->SetPosition( m_StrAry[ i - 1 ]->GetRightPos(), m_BasePos.y );
	}
}

// フォント文字サイズ指定
bool CBillboardString::SetMaxPixelSize( unsigned int size )
{
	//同じサイズなので更新しない
	if ( m_PixelSize == size ) return true;
	m_PixelSize = size;
	m_IsUpdatePosition = true;
	m_IsUpdate = true;
	return false;
}

//フォントスケール指定
void CBillboardString::SetScale( float xs, float ys )
{
	//現在のスケールと指定のスケールが異なっているなら
	if( m_Scale.x != xs || m_Scale.y != ys )
	{
		m_Scale.x = xs;
		m_Scale.y = ys;
		m_IsUpdatePosition = true;
		m_IsUpdate = true;
	}
}

//位置指定
void CBillboardString::SetPosition( float x, float y )
{
	//現在の座標と指定の座標が異なっているなら
	if( m_BasePos.x != x || m_BasePos.y != y )
	{
		m_BasePos.x = x;
		m_BasePos.y = y;
		m_IsUpdatePosition = true;
	}
}

// フォント指定
bool CBillboardString::SetFont( const TCHAR *FaceName )
{
	//同じフォント名が指定されているので更新しない
	if( _tcscmp( m_FaceName, FaceName ) == 0 ) return true;

	_tcsncpy( m_FaceName, FaceName, LF_FACESIZE-1 );
	m_IsUpdatePosition = true;
	m_IsUpdate = true;
	return true;
}

// アルファ値指定
void CBillboardString::SetAlpha( int alpha ) {
	//アルファ値制限
	if( alpha >= 256 ) alpha = 255;
	if( alpha <= 0 ) alpha = 0;

	//現在のアルファ値と指定のアルファ値が異なっているなら
	if( m_Alpha != alpha )
	{
		m_Alpha = alpha;
		m_IsUpdate = true;
	}
}

// 文字のベースカラー指定
void CBillboardString::SetColor( DWORD color )
{
	//現在のカラー値と指定のカラー値が同じなら更新しない
	if ( m_Color == color ) return;

	m_Color = color & 0x00ffffff;
	m_IsUpdate = true;
}

// 更新
void CBillboardString::Update()
{
	for( std::size_t i = 0; i < m_CurLen; i++ )
	{
		m_StrAry[ i ]->SetColor( m_Color );
		m_StrAry[ i ]->SetAlpha( m_Alpha );
		m_StrAry[ i ]->SetFont( m_FaceName );
		m_StrAry[ i ]->SetScale( m_Scale.x, m_Scale.y );
		m_StrAry[ i ]->SetMaxPixelSize( m_PixelSize );
		m_StrAry[ i ]->Update();
	}
}

//描画
bool CBillboardString::Draw()
{
	//更新が必要な場合は全文字を更新
	if( m_IsUpdate )
	{
		Update();
		m_IsUpdate = false;
	}

	//位置更新が必要な場合に更新
	if ( m_IsUpdatePosition )
	{
		UpdatePositions();
		m_IsUpdatePosition = false;
	}

	for( std::size_t i = 0; i < m_CurLen; i++ )
	{
		m_StrAry[ i ]->Draw();
	}
	return true;
}
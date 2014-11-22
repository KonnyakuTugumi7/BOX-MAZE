// 2008. 11. 16 v1.00 -> v1.01
// �E����������𖈃t���[���o�^����ƃt��Update���������Ă��܂��o�O���C��
//

#include "DXUT.h"
#include "BillboardString.h"
#include <crtdbg.h>
#include <string.h>
#include <tchar.h>

//�R���X�g���N�^
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
	SetFont( _T("�l�r �S�V�b�N") );
}

//�f�X�g���N�^
CBillboardString::~CBillboardString()
{
	for ( std::size_t i = 0; i < m_StrAry.size(); i++ )
	{
		delete m_StrAry[ i ];
	}
	m_Dev->Release();
}

//������o�^
void CBillboardString::RegistString( const TCHAR* str )
{
	//�������`�F�b�N����CBillboardChar�I�u�W�F�N�g�z�����z
	unsigned int len , i;
	unsigned int updateCounter = 0;
#if _UNICODE
	//Unicode�̏ꍇ�͒P��
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
	//�}���`�����̏ꍇ�͕�����͂��K�v
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

	//�����̕ύX���������ꍇ�͍X�V������
	if( updateCounter > 0 )
	{
		m_IsUpdate = true;
		m_IsUpdatePosition = true;
	}
}

//�ʒu�֌W�X�V
void CBillboardString::UpdatePositions()
{
	//�o�^����Ă��镶���񂪂Ȃ��Ȃ�ȉ��̏������s��Ȃ�
	if ( m_CurLen == 0 ) return;

	m_StrAry[ 0 ]->SetPosition( m_BasePos.x, m_BasePos.y );
	for ( std::size_t i = 1; i < m_CurLen; i++ )
	{
		//float x = m_StrAry[ i - 1 ]->GetRightPos();
		//float y = m_BasePos.y;
		m_StrAry[ i ]->SetPosition( m_StrAry[ i - 1 ]->GetRightPos(), m_BasePos.y );
	}
}

// �t�H���g�����T�C�Y�w��
bool CBillboardString::SetMaxPixelSize( unsigned int size )
{
	//�����T�C�Y�Ȃ̂ōX�V���Ȃ�
	if ( m_PixelSize == size ) return true;
	m_PixelSize = size;
	m_IsUpdatePosition = true;
	m_IsUpdate = true;
	return false;
}

//�t�H���g�X�P�[���w��
void CBillboardString::SetScale( float xs, float ys )
{
	//���݂̃X�P�[���Ǝw��̃X�P�[�����قȂ��Ă���Ȃ�
	if( m_Scale.x != xs || m_Scale.y != ys )
	{
		m_Scale.x = xs;
		m_Scale.y = ys;
		m_IsUpdatePosition = true;
		m_IsUpdate = true;
	}
}

//�ʒu�w��
void CBillboardString::SetPosition( float x, float y )
{
	//���݂̍��W�Ǝw��̍��W���قȂ��Ă���Ȃ�
	if( m_BasePos.x != x || m_BasePos.y != y )
	{
		m_BasePos.x = x;
		m_BasePos.y = y;
		m_IsUpdatePosition = true;
	}
}

// �t�H���g�w��
bool CBillboardString::SetFont( const TCHAR *FaceName )
{
	//�����t�H���g�����w�肳��Ă���̂ōX�V���Ȃ�
	if( _tcscmp( m_FaceName, FaceName ) == 0 ) return true;

	_tcsncpy( m_FaceName, FaceName, LF_FACESIZE-1 );
	m_IsUpdatePosition = true;
	m_IsUpdate = true;
	return true;
}

// �A���t�@�l�w��
void CBillboardString::SetAlpha( int alpha ) {
	//�A���t�@�l����
	if( alpha >= 256 ) alpha = 255;
	if( alpha <= 0 ) alpha = 0;

	//���݂̃A���t�@�l�Ǝw��̃A���t�@�l���قȂ��Ă���Ȃ�
	if( m_Alpha != alpha )
	{
		m_Alpha = alpha;
		m_IsUpdate = true;
	}
}

// �����̃x�[�X�J���[�w��
void CBillboardString::SetColor( DWORD color )
{
	//���݂̃J���[�l�Ǝw��̃J���[�l�������Ȃ�X�V���Ȃ�
	if ( m_Color == color ) return;

	m_Color = color & 0x00ffffff;
	m_IsUpdate = true;
}

// �X�V
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

//�`��
bool CBillboardString::Draw()
{
	//�X�V���K�v�ȏꍇ�͑S�������X�V
	if( m_IsUpdate )
	{
		Update();
		m_IsUpdate = false;
	}

	//�ʒu�X�V���K�v�ȏꍇ�ɍX�V
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
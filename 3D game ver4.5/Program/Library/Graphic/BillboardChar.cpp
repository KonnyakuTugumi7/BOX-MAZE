#include "DXUT.h"
#include "../../Library/Graphic/BillboardString.h"
#include <crtdbg.h>
#include <stdio.h>

const CBillboardChar::_2DBILLBOARDVERTEX CBillboardChar::m_Vertex_g[4] = {
	{  0.5f, -0.5f, 0.0f, 1.0f, 0xffffffff, 1.0f, 0.0f },
	{  0.5f,  0.5f, 0.0f, 1.0f, 0xffffffff, 1.0f, 1.0f },
	{ -0.5f, -0.5f, 0.0f, 1.0f, 0xffffffff, 0.0f, 0.0f },
	{ -0.5f,  0.5f, 0.0f, 1.0f, 0xffffffff, 0.0f, 1.0f }
};

namespace{
	// �𑜓x�n�b�V��
	unsigned int resolutionHash_g[] = {
		GGO_GRAY2_BITMAP,
		GGO_GRAY4_BITMAP,
		GGO_GRAY8_BITMAP
	};

	// ����\�n�b�V��
	unsigned int level_g[] = { 5, 17, 65 };

	// �ׂ���O�a�l�Z�o
	unsigned int PowerSaturation( unsigned int val ) {
		val -= 1;
		BYTE S1 = (val >> 16) != 0;
		val >>= 16 * S1;
		BYTE S2 = (val >> 8) != 0;
		val >>= 8 * S2;
		BYTE S3 = (val >> 4) != 0;
		val >>= 4 * S3;
		BYTE S4 = (val >> 2) != 0;
		val >>= 2 * S4;
		BYTE S5 = (val >> 1) != 0;
		val >>= S5;
		BYTE S6 = ( BYTE )val;
		return 1 << (16*S1 + 8*S2 + 4*S3 + 2*S4 + S5 + S6);
	}
}

//�R���X�g���N�^
CBillboardChar::CBillboardChar( IDirect3DDevice9 *dev ) :
	m_pTex( 0 ),
	m_MaxPixel( 32 ),
	m_Dev( 0 ),
	m_IsUpdate( true ),
	m_hFont( 0 ),
	m_XScale( 1.0f ),
	m_YScale( 1.0f ),
	m_ISFontHandleRecreate( true ),
	m_TexSize( 0 ),
	m_Angle( 0 ),
	m_Pos_X( 0 ),
	m_Pos_Y( 0 ),
	m_Color( 0x00ffffff ),
	m_Alpha( 0xff ),
	m_Center_X( -0.5f ),
	m_Center_Y( -0.5f ),
	m_iBmpOfs_x( 0 ),
	m_iBmpOfs_y( 0 ),
	m_iBmp_W( 0 ),
	m_iBmp_H( 0 ),
	m_R_Margin( 0 ),
	m_B_Margin( 0 ),
	m_resolution( BILLBOARDCHAR_RESOLUTION_MIDDLE )
{
	m_Char[0] = 0;
	m_Char[1] = 0;

	_ASSERT( dev );
	m_Dev = dev;
	dev->AddRef();

	// ���_�o�b�t�@�̐���
	m_Dev->CreateVertexBuffer(sizeof(_2DBILLBOARDVERTEX)*4, D3DUSAGE_WRITEONLY, FVF_2DBILLBOARDVERTEX, D3DPOOL_MANAGED, &m_pVertexBuf, NULL);
	_ASSERT( m_pVertexBuf );
	void *pData;
	if( SUCCEEDED( m_pVertexBuf->Lock(0, sizeof(_2DBILLBOARDVERTEX)*4, (void**)&pData, 0))) {
		memcpy(pData, m_Vertex_g, sizeof(_2DBILLBOARDVERTEX)*4);
		m_pVertexBuf->Unlock();
	}

	memset( m_lfFaceName, 0, LF_FACESIZE );
	SetFont( _T("�l�r ����") );
	SetMaxPixelSize( m_MaxPixel );
}

//�f�X�g���N�^
CBillboardChar::~CBillboardChar()
{
	if( m_pVertexBuf )
		m_pVertexBuf->Release();
	if ( m_hFont )
		DeleteObject(m_hFont);
	if( m_pTex )
		m_pTex->Release();
	if( m_Dev )
		m_Dev->Release();
}

//�����o�^
CBillboardChar::Regist CBillboardChar::RegistChar( const TCHAR *c )
{
	_ASSERT( c );
	if( !c )
		return Regist_Error;
#if _UNICODE
	//unicode�̏ꍇ�P�����
	if(m_Char[0] == c[0] )
		return Regist_SameChar;

	m_Char[0] = c[0];
#else
	//�}���`�o�C�g�����̏ꍇ�A
	//1�o�C�g�����̃R�[�h��1�o�C�g�ڂ�UINT�ϊ��A
	//2�o�C�g�����̃R�[�h��[�擱�R�[�h]*256 + [�����R�[�h]�ł�
	if(IsDBCSLeadByte(*c)) {

		if( m_Char[0] == c[0] && m_Char[1] == c[1] )
			return Regist_SameChar;	//���������Ȃ̂ōX�V���Ȃ�

		m_Char[0] = c[0];
		m_Char[1] = c[1];
	}
	else {
		if( m_Char[0] == c[0] )
			return Regist_SameChar;
		m_Char[0] = c[0];
	}
#endif

	//�A�b�v�f�[�g
	m_IsUpdate = true;
	return Regist_OK;
}

//��{�����R�s�[
void CBillboardChar::CopyBaseInfoFrom( CBillboardChar* src )
{
	//�\�L�����ȊO�̏�����������R�s�[
	SetAlpha( src->m_Alpha );
	SetAngle( src->m_Angle );
	SetCenterPos( src->m_Center_X, src->m_Center_Y );
	SetColor( src->m_Color );
	SetFont( src->m_lfFaceName );
	SetMaxPixelSize( src->m_MaxPixel );
	SetPosition( src->m_Pos_X, src->m_Pos_Y );
	SetScale( src->m_XScale, src->m_YScale );
	SetResolution( src->m_resolution );
}

//�r���{�[�h�e�N�X�`���쐬
bool CBillboardChar::CreateTexture( unsigned int width, unsigned int height )
{
	_ASSERT( m_Dev );

	//�e�N�X�`���쐬
	IDirect3DTexture9* tmpTex = NULL;
	if( FAILED( m_Dev->CreateTexture( width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tmpTex, NULL ) ) )
	{
		if( FAILED( m_Dev->CreateTexture( width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tmpTex, NULL ) ) )
		{
			//�e�N�X�`���쐬���s
			_ASSERT( 0 );
			return false;
		}
	}

	if( m_pTex ) m_pTex->Release();
	m_pTex = tmpTex;

	return true;
}

//�t�H���g�n���h������
bool CBillboardChar::CreateFontHandle()
{
	LOGFONT lf = { m_MaxPixel, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_SCRIPT };
	_tcscpy( lf.lfFaceName, m_lfFaceName );
	lf.lfWeight = 500;
	HFONT tmpFont = CreateFontIndirect(&lf);
	if( !tmpFont )
	{
		_ASSERT( 0 );
		return false;
	}
	DeleteObject( m_hFont );
	m_hFont = tmpFont;
	return true;
}

// �����R�[�h�ϊ�
unsigned int CBillboardChar::TransCharCode( const TCHAR* c )
{
#if _UNICODE
	// unicode�̏ꍇ�A�����R�[�h�͒P���Ƀ��C�h������UINT�ϊ�
	 return (UINT)m_Char[0];
#else
	// �}���`�o�C�g�����̏ꍇ�A
	// 1�o�C�g�����̃R�[�h��1�o�C�g�ڂ�UINT�ϊ��A
	// 2�o�C�g�����̃R�[�h��[�擱�R�[�h]*256 + [�����R�[�h]
	if(IsDBCSLeadByte(m_Char[0])) return (BYTE)m_Char[0]<<8 | (BYTE)m_Char[1];
	return m_Char[0];
#endif
}

//�t�H���g�r�b�g�}�b�v�𐶐�
DWORD CBillboardChar::CreateFontBMP( TEXTMETRIC& TM, GLYPHMETRICS& GM, BYTE **outBMP )
{
	//�f�o�C�X�R���e�L�X�g�擾
	//�f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, m_hFont);

	//�����R�[�h�擾
	UINT code = TransCharCode( m_Char );

	GetTextMetrics( hdc, &TM );
	CONST MAT2 Mat = {{0,1},{0,0},{0,0},{0,1}};
	DWORD size = GetGlyphOutline(hdc, code, resolutionHash_g[m_resolution], &GM, 0, NULL, &Mat);

	if( size != 0 ) *outBMP = new BYTE[size];

	GetGlyphOutline(hdc, code, resolutionHash_g[m_resolution], &GM, size, *outBMP, &Mat);

	//���ƍ����A�I�t�Z�b�g�l�A�}�[�W�����Z�o
	m_iBmpOfs_x = GM.gmptGlyphOrigin.x;
	m_iBmp_W = GM.gmBlackBoxX + (4-(GM.gmBlackBoxX%4))%4;
	m_R_Margin = GM.gmCellIncX - GM.gmptGlyphOrigin.x - GM.gmBlackBoxX;
	m_iBmpOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
	m_iBmp_H = GM.gmBlackBoxY;
	m_B_Margin = TM.tmHeight - GM.gmptGlyphOrigin.y - GM.gmBlackBoxY;

	//�T�C�Y0�̕����i�󔒁j�̏ꍇ��
	//��Ŏ擾�����r�b�g�}�b�v�̕��ƍ����Ԃ񂾂�
	//��������^����
	if( !size )
	{
		*outBMP = new BYTE[m_iBmp_W * m_iBmp_H];
		memset( *outBMP, 0x00, m_iBmp_W * m_iBmp_H );
	}

	//�f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̊J��
	SelectObject(hdc, oldFont);
	ReleaseDC(NULL, hdc);

	return size;
}

//BMP���e�N�X�`���ɓK�p
bool CBillboardChar::ApplyBMPToTex( BYTE* bmp, TEXTMETRIC &TM, GLYPHMETRICS &GM )
{
	_ASSERT( bmp );
	D3DLOCKED_RECT LockedRect;
	if( FAILED( m_pTex->LockRect(0, &LockedRect, NULL, D3DLOCK_DISCARD ) ) )
	{
		if( FAILED( m_pTex->LockRect(0, &LockedRect, NULL, 0 ) ) )
		{
			//���b�N���s
			_ASSERT( 0 );
			return false;
		}
	}

	//�e�N�X�`��������
	memset( LockedRect.pBits , 0, LockedRect.Pitch * m_iBmp_H );	// �e�N�X�`����^������

	//iBmpOfs_x, iBmpOfs_y : �����o���ʒu(����)
	//iBmp_w, iBmp_h : �t�H���g�r�b�g�}�b�v�̕���
	//Level : ���l�̒i�K
	int Level = level_g[ m_resolution ];
	int x, y;
	DWORD Alpha, Color;
	for( y = 0; y < m_iBmp_H; y++)
	{
		for( x = 0; x < m_iBmp_W; x++)
		{
			Alpha = ( 255 * bmp[ x + m_iBmp_W * y ] ) / (Level-1);
#if _DEBUG
			Color = 0x00ffffff | (Alpha<<24) | 0x33000000;
#else
			Color = 0x00ffffff | (Alpha<<24);
#endif
			memcpy( (BYTE*)LockedRect.pBits + LockedRect.Pitch*y + 4*x, &Color, sizeof(DWORD) );
		}
	}
	m_pTex->UnlockRect(0);
	return true;
}

//�������e�N�X�`���ɍ���
bool CBillboardChar::PlotCharToTex()
{
	//�t�H���g�n���h������
	if( m_ISFontHandleRecreate && !CreateFontHandle() ) return false;

	//�t�H���g�r�b�g�}�b�v����
	TEXTMETRIC TM;
	GLYPHMETRICS GM;
	BYTE *bmp = 0;
	CreateFontBMP( TM, GM, &bmp );

	//�e�N�X�`���쐬
	CreateTexture( m_iBmp_W, m_iBmp_H );

	//�e�N�X�`���Ƀt�H���g�r�b�g�}�b�v��������
	ApplyBMPToTex( bmp, TM, GM );

	delete[] bmp;

	return true;
}

//�A�b�v�f�[�g
bool CBillboardChar::Update()
{
	if( m_Char && m_IsUpdate == true ) PlotCharToTex();

	m_IsUpdate = false;
	m_ISFontHandleRecreate = false;

	return true;
}

//�`��ʒu�X�V
bool CBillboardChar::UpdateSizeAndPosition( float x, float y)
{
	int i;
	_ASSERT( m_Dev );
	_2DBILLBOARDVERTEX *pData;
	if( FAILED( m_pVertexBuf->Lock(0, sizeof(_2DBILLBOARDVERTEX)*4, (void**)&pData, 0)))
		return false;

	float px, py, scx, scy;
	for ( i = 0; i < 4; i++ )
	{
		//���_�ό`
		scx = m_XScale * m_iBmp_W;
		scy = m_YScale * m_iBmp_H;
		px = m_Vertex_g[i].x * scx;
		py = m_Vertex_g[i].y * scy;

		//���S�_��]
		float tmp = px;
		px = cos( m_Angle ) * px  - sin( m_Angle ) * py;
		py = sin( m_Angle ) * tmp + cos( m_Angle ) * py;

		//�ʒu�ύX
		pData[i].x = px + x + m_XScale * m_iBmpOfs_x - scx * m_Center_X;
		pData[i].y = py + y + m_YScale * m_iBmpOfs_y - scy * m_Center_Y;
		pData[i].z = 0.0f;
		pData[i].rhw = 1.0f;

		//�e�N�X�`��UV�𕶎��̍ő�T�C�Y/�e�N�X�`���T�C�Y�ɍ��킹��
		pData[i].dwColor = m_Color | ( m_Alpha << 24 );
	}

	//UV�̍ő�l��1/�s�N�Z���T�C�Y�������g��
	pData[ 0 ].u = 1.0f + 1.0f / m_iBmp_W;
	pData[ 1 ].u = 1.0f + 1.0f / m_iBmp_W;
	pData[ 1 ].v = 1.0f + 1.0f / m_iBmp_H;
	pData[ 3 ].v = 1.0f + 1.0f / m_iBmp_H;

	m_pVertexBuf->Unlock();
	return true;
}

// �`��
bool CBillboardChar::Draw()
{
	//�A�b�v�f�[�g���K�v���`�F�b�N
	if( m_IsUpdate && !Update() ) return false;

	//�`��T�C�Y�ƈʒu�̍X�V
	UpdateSizeAndPosition( m_Pos_X, m_Pos_Y );

	//�`��(BeginScene���Ă΂�Ă���O��)
	_ASSERT( m_Dev );
	SetRenderState();
	m_Dev->SetStreamSource(0, m_pVertexBuf, 0, sizeof( _2DBILLBOARDVERTEX ));
	m_Dev->SetFVF( FVF_2DBILLBOARDVERTEX );
	m_Dev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	return true;
}

void CBillboardChar::SetRenderState()
{
	//�e�N�X�`���Z�b�g
	m_Dev->SetTexture(0, m_pTex);
	m_Dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE ); 
	m_Dev->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_MODULATE );
	m_Dev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE ); 
	m_Dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_Dev->SetTextureStageState(0, D3DTSS_ALPHAOP , D3DTOP_MODULATE ); 
	m_Dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE ); //�|���̃��l�𗘗p
	
	m_Dev->SetSamplerState( 0, D3DSAMP_BORDERCOLOR, 0x00000000 );
	m_Dev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER );
	m_Dev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER );

	//�����_�����O�X�e�[�g
	m_Dev->SetRenderState(D3DRS_ALPHABLENDENABLE, true );
	m_Dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	m_Dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
}
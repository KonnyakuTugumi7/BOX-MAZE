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
	// 解像度ハッシュ
	unsigned int resolutionHash_g[] = {
		GGO_GRAY2_BITMAP,
		GGO_GRAY4_BITMAP,
		GGO_GRAY8_BITMAP
	};

	// 分解能ハッシュ
	unsigned int level_g[] = { 5, 17, 65 };

	// べき乗飽和値算出
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

//コンストラクタ
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

	// 頂点バッファの生成
	m_Dev->CreateVertexBuffer(sizeof(_2DBILLBOARDVERTEX)*4, D3DUSAGE_WRITEONLY, FVF_2DBILLBOARDVERTEX, D3DPOOL_MANAGED, &m_pVertexBuf, NULL);
	_ASSERT( m_pVertexBuf );
	void *pData;
	if( SUCCEEDED( m_pVertexBuf->Lock(0, sizeof(_2DBILLBOARDVERTEX)*4, (void**)&pData, 0))) {
		memcpy(pData, m_Vertex_g, sizeof(_2DBILLBOARDVERTEX)*4);
		m_pVertexBuf->Unlock();
	}

	memset( m_lfFaceName, 0, LF_FACESIZE );
	SetFont( _T("ＭＳ 明朝") );
	SetMaxPixelSize( m_MaxPixel );
}

//デストラクタ
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

//文字登録
CBillboardChar::Regist CBillboardChar::RegistChar( const TCHAR *c )
{
	_ASSERT( c );
	if( !c )
		return Regist_Error;
#if _UNICODE
	//unicodeの場合単純代入
	if(m_Char[0] == c[0] )
		return Regist_SameChar;

	m_Char[0] = c[0];
#else
	//マルチバイト文字の場合、
	//1バイト文字のコードは1バイト目のUINT変換、
	//2バイト文字のコードは[先導コード]*256 + [文字コード]です
	if(IsDBCSLeadByte(*c)) {

		if( m_Char[0] == c[0] && m_Char[1] == c[1] )
			return Regist_SameChar;	//同じ文字なので更新しない

		m_Char[0] = c[0];
		m_Char[1] = c[1];
	}
	else {
		if( m_Char[0] == c[0] )
			return Regist_SameChar;
		m_Char[0] = c[0];
	}
#endif

	//アップデート
	m_IsUpdate = true;
	return Regist_OK;
}

//基本情報をコピー
void CBillboardChar::CopyBaseInfoFrom( CBillboardChar* src )
{
	//表記文字以外の情報を引数からコピー
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

//ビルボードテクスチャ作成
bool CBillboardChar::CreateTexture( unsigned int width, unsigned int height )
{
	_ASSERT( m_Dev );

	//テクスチャ作成
	IDirect3DTexture9* tmpTex = NULL;
	if( FAILED( m_Dev->CreateTexture( width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tmpTex, NULL ) ) )
	{
		if( FAILED( m_Dev->CreateTexture( width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tmpTex, NULL ) ) )
		{
			//テクスチャ作成失敗
			_ASSERT( 0 );
			return false;
		}
	}

	if( m_pTex ) m_pTex->Release();
	m_pTex = tmpTex;

	return true;
}

//フォントハンドル生成
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

// 文字コード変換
unsigned int CBillboardChar::TransCharCode( const TCHAR* c )
{
#if _UNICODE
	// unicodeの場合、文字コードは単純にワイド文字のUINT変換
	 return (UINT)m_Char[0];
#else
	// マルチバイト文字の場合、
	// 1バイト文字のコードは1バイト目のUINT変換、
	// 2バイト文字のコードは[先導コード]*256 + [文字コード]
	if(IsDBCSLeadByte(m_Char[0])) return (BYTE)m_Char[0]<<8 | (BYTE)m_Char[1];
	return m_Char[0];
#endif
}

//フォントビットマップを生成
DWORD CBillboardChar::CreateFontBMP( TEXTMETRIC& TM, GLYPHMETRICS& GM, BYTE **outBMP )
{
	//デバイスコンテキスト取得
	//デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, m_hFont);

	//文字コード取得
	UINT code = TransCharCode( m_Char );

	GetTextMetrics( hdc, &TM );
	CONST MAT2 Mat = {{0,1},{0,0},{0,0},{0,1}};
	DWORD size = GetGlyphOutline(hdc, code, resolutionHash_g[m_resolution], &GM, 0, NULL, &Mat);

	if( size != 0 ) *outBMP = new BYTE[size];

	GetGlyphOutline(hdc, code, resolutionHash_g[m_resolution], &GM, size, *outBMP, &Mat);

	//幅と高さ、オフセット値、マージンを算出
	m_iBmpOfs_x = GM.gmptGlyphOrigin.x;
	m_iBmp_W = GM.gmBlackBoxX + (4-(GM.gmBlackBoxX%4))%4;
	m_R_Margin = GM.gmCellIncX - GM.gmptGlyphOrigin.x - GM.gmBlackBoxX;
	m_iBmpOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
	m_iBmp_H = GM.gmBlackBoxY;
	m_B_Margin = TM.tmHeight - GM.gmptGlyphOrigin.y - GM.gmBlackBoxY;

	//サイズ0の文字（空白）の場合は
	//上で取得したビットマップの幅と高さぶんだけ
	//メモリを与える
	if( !size )
	{
		*outBMP = new BYTE[m_iBmp_W * m_iBmp_H];
		memset( *outBMP, 0x00, m_iBmp_W * m_iBmp_H );
	}

	//デバイスコンテキストとフォントハンドルの開放
	SelectObject(hdc, oldFont);
	ReleaseDC(NULL, hdc);

	return size;
}

//BMPをテクスチャに適用
bool CBillboardChar::ApplyBMPToTex( BYTE* bmp, TEXTMETRIC &TM, GLYPHMETRICS &GM )
{
	_ASSERT( bmp );
	D3DLOCKED_RECT LockedRect;
	if( FAILED( m_pTex->LockRect(0, &LockedRect, NULL, D3DLOCK_DISCARD ) ) )
	{
		if( FAILED( m_pTex->LockRect(0, &LockedRect, NULL, 0 ) ) )
		{
			//ロック失敗
			_ASSERT( 0 );
			return false;
		}
	}

	//テクスチャ初期化
	memset( LockedRect.pBits , 0, LockedRect.Pitch * m_iBmp_H );	// テクスチャを真透明に

	//iBmpOfs_x, iBmpOfs_y : 書き出し位置(左上)
	//iBmp_w, iBmp_h : フォントビットマップの幅高
	//Level : α値の段階
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

//文字をテクスチャに刻印
bool CBillboardChar::PlotCharToTex()
{
	//フォントハンドル生成
	if( m_ISFontHandleRecreate && !CreateFontHandle() ) return false;

	//フォントビットマップ生成
	TEXTMETRIC TM;
	GLYPHMETRICS GM;
	BYTE *bmp = 0;
	CreateFontBMP( TM, GM, &bmp );

	//テクスチャ作成
	CreateTexture( m_iBmp_W, m_iBmp_H );

	//テクスチャにフォントビットマップ書き込み
	ApplyBMPToTex( bmp, TM, GM );

	delete[] bmp;

	return true;
}

//アップデート
bool CBillboardChar::Update()
{
	if( m_Char && m_IsUpdate == true ) PlotCharToTex();

	m_IsUpdate = false;
	m_ISFontHandleRecreate = false;

	return true;
}

//描画位置更新
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
		//頂点変形
		scx = m_XScale * m_iBmp_W;
		scy = m_YScale * m_iBmp_H;
		px = m_Vertex_g[i].x * scx;
		py = m_Vertex_g[i].y * scy;

		//中心点回転
		float tmp = px;
		px = cos( m_Angle ) * px  - sin( m_Angle ) * py;
		py = sin( m_Angle ) * tmp + cos( m_Angle ) * py;

		//位置変更
		pData[i].x = px + x + m_XScale * m_iBmpOfs_x - scx * m_Center_X;
		pData[i].y = py + y + m_YScale * m_iBmpOfs_y - scy * m_Center_Y;
		pData[i].z = 0.0f;
		pData[i].rhw = 1.0f;

		//テクスチャUVを文字の最大サイズ/テクスチャサイズに合わせる
		pData[i].dwColor = m_Color | ( m_Alpha << 24 );
	}

	//UVの最大値を1/ピクセルサイズ分だけ拡張
	pData[ 0 ].u = 1.0f + 1.0f / m_iBmp_W;
	pData[ 1 ].u = 1.0f + 1.0f / m_iBmp_W;
	pData[ 1 ].v = 1.0f + 1.0f / m_iBmp_H;
	pData[ 3 ].v = 1.0f + 1.0f / m_iBmp_H;

	m_pVertexBuf->Unlock();
	return true;
}

// 描画
bool CBillboardChar::Draw()
{
	//アップデートが必要かチェック
	if( m_IsUpdate && !Update() ) return false;

	//描画サイズと位置の更新
	UpdateSizeAndPosition( m_Pos_X, m_Pos_Y );

	//描画(BeginSceneが呼ばれている前提)
	_ASSERT( m_Dev );
	SetRenderState();
	m_Dev->SetStreamSource(0, m_pVertexBuf, 0, sizeof( _2DBILLBOARDVERTEX ));
	m_Dev->SetFVF( FVF_2DBILLBOARDVERTEX );
	m_Dev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	return true;
}

void CBillboardChar::SetRenderState()
{
	//テクスチャセット
	m_Dev->SetTexture(0, m_pTex);
	m_Dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE ); 
	m_Dev->SetTextureStageState(0, D3DTSS_COLOROP , D3DTOP_MODULATE );
	m_Dev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE ); 
	m_Dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_Dev->SetTextureStageState(0, D3DTSS_ALPHAOP , D3DTOP_MODULATE ); 
	m_Dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE ); //板ポリのα値を利用
	
	m_Dev->SetSamplerState( 0, D3DSAMP_BORDERCOLOR, 0x00000000 );
	m_Dev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER );
	m_Dev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER );

	//レンダリングステート
	m_Dev->SetRenderState(D3DRS_ALPHABLENDENABLE, true );
	m_Dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	m_Dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
}
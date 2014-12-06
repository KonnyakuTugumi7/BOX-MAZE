#include "DXUT.h"
#include "font.h"
#include "../../Library/Support/common.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../Library/Graphic/vertex.h"
#include "../../GameEngine/Graphic/cube.h"
#include "../../GameEngine/Camera/camera_base.h"
#include <cassert>

//�f�X�g���N�^
CFont::~CFont()
{
	//�f�[�^�����ׂĉ��
	for ( auto it = CFont::m_data.begin() ; it != CFont::m_data.end() ; it++ )
	{
		SAFE_RELEASE( ( *it ).second );
	}
	m_data.clear();
}

//�`��
void CFont::Draw( const CameraBaseWP camera , const std::wstring& string , const D3DXVECTOR2& pos , const D3DXVECTOR4& color)
{
	HRESULT hr;
	//z�o�b�t�@�̂݃N���A
	V( CGraphicsManager::m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255 , 0 , 0 , 0/*255, 0, 0, 255*/ ), 1.0f, 0 ) );

	//�����(0,0)�ɂ���ׂ̃I�t�Z�b�g���`
	D3DXVECTOR2 offsetPt( CGraphicsManager::m_window_width * 0.5f , CGraphicsManager::m_window_height * 0.5f);

	// ���W���`
	D3DXVECTOR3 drawPt( 0 , offsetPt.y - pos.y , 0 );

	for( std::size_t i = 0 ; i < string.size() ; i++ )
	{
		// ���W�𐮗񏇂ɕ␳
		switch(m_align)
		{
		// ������
		case LEFT:
			drawPt.x = pos.x + ( i * m_scale.x ) - offsetPt.x;
			break;

		// �E����
		case RIGHT:
			drawPt.x = pos.x - ( m_scale.x * ( string.size() - 1 - i ) ) - offsetPt.x;
			break;

		// �^�񒆑���
		case CENTER:
			drawPt.x = pos.x + ( i * m_scale.x ) - ( m_scale.x * 0.5f * ( string.size() - 1 ) ) - offsetPt.x;
			break;
		}
		m_pos = drawPt;
		m_scl = m_scale;
		m_col = color;
		m_texs[ 0 ]->SetTex( m_data[ string.at( i ) ] );
		//�`��
		Render( camera );
	}
}

bool CFont::CreateTexture( std::size_t size , std::size_t weight , const std::wstring& type , const std::wstring& fonts )
{
	// �t�H���g�T�C�Y���Q�̗ݏ�T�C�Y�ŎZ�o����
	std::size_t fontSize = 1;
	while (fontSize < size) fontSize <<= 1;

	LOGFONT lf =
	{
		size,                      // �����Z���܂��͕����̍���
		0,                         // ���ϕ�����
		0,                         // ��������̕�����X���Ƃ̊p�x
		0,                         // �x�[�X���C����X���Ƃ̊p�x
		weight,                    // �t�H���g�̑���
		0,                         // �C�^���b�N�̎w��
		0,                         // �����t���w��
		0,                         // �ł��������t���w��
		SHIFTJIS_CHARSET,          // �L�����N�^�Z�b�g
		OUT_TT_ONLY_PRECIS,        // �o�͐��x
		CLIP_DEFAULT_PRECIS,       // �N���b�s���O�̐��x
		PROOF_QUALITY,             // �o�͕i��
		DEFAULT_PITCH | FF_MODERN, // �s�b�`�ƃt�@�~��
		NULL                       // �t�H���g��
	};
	
	// ���̂�ݒ肷��
	lstrcpyW(lf.lfFaceName, type.c_str());

	HFONT hFont = CreateFontIndirect(&lf);
	if (!hFont)
	{
		MessageBox(NULL, TEXT(" !! Font Create Err 01 !! "), NULL, MB_OK | MB_ICONSTOP);
		return false;
	}	
	
	// �f�o�C�X�R���e�L�X�g�擾
	// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
	
	for(std::size_t i = 0; i < fonts.size(); i++)
	{
		// �t�H���g�r�b�g�}�b�v�擾
		const wchar_t c = fonts.at(i);

		// ���d������h��
		auto it = m_data.find(c);
		if (it != m_data.end()) continue;

		// 1 �����R�[�h�擾
		UINT code = (UINT)c;
	
		// �t�H���g�r�b�g�}�b�v�擾
		TEXTMETRIC TM;
		GetTextMetrics(hdc, &TM);
		GLYPHMETRICS GM;
		CONST MAT2 Mat = { {0,1}, {0,0}, {0,0}, {0,1} };
	
		//DWORD outline = GetGlyphOutline(hdc, code, GGO_BITMAP, &GM, 0, NULL, &Mat);
		//DWORD outline = GetGlyphOutline(hdc, code, GGO_GRAY2_BITMAP, &GM, 0, NULL, &Mat);
		//DWORD outline = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat);
		DWORD outline = GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &GM, 0, NULL, &Mat);
	
		BYTE *ptr = new BYTE[outline];
		
		//BYTE *ptr = (BYTE*)T2Malloc(sizeof(BYTE) * outline);
		//GetGlyphOutline(hdc, code, GGO_BITMAP, &GM, outline, ptr, &Mat);
		//GetGlyphOutline(hdc, code, GGO_GRAY2_BITMAP, &GM, outline, ptr, &Mat);
		//GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, outline, ptr, &Mat);
		GetGlyphOutline(hdc, code, GGO_GRAY8_BITMAP, &GM, outline, ptr, &Mat);

		// �e�N�X�`���쐬
		D3DLOCKED_RECT LockedRect;
		LPDIRECT3DTEXTURE9 texture = NULL;

		//if (FAILED(device->CreateTexture(GM.gmCellIncX, TM.tmHeight, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL)))
		if (FAILED(CGraphicsManager::m_pd3dDevice->CreateTexture(GM.gmCellIncX, TM.tmHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture, NULL)))
		{
			MessageBox(NULL, TEXT(" !! Font Create Err 02 !! "), NULL, MB_OK | MB_ICONSTOP);
		}

		if (FAILED(texture->LockRect(0, &LockedRect, NULL, D3DLOCK_DISCARD)))
		{
			MessageBox(NULL, TEXT(" !! Font Create Err 03 !! "), NULL, MB_OK | MB_ICONSTOP);
		}

		// �t�H���g���̏�������
		// iOfs_x, iOfs_y : �����o���ʒu(����)
		// iBmp_w, iBmp_h : �t�H���g�r�b�g�}�b�v�̕���
		// Level : ���l�̒i�K (GGO_GRAY4_BITMAP��17�i�K�AGGO_GRAY8_BITMAP��65�i�K)
		int iOfs_x = GM.gmptGlyphOrigin.x;
		int iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
		int iBmp_w = GM.gmBlackBoxX + (4 - (GM.gmBlackBoxX % 4)) % 4;
		int iBmp_h = GM.gmBlackBoxY;
		int Level = 65;

		FillMemory(LockedRect.pBits, LockedRect.Pitch * TM.tmHeight, 0);
		
		for (int y = iOfs_y ; y < iOfs_y+iBmp_h ; y++)
		{
			for (int x = iOfs_x ; x < iOfs_x+iBmp_w ; x++)
			{	
				DWORD Alpha = (DWORD)ptr[x-iOfs_x + iBmp_w * (y-iOfs_y)];
				Alpha = Alpha * (256 / (Level - 1));
				if (Alpha > 255) Alpha = 255;

				// �F�͔|���S���Őݒ肷�邽�ߔ��ɂ���
				DWORD Color = D3DCOLOR_ARGB(Alpha, 255, 255, 255);
				memcpy((BYTE*)LockedRect.pBits + LockedRect.Pitch * y + 4 * x, &Color, sizeof(DWORD));
			}
		}
		
		texture->UnlockRect(0);
		
		delete[] ptr;
		
		// �f�[�^�}�b�v�ɓo�^����
		CFont::m_data[c] = texture;
	}
	
	// �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h�����������
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	return true;
}

//�`��
void CFont::Render( const CameraBaseWP camera )
{
	HRESULT hr;

	//���[���h�}�g���N�X�ݒ�(���[�J�����W���烏�[���h���W��)
	if( m_is_billboard == true )
	{
		SetWldMtxBillBoard( camera );
	}
	else
	{
		//�e(cube)�����݂���ꍇ�͐e�̃��[���h�}�g���b�N�X��n��
		m_cube.expired() ? SetWldMtx() : SetWldMtx( &m_cube.lock()->GetWldMtx() );
	}
	m_wld_mtx = m_wld_mtx * camera.lock()->GetViewMatrix() * camera.lock()->GetProjectionMatrix();

	//�e�����݂��Ă��e�̕`��t���O���I�t�Ȃ�ȉ��̏������s��Ȃ�
	if( m_cube.expired() == false && m_cube.lock()->GetIsRender() == false ) return;

	//�J�����O�ݒ�
	CGraphicsManager::m_pd3dDevice->SetRenderState( D3DRS_CULLMODE , m_cull_state );

	//�V�F�[�_�ݒ�
	SetSDR();

	//----------------------------�`�揈��-------------------------------

	//�V�F�[�_�Ƀe�N�X�`����n��
	if( 0 != m_texs.size() )
	{
		CGraphicsManager::m_sdr_use->m_sdr->SetTexture( CGraphicsManager::m_sdr_use->m_sdr_tex , m_texs[ 0 ]->GetTex() );
		CGraphicsManager::m_sdr_use->m_sdr->SetVector( CGraphicsManager::m_sdr_use->m_sdr_color , /*&D3DXVECTOR4( 1 , 1 , 1 , 1 )*/&D3DXVECTOR4( m_col.x , m_col.y , m_col.z , m_col.w ) );
	}
	//�e�N�X�`����������΃J���[��n��
	else
	{
		CGraphicsManager::m_sdr_use->m_sdr->SetVector( CGraphicsManager::m_sdr_use->m_sdr_color , &D3DXVECTOR4( m_col.x , m_col.y , m_col.z , m_col.w ) );
	}

	//�V�F�[�_�̎g�p�J�n
	CGraphicsManager::m_sdr_use->Begin();

	// �V�F�[�_�̃p�X�ݒ�
	if( 0 != m_texs.size() )
	{
		if( !m_is_add_blend )
		{
			CGraphicsManager::m_sdr_use->BeginPass( 0 );
		}
		else
		{
			CGraphicsManager::m_sdr_use->BeginPass( 1 );
		}
	}
	else
	{
		CGraphicsManager::m_sdr_use->BeginPass( 2 );
	}

	if( SUCCEEDED( CGraphicsManager::m_pd3dDevice->BeginScene() ) )
	{
		if(D3D_OK != m_mesh->GetMesh()->DrawSubset( 0 ))
		{
			return;
		}
		V( CGraphicsManager::m_pd3dDevice->EndScene() );
	}

	//�p�X�I��
	CGraphicsManager::m_sdr_use->EndPass();

	//�V�F�[�_�I��
	CGraphicsManager::m_sdr_use->End();
}

//����
DebugFontSP CDebugFont::Create( const int width , const int height , const int type )
{
	//�t�H���g
	DebugFontSP font = std::make_shared< CDebugFont >();

	//������
	font->Init( width , height , type );

	return font;
}

//������
void CDebugFont::Init( const int width , const int height , const int type )
{
	//�t�H���g���
	D3DXFONT_DESC FontInfo;

	FontInfo.Height          = height;
	FontInfo.Width           = width;
    FontInfo.Weight          = 0;		
    FontInfo.MipLevels       = 0;
    FontInfo.Italic          = FALSE;
    FontInfo.CharSet         = SHIFTJIS_CHARSET;
    FontInfo.OutputPrecision = OUT_TT_ONLY_PRECIS;
    FontInfo.Quality         = DEFAULT_QUALITY;
    FontInfo.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
	if( type == 0 )
	{
		lstrcpy( FontInfo.FaceName , L"�l�r �S�V�b�N" );
	}
	else
	{
		lstrcpy( FontInfo.FaceName , L"�l�r ����" );
	}
	
	//�t�H���g�쐬
	D3DXCreateFontIndirect( CGraphicsManager::m_pd3dDevice , &FontInfo , &m_font );
}

//�`��
void CDebugFont::DrawFont( const std::string str , const int posX , const int posY , const FontColor fontcolor )
{
	char buff[ 256 ] = { 0 };
	va_list ap;
 	va_start( ap , str );
	vsprintf( buff , str.c_str() , ap );
	va_end( ap );
	
	//�`�悷�镶����
	WCHAR buff_name[ 256 ] = { 0 };
	//WCHAR�^�֕ϊ�
	CCommon::DXconvAnsiToWide( buff_name , buff , 256 );

	//�`��͈�
	RECT rect;

	rect.left = posX;
	rect.top = posY;
	rect.right = 0;
	rect.bottom = 0;

	HRESULT hr;

	//�e�L�X�g�`��
	if( SUCCEEDED( CGraphicsManager::m_pd3dDevice->BeginScene() ) )
	{
		m_font->DrawText( NULL ,
						  buff_name ,
						  -1 ,
						  &rect ,
						  DT_LEFT | DT_NOCLIP ,
						  fontcolor
						);

		V( CGraphicsManager::m_pd3dDevice->EndScene() );
	}
}
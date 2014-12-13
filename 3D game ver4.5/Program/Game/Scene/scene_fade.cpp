#include "DXUT.h"
#include "scene_fade.h"
#include "../../Library/Support/common.h"
#include "../../Library/Input/input.h"
#include "../../Library/Sound/sound.h"
#include "../../Library/Sound/sound_manager.h"
#include "../../Library/Graphic/graphics_manager.h"
#include "../../GameEngine/Scene/scene_manager.h"
#include "../../GameEngine/Graphic/sprite.h"
#include "../../Game/Scene/scene_title.h"
#include "../../Game/Camera/fp_camera.h"
#include "../../Game/Camera/tp_camera.h"
#include "../../GameEngine/Scene/scene_manager.h"

//������
void CSceneFade::SceneInit()
{
	//CSV
	m_Csv = CCsv::Create( "Content/csv/FadeData.csv" );

	//�J����
	m_Camera = CTPCamera::Create();

	//�t�F�[�h��ԊǗ��i���o�[
	m_FadeMode = FADE_NON;
	
	//�t�F�[�h�摜
	//m_Fade = CBoard::Create( CGraphicsManager::m_WindowWidth , CGraphicsManager::m_WindowHeight , "Content/graphics/fade_img.bmp" , CGraphicsManager::RENDERLIST_STATE::FRONT_2D );
	m_Fade = std::make_shared< CBoard >();
	m_Fade->Init( CGraphicsManager::m_WindowWidth , CGraphicsManager::m_WindowHeight , "Content/graphics/fade_img.bmp" );
	m_Fade->m_Col = D3DXVECTOR4( 1 , 1 , 1 , 0 );
	m_Fade->SetCameraDistance( 0.1f );
	m_Fade->SetIsRender( false );
}

//�������
void CSceneFade::SceneDelete()
{
	//�t�F�[�h�摜���
	m_Fade.reset();
}

//���W�b�N����
void CSceneFade::SceneFrameMove( const float elapsed_time )
{	
	//�t�F�[�h���łȂ���Έȉ��̏������s��Ȃ�
	if( m_FadeMode == FADE_NON )
	{
		m_Fade->SetIsRender( false );
		return;
	}
	else
	{
		m_Fade->SetIsRender( true );
	}

	//�������A���t�@�l�����Ă���
	m_Fade->m_Col.w += 1.0f / m_FadeMode * elapsed_time;

	//�t�F�[�h�C��
	if( m_FadeMode == FADE_IN )
	{
		//�\�񂳂�Ă���V�[�P���X�Ɉڍs
		if( 0.0f > m_Fade->m_Col.w )
		{
			//�O�̂��߃��~�b�g
			m_Fade->m_Col.w = 0.0f;

			//�ʏ��Ԃ�
			m_FadeMode = FADE_NON;
		}
	}
	//�t�F�[�h�A�E�g
	else if( m_FadeMode == FADE_OUT )
	{
		if( 1.0f < m_Fade->m_Col.w )
		{	
			//�O�̂��߃��~�b�g
			m_Fade->m_Col.w = 1.0f;

			//�t�F�[�h�C����
			m_FadeMode = FADE_IN;
		}
	}
}

//�`�揈��
void CSceneFade::SceneFrameRender( const float elapsed_time )
{
	//�t�F�[�h���łȂ���Έȉ��̏������s��Ȃ�
	if( m_FadeMode == FADE_NON ) return;

	//�`��
	m_Fade->Render( m_Camera );
}

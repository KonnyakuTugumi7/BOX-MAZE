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
	m_csv = CCsv::Create( "Content/csv/FadeData.csv" );

	//�J����
	m_camera = CTPCamera::Create();

	//�t�F�[�h��ԊǗ��i���o�[
	m_fade_mode = FADE_NON;

	//�t�F�[�h�p�摜���[�h
	//m_fade = CSprite::Create( "Content/graphics/fade_img.bmp" , CGraphicsManager::FRONT_2D );
	m_fade = std::make_shared< CSprite >();
	m_fade->Init( "Content/graphics/fade_img.bmp" );
	m_fade->m_pos = D3DXVECTOR3( m_csv->GetToken< int >( 0 , 0 ) , m_csv->GetToken< int >( 0 , 1 ) , 0 );
	m_fade->m_col = D3DXVECTOR4( 1 , 1 , 1 , 0 );
	m_fade->SetCameraDistance( 0.1f );
}

//�������
void CSceneFade::SceneDelete()
{
	//�t�F�[�h�摜���
	m_fade.reset();
}

//���W�b�N����
void CSceneFade::SceneFrameMove( const float elapsed_time )
{	
	//�t�F�[�h���łȂ���Έȉ��̏������s��Ȃ�
	if( m_fade_mode == FADE_NON ) return;

	//�������A���t�@�l�����Ă���
	m_fade->m_col.w += 1.0f / m_fade_mode * elapsed_time;

	//�t�F�[�h�C��
	if( m_fade_mode == FADE_IN )
	{
		//�\�񂳂�Ă���V�[�P���X�Ɉڍs
		if( 0.0f > m_fade->m_col.w )
		{
			//�O�̂��߃��~�b�g
			m_fade->m_col.w = 0.0f;

			//�ʏ��Ԃ�
			m_fade_mode = FADE_NON;
		}
	}
	//�t�F�[�h�A�E�g
	else if( m_fade_mode == FADE_OUT )
	{
		if( 1.0f < m_fade->m_col.w )
		{	
			//�O�̂��߃��~�b�g
			m_fade->m_col.w = 1.0f;

			//�t�F�[�h�C����
			m_fade_mode = FADE_IN;
		}
	}
}

//�`�揈��
void CSceneFade::SceneFrameRender( const float elapsed_time )
{
	//�t�F�[�h���łȂ���Έȉ��̏������s��Ȃ�
	if( m_fade_mode == FADE_NON ) return;

	//�`��
	m_fade->Render( m_camera );
}

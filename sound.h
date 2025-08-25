//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : KAITO IWASAKI
// Special Thanks : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM003,			// BGM3
	SOUND_LABEL_BGM004,			// BGM4
	SOUND_LABEL_STAGE01,
	SOUND_LABEL_STAGE02,
	SOUND_LABEL_STAGE03,
	SOUND_LABEL_STAGE04,
	SOUND_LABEL_STAGE05,
	SOUND_LABEL_SE_SHOT,		// �e���ˉ�
	SOUND_LABEL_SE_HIT000,		// �q�b�g��
	SOUND_LABEL_SE_HIT001,		// �q�b�g��
	SOUND_LABEL_SE_DAMAGE,		// �q�b�g��
	SOUND_LABEL_SE_EXPLOSION,	// ������
	SOUND_LABEL_SE_PAUSE,		// ������
	SOUND_LABEL_SE_GAMEOVER,	// ������
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
HRESULT PlaySound(SOUND_LABEL label, float fVolume);
void StopSound(SOUND_LABEL label);
void StopSound(void);
void PauseSound(SOUND_LABEL label);
HRESULT UnPauseSound(SOUND_LABEL label);

#endif

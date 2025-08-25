//=============================================================================
//
// サウンド処理 [sound.h]
// Author : KAITO IWASAKI
// Special Thanks : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
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
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_HIT000,		// ヒット音
	SOUND_LABEL_SE_HIT001,		// ヒット音
	SOUND_LABEL_SE_DAMAGE,		// ヒット音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_PAUSE,		// 爆発音
	SOUND_LABEL_SE_GAMEOVER,	// 爆発音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
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

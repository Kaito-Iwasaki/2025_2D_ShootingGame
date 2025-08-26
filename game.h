//=====================================================================
// 
// game.cppのヘッダファイル [game.h]
// Author:岩崎桧翔
//
//=====================================================================
#ifndef _GAME_H_
#define _GAME_H_		// 二重インクルード防止のマクロ

#include "main.h"

//*********************************************************************
// ウェーブの種類
//*********************************************************************
typedef enum
{
	WAVE_NONE = 0,
	WAVE_START,
	WAVE_PROGRESSING,
	WAVE_END,
	WAVE_MAX
}WAVE;

typedef struct
{
	bool bPaused;
	int nGameCount;
	int waveState;
	int nWave;
}STAGE;

typedef struct
{
	int nWave;
	int nCountTime;
	int nType;
	D3DXVECTOR3 nPos;
	bool bInversed;
}ENEMYTIMELINE;

// プロトタイプ宣言
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void LoadStage(ENEMYTIMELINE* pTimeline, const char* pFileName);

#endif
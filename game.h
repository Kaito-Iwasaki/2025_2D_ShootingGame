//=====================================================================
// 
// game.cpp�̃w�b�_�t�@�C�� [game.h]
// Author:���O��
//
//=====================================================================
#ifndef _GAME_H_
#define _GAME_H_		// ��d�C���N���[�h�h�~�̃}�N��

#include "main.h"

//*********************************************************************
// �E�F�[�u�̎��
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

// �v���g�^�C�v�錾
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void LoadStage(ENEMYTIMELINE* pTimeline, const char* pFileName);

#endif
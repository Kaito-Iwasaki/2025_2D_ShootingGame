//=====================================================================
// 
// score.cpp�̃w�b�_�t�@�C�� [score.h]
// Author:���O��
//
//=====================================================================
#ifndef _SCORE_H_
#define _SCOREH_		// ��d�C���N���[�h�h�~�̃}�N��

#include "main.h"

// �}�N����`
#define NUM_PLACE		(8)		// ����

// �v���g�^�C�v�錾
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValue);
int GetScore(void);

#endif
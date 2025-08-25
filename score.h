//=====================================================================
// 
// score.cppのヘッダファイル [score.h]
// Author:岩崎桧翔
//
//=====================================================================
#ifndef _SCORE_H_
#define _SCOREH_		// 二重インクルード防止のマクロ

#include "main.h"

// マクロ定義
#define NUM_PLACE		(8)		// 桁数

// プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValue);
int GetScore(void);

#endif
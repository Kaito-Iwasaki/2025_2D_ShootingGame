//=====================================================================
// 
// bg.cppのヘッダファイル [bg.h]
// Author:岩崎桧翔
//
//=====================================================================
#ifndef _BG_H_
#define _BG_H_		// 二重インクルード防止のマクロ

#include "main.h"

// 背景構造体
typedef struct
{
	D3DXVECTOR3 pos;
	float fMove;
	D3DXCOLOR col;
}Background;

// プロトタイプ宣言
void InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

#endif
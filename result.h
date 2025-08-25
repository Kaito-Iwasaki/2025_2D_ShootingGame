//=====================================================================
// 
// result.cppのヘッダファイル [result.h]
// Author:岩崎桧翔
//
//=====================================================================
#ifndef _RESULT_H_
#define _RESULT_H_		// 二重インクルード防止のマクロ

#include "main.h"

// 背景構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
}Result;

// プロトタイプ宣言
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif
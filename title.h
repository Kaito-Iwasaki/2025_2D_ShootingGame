//=====================================================================
// 
// title.cppのヘッダファイル [title.h]
// Author:岩崎桧翔
//
//=====================================================================
#ifndef _TITLE_H_
#define _TITLE_H_		// 二重インクルード防止のマクロ

#include "main.h"

// 背景構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
}Title;

// プロトタイプ宣言
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif
//=====================================================================
// 
// pause.cppのヘッダファイル [pause.h]
// Author:岩崎桧翔
//
//=====================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_		// 二重インクルード防止のマクロ

#include "main.h"

// 背景構造体
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,
	PAUSE_MENU_RETRY,
	PAUSE_MENU_QUIT,
	PAUSE_MENU_MAX
}PAUSE_MENU;

// プロトタイプ宣言
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void SetPauseMenuCursor(int nCursor);

#endif
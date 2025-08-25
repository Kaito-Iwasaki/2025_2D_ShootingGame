//=====================================================================
// 
// input.cppのヘッダファイル [input.h]
// Author:岩崎桧翔
//
//=====================================================================
#ifndef _INPUT_H_
#define _INPUT_H_		// 二重インクルード防止のマクロ

#include "main.h"

//*********************************************
// マクロ定義
//*********************************************
#define INPUT_MAX_MAGNITUDE		(32767)		// スティックの最大移動量
#define INPUT_DEADZONE			(3000)		// スティックのデッドゾーン（0～32767）

//*********************************************
// ジョイパッドのキーの種類
//*********************************************
typedef enum
{
	JOYKEY_UP = 0,		// 十字キー上
	JOYKEY_DOWN,		// 十字キー下
	JOYKEY_LEFT,		// 十字キー左
	JOYKEY_RIGHT,		// 十字キー右
	JOYKEY_START,		// スタート
	JOYKEY_BACK,		// バック（セレクト）
	JOYKEY_LTHUMB,		// 左スティック押し込み
	JOYKEY_RTHUMB,		// 右スティック押し込み
	JOYKEY_LSHOULDER,	// LB
	JOYKEY_RSHOULDER,	// RB
	JOYKEY_A = 12,		// A
	JOYKEY_B,			// B
	JOYKEY_X,			// X
	JOYKEY_Y,			// Y
	JOYKEY_MAX			// 十字キー上
}JOYKEY;

//*********************************************
// プロトタイプ宣言
//*********************************************
// [キーボード]
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey);
// [ジョイパッド]
HRESULT InitJoypad(void);
void UninitJoypad(void);
void UpdateJoypad(void);
XINPUT_STATE* GetJoypad(void);
bool GetJoypadPress(JOYKEY key);
bool GetJoypadTrigger(JOYKEY key);
bool GetJoypadRelease(JOYKEY key);
void SetVibration(WORD wLeftMotorSpeed, WORD wRightMotorSpeed);

#endif
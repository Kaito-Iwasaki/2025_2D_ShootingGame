//=====================================================================
// 
// main.cppのヘッダファイル [main.h]
// Author:岩崎桧翔
//
//=====================================================================
#ifndef _MAIN_H_	// if not defined (main.hが定義されていなかったら)
#define _MAIN_H_	// 二重インクルード防止のマクロを定義する(これは#pragma onceとまったく同じ意味)

#include <windows.h>
#include <stdio.h>
#include "d3dx9.h"							// 描画処理に必要
#define	DIRECTINPUT_VERSION		(0x0800)	
#include "dinput.h"
#include "xaudio2.h"
#include "xinput.h"

// ライブラリのリンク
#pragma comment(lib, "d3d9.lib")		// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")		// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib, "dxguid.lib")		// DirectXコンポーネント（部品）使用に必要
#pragma comment(lib, "winmm.lib")		// システム時刻取得に必要
#pragma comment(lib, "dinput8.lib")		// 入力処理に必要
#pragma comment(lib, "xinput.lib")		// ジョイパッド処理に必要

// マクロ定義
#define SCREEN_WIDTH		(1280)		// ウィンドウの幅
#define SCREEN_HEIGHT		(720)		// ウィンドウの高さ
#define WINDOWED			(true)		// ウィンドウ表示するか
#define BG_COLOR			(D3DCOLOR_RGBA(0, 0, 0, 0))
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 頂点フォーマット[2D]
#define D3DXVECTOR3_ZERO	D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define VIBRATION_ENABLED		(true)		// 振動が有効か

// 画面（モード）の種類
typedef enum
{
	MODE_TITLE = 0,
	MODE_GAME,
	MODE_STAGE1,
	MODE_RESULT,
	MODE_MAX
}MODE;

// 頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_2D;

// プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
HWND GetMainWindow(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif
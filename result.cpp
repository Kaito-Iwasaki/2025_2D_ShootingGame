//=====================================================================
// 
// 背景処理 [result.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "result.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "player.h"
#include "score.h"

// マクロ定義
#define TEXTURE_FILENAME			"data\\TEXTURE\\sakai2.png"
#define TEXTURE_POS_X				(SCREEN_WIDTH / 2)
#define TEXTURE_POS_Y				(SCREEN_HEIGHT / 2)
#define TEXTURE_SCALE_X				(SCREEN_WIDTH)
#define TEXTURE_SCALE_Y				(SCREEN_HEIGHT)
#define TEXTURE_COLOR_CLEAR			D3DCOLOR_RGBA(255, 255, 255, 255)
#define TEXTURE_COLOR_GAMEOVER		D3DCOLOR_RGBA(255, 0, 0, 255)

#define MAX_LABEL		(2)

// グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureResult = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// 頂点バッファへのポインタ
Result g_result;
LPD3DXFONT g_pFontLabel[MAX_LABEL] = {};						// フォントへのポインタ

//=====================================================================
// 初期化処理
//=====================================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ
	Result* pResult = &g_result;

	// デバイスの取得
	pDevice = GetDevice();

	// デバッグ表示用のフォントを設定
	D3DXFONT_DESC fontDesc = {
	40,                        // Height
	0,                         // Width (0 = 自動)
	FW_NORMAL,                 // Weight
	1,                         // MipLevels
	FALSE,                     // Italic
	DEFAULT_CHARSET,          // CharSet
	OUT_DEFAULT_PRECIS,       // OutputPrecision
	CLIP_DEFAULT_PRECIS,      // Quality
	DEFAULT_PITCH | FF_DONTCARE, // PitchAndFamily
	"x10y12pxDonguriDuel"              // FaceName（.ttf名）
	};

	// デバッグ表示用のフォントを作成
	for (int nCountFont = 0; nCountFont < MAX_LABEL; nCountFont++)
	{
		D3DXCreateFontIndirect(pDevice, &fontDesc, &g_pFontLabel[nCountFont]);
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE_FILENAME,
		&g_pTextureResult
	);

	// 弾の情報の初期化
	pResult->pos = D3DXVECTOR3(TEXTURE_POS_X, TEXTURE_POS_Y, 0.0f);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL
	);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pResult->pos.x - TEXTURE_SCALE_X / 2, pResult->pos.y - TEXTURE_SCALE_Y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pResult->pos.x + TEXTURE_SCALE_X / 2, pResult->pos.y - TEXTURE_SCALE_Y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pResult->pos.x - TEXTURE_SCALE_X / 2, pResult->pos.y + TEXTURE_SCALE_Y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pResult->pos.x + TEXTURE_SCALE_X / 2, pResult->pos.y + TEXTURE_SCALE_Y / 2, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = pResult->col;
	pVtx[1].col = pResult->col;
	pVtx[2].col = pResult->col;
	pVtx[3].col = pResult->col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffResult->Unlock();

	PlaySound(SOUND_LABEL_SE_GAMEOVER);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitResult(void)
{
	// フォントの破棄
	for (int nCountFont = 0; nCountFont < MAX_LABEL; nCountFont++)
	{
		if (g_pFontLabel[nCountFont] != NULL)
		{
			g_pFontLabel[nCountFont]->Release();
			g_pFontLabel[nCountFont] = NULL;
		}

	}

	// テクスチャの破棄
	if (g_pTextureResult != NULL)
	{
		g_pTextureResult->Release();
		g_pTextureResult = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadPress(JOYKEY_START))
	{
		SetFade(MODE_TITLE);
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	RECT rect = { SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 200, SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT / 2 + 200 };
	char aMessage[256];

	if (IsPlayerDead() == true)
	{
		g_pFontLabel[0]->DrawText(NULL, "GAME OVER", -1, &rect, DT_CENTER | DT_VCENTER, D3DXCOLOR(255, 0, 0, 255));
	}
	else
	{
		wsprintf(aMessage, "GAME CLEAR\n YOUR SCORE:%d", GetScore());
		g_pFontLabel[0]->DrawText(NULL, aMessage, -1, &rect, DT_CENTER | DT_VCENTER, D3DXCOLOR(255, 255, 0, 255));
	}
}
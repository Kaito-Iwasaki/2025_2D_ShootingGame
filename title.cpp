//=====================================================================
// 
// 背景処理 [title.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

// マクロ定義
#define TEXTURE_FILENAME		"data\\TEXTURE\\title000.png"
#define TEXTURE_POS_X			(SCREEN_WIDTH / 2)
#define TEXTURE_POS_Y			(SCREEN_HEIGHT / 2)
#define TEXTURE_SCALE_X			(SCREEN_WIDTH)
#define TEXTURE_SCALE_Y			(SCREEN_HEIGHT)
#define TEXTURE_COLOR			D3DCOLOR_RGBA(255, 255, 255, 255)

// グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureTitle = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;			// 頂点バッファへのポインタ
Title g_title;

//=====================================================================
// 初期化処理
//=====================================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ
	Title* pTitle = &g_title;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE_FILENAME,
		&g_pTextureTitle
	);

	// 弾の情報の初期化
	pTitle->pos = D3DXVECTOR3(TEXTURE_POS_X, TEXTURE_POS_Y, 0.0f);
	pTitle->col = TEXTURE_COLOR;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL
	);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pTitle->pos.x - TEXTURE_SCALE_X / 2, pTitle->pos.y - TEXTURE_SCALE_Y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pTitle->pos.x + TEXTURE_SCALE_X / 2, pTitle->pos.y - TEXTURE_SCALE_Y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pTitle->pos.x - TEXTURE_SCALE_X / 2, pTitle->pos.y + TEXTURE_SCALE_Y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pTitle->pos.x + TEXTURE_SCALE_X / 2, pTitle->pos.y + TEXTURE_SCALE_Y / 2, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = TEXTURE_COLOR;
	pVtx[1].col = TEXTURE_COLOR;
	pVtx[2].col = TEXTURE_COLOR;
	pVtx[3].col = TEXTURE_COLOR;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();

	PlaySound(SOUND_LABEL_BGM003);
}
//=====================================================================
// 終了処理
//=====================================================================
void UninitTitle(void)
{
	// テクスチャの破棄
	if (g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateTitle(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadPress(JOYKEY_START))
	{
		SetFade(MODE_STAGE1);
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitle);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
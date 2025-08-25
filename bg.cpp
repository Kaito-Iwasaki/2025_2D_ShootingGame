//=====================================================================
// 
// 背景処理 [bg.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "bg.h"
#include "player.h"

// マクロ定義
#define TEXTURE1_FILENAME		"data\\TEXTURE\\bg100.png"
#define TEXTURE2_FILENAME		"data\\TEXTURE\\bg101.png"
#define TEXTURE3_FILENAME		"data\\TEXTURE\\bg102.png"
#define TEXTURE_POS_X			(SCREEN_WIDTH / 2)
#define TEXTURE_POS_Y			(SCREEN_HEIGHT / 2)
#define TEXTURE_SIZE			(2)
#define TEXTURE_SCALE_X			(SCREEN_WIDTH * TEXTURE_SIZE)
#define TEXTURE_SCALE_Y			(SCREEN_HEIGHT * TEXTURE_SIZE)

#define NUM_BG		(3)
#define BG_COLOR	D3DCOLOR_RGBA(255, 255, 255, 255)

// グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureBG[NUM_BG] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;				// 頂点バッファへのポインタ
Background g_aBG[NUM_BG];

//=====================================================================
// 初期化処理
//=====================================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ
	Background* pBG = &g_aBG[0];
	int nCntBG;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE3_FILENAME,
		&g_apTextureBG[0]
	);
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE2_FILENAME,
		&g_apTextureBG[1]
	);
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE1_FILENAME,
		&g_apTextureBG[2]
	);


	// 弾の情報の初期化
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		pBG[nCntBG].pos = D3DXVECTOR3(TEXTURE_POS_X, TEXTURE_POS_Y, 0.0f);
		pBG[nCntBG].fMove = 1.0f * (nCntBG + 1);
		pBG[nCntBG].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL
	);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++, pBG++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pBG->pos.x - TEXTURE_SCALE_X / 2, pBG->pos.y - TEXTURE_SCALE_Y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pBG->pos.x + TEXTURE_SCALE_X / 2, pBG->pos.y - TEXTURE_SCALE_Y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pBG->pos.x - TEXTURE_SCALE_X / 2, pBG->pos.y + TEXTURE_SCALE_Y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pBG->pos.x + TEXTURE_SCALE_X / 2, pBG->pos.y + TEXTURE_SCALE_Y / 2, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = BG_COLOR;
		pVtx[1].col = BG_COLOR;
		pVtx[2].col = BG_COLOR;
		pVtx[3].col = BG_COLOR;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f * TEXTURE_SIZE, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f * TEXTURE_SIZE);
		pVtx[3].tex = D3DXVECTOR2(1.0f * TEXTURE_SIZE, 1.0f * TEXTURE_SIZE);

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBG->Unlock();
}
//=====================================================================
// 終了処理
//=====================================================================
void UninitBG(void)
{
	int nCntBG;

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		// テクスチャの破棄
		if (g_apTextureBG[nCntBG] != NULL)
		{
			g_apTextureBG[nCntBG]->Release();
			g_apTextureBG[nCntBG] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateBG(void)
{
	VERTEX_2D* pVtx;
	Background* pBG = &g_aBG[0];
	int nCntBG;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++, pBG++)
	{
		// プレイヤーの位置を取得
		Player* pPlayer = GetPlayer();

		// 背景の位置を計算
		pBG->pos.x = (SCREEN_WIDTH - pPlayer->pos.x) * pBG->fMove * 0.3;
		pBG->pos.y += pBG->fMove;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pBG->pos.x - TEXTURE_SCALE_X / 2, pBG->pos.y - TEXTURE_SCALE_Y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pBG->pos.x + TEXTURE_SCALE_X / 2, pBG->pos.y - TEXTURE_SCALE_Y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pBG->pos.x - TEXTURE_SCALE_X / 2, pBG->pos.y + TEXTURE_SCALE_Y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pBG->pos.x + TEXTURE_SCALE_X / 2, pBG->pos.y + TEXTURE_SCALE_Y / 2, 0.0f);

		if (pBG->pos.y >= SCREEN_HEIGHT)
		{// 背景が下に行ったら上に戻してループさせる
			pBG->pos.y = 0;
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBG->Unlock();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBG;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureBG[nCntBG]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * 4, 2);
	}
}
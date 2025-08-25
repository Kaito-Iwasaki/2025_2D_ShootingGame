//=====================================================================
// 
// 体力バー [Healthbar.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "healthbar.h"
#include "player.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define INIT_SIZE_X		(128.0f)
#define INIT_SIZE_Y		(16.0f)
#define INIT_POS_X		(10)
#define INIT_POS_Y		(SCREEN_HEIGHT - INIT_SIZE_Y - 10)

#define COLOR_REMAIN	D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_DAMAGE	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHealthbar = NULL;		// 頂点バッファへのポインタ
HEALTHBAR g_aHealthbar[2];
float g_fLengthHealthbar;				// 対角線の長さ
float g_fAngleHealthbar;				// 対角線の角度

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************


//=====================================================================
// 
// ***** 初期化処理 *****
// 
//=====================================================================
void InitHealthbar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;
	HEALTHBAR* pHealthbar = &g_aHealthbar[0];

	memset(pHealthbar, 0, sizeof(HEALTHBAR) * 2);
	for (int nCount = 0; nCount < 2; nCount++)
	{
		pHealthbar[nCount].pos = { INIT_POS_X, INIT_POS_Y, 0 };
		pHealthbar[nCount].size = { INIT_SIZE_X, INIT_SIZE_Y, 0 };
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHealthbar,
		NULL
	);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHealthbar->Lock(0, 0, (void**)&pVtx, 0);

	pHealthbar = &g_aHealthbar[0];

	for (int nCount = 0; nCount < 2; nCount++, pHealthbar++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pHealthbar->pos.x, pHealthbar->pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pHealthbar->pos.x + pHealthbar->size.x, pHealthbar->pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pHealthbar->pos.x, pHealthbar->pos.y + pHealthbar->size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pHealthbar->pos.x + pHealthbar->size.x, pHealthbar->pos.y + pHealthbar->size.y, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		if (nCount == 0)
		{
			pVtx[0].col = COLOR_DAMAGE;
			pVtx[1].col = COLOR_DAMAGE;
			pVtx[2].col = COLOR_DAMAGE;
			pVtx[3].col = COLOR_DAMAGE;
		}
		else if (nCount == 1)
		{
			pVtx[0].col = COLOR_REMAIN;
			pVtx[1].col = COLOR_REMAIN;
			pVtx[2].col = COLOR_REMAIN;
			pVtx[3].col = COLOR_REMAIN;
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffHealthbar->Unlock();
}

//=====================================================================
// 
// ***** 終了処理 *****
// 
//=====================================================================
void UninitHealthbar(void)
{
	if (g_pVtxBuffHealthbar != NULL)
	{
		g_pVtxBuffHealthbar->Release();
		g_pVtxBuffHealthbar = NULL;
	}
}

//=====================================================================
// 
// ***** 更新処理 *****
// 
//=====================================================================
void UpdateHealthbar(void)
{
	Player* pPlayer = GetPlayer();
	VERTEX_2D* pVtx;
	HEALTHBAR* pHealthbar = &g_aHealthbar[0];


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHealthbar->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pHealthbar->pos.x, pHealthbar->pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pHealthbar->pos.x + pHealthbar->size.x, pHealthbar->pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pHealthbar->pos.x, pHealthbar->pos.y + pHealthbar->size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pHealthbar->pos.x + pHealthbar->size.x, pHealthbar->pos.y + pHealthbar->size.y, 0.0f);

	pHealthbar++;
	pVtx += 4;

	float fSizeX = INIT_SIZE_X * (pPlayer->fLife / 3.0f);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pHealthbar->pos.x, pHealthbar->pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pHealthbar->pos.x + fSizeX, pHealthbar->pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pHealthbar->pos.x, pHealthbar->pos.y + pHealthbar->size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pHealthbar->pos.x + fSizeX, pHealthbar->pos.y + pHealthbar->size.y, 0.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffHealthbar->Unlock();
}

//=====================================================================
// 
// ***** 描画処理 *****
// 
//=====================================================================
void DrawHealthbar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHealthbar, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < 2; nCount++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}
}
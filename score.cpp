//=====================================================================
// 
// 爆発エフェクト処理 [explosion.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "score.h"
#include <math.h>

// マクロ定義
#define TEXTURE_FILENAME			"data\\TEXTURE\\number000.png"
#define TEXTURE_SCALE_X				(16.0f)
#define TEXTURE_SCALE_Y				(32.0f)
#define TEXTURE_COLOR				D3DCOLOR_RGBA(255, 255, 255, 255)
#define TEXTURE_MAX_X				(10)									// テクスチャパターン数X
#define TEXTURE_MAX_Y				(1)										// テクスチャパターン数Y
#define TEXTURE_MAX_PATTERN			(TEXTURE_MAX_X * TEXTURE_MAX_Y)			// テクスチャ総パターン数
#define TEXTURE_INTERVAL			(3)										// パターン切り替えフレーム数

// グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// 頂点バッファへのポインタ
D3DXVECTOR3 g_posScore;
int g_nScore;

//=====================================================================
// 初期化処理
//=====================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	int nCntScore;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FILENAME, &g_pTextureScore);

	// スコアの情報の初期化
	g_posScore = D3DXVECTOR3(10.0f, 10.0f, 0.0f);
	g_nScore = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL
	);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x + (nCntScore * TEXTURE_SCALE_X), g_posScore.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x + ((nCntScore + 1) * TEXTURE_SCALE_X), g_posScore.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x + (nCntScore * TEXTURE_SCALE_X), g_posScore.y + TEXTURE_SCALE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x + ((nCntScore + 1) * TEXTURE_SCALE_X), g_posScore.y + TEXTURE_SCALE_Y, 0.0f);

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
		pVtx[0].tex.x = 0.0f;
		pVtx[0].tex.y = 0.0f;

		pVtx[1].tex.x = 1.0f / TEXTURE_MAX_X;
		pVtx[1].tex.y = 0.0f;

		pVtx[2].tex.x = 0.0f;
		pVtx[2].tex.y = 1.0f / TEXTURE_MAX_Y;

		pVtx[3].tex.x = 1.0f / TEXTURE_MAX_X;
		pVtx[3].tex.y = 1.0f / TEXTURE_MAX_Y;

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitScore(void)
{
	// テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateScore(void)
{

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//=====================================================================
// スコアの設定処理
//=====================================================================
void SetScore(int nScore)
{
	VERTEX_2D* pVtx;
	int aTexU[NUM_PLACE];
	int nCntScore;

	g_nScore = nScore;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		aTexU[nCntScore] = g_nScore % (int)pow(10, (double)NUM_PLACE - nCntScore) / (int)pow(10, (double)NUM_PLACE - 1 - nCntScore);

		// テクスチャ座標の設定
		pVtx[0].tex.x = 1.0f / TEXTURE_MAX_X * aTexU[nCntScore];
		pVtx[0].tex.y = 0.0f;

		pVtx[1].tex.x = 1.0f / TEXTURE_MAX_X * (aTexU[nCntScore] + 1);
		pVtx[1].tex.y = 0.0f;

		pVtx[2].tex.x = 1.0f / TEXTURE_MAX_X * aTexU[nCntScore];
		pVtx[2].tex.y = 1.0f;

		pVtx[3].tex.x = 1.0f / TEXTURE_MAX_X * (aTexU[nCntScore] + 1);
		pVtx[3].tex.y = 1.0f;

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();
}

//=====================================================================
// スコアの加算処理
//=====================================================================
void AddScore(int nValue)
{
	VERTEX_2D* pVtx;
	int aTexU[NUM_PLACE];
	int nCntScore;

	g_nScore += nValue;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		aTexU[nCntScore] = g_nScore % (int)pow(10, (double)NUM_PLACE - nCntScore) / (int)pow(10, (double)NUM_PLACE - 1 - nCntScore);

		// テクスチャ座標の設定
		pVtx[0].tex.x = 1.0f / TEXTURE_MAX_X * aTexU[nCntScore];
		pVtx[0].tex.y = 0.0f;

		pVtx[1].tex.x = 1.0f / TEXTURE_MAX_X * (aTexU[nCntScore] + 1);
		pVtx[1].tex.y = 0.0f;

		pVtx[2].tex.x = 1.0f / TEXTURE_MAX_X * aTexU[nCntScore];
		pVtx[2].tex.y = 1.0f;

		pVtx[3].tex.x = 1.0f / TEXTURE_MAX_X * (aTexU[nCntScore] + 1);
		pVtx[3].tex.y = 1.0f;

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();
}

int GetScore(void)
{
	return g_nScore;
}
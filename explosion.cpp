//=====================================================================
// 
// 爆発エフェクト処理 [explosion.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "explosion.h"

// マクロ定義
#define TEXTURE_FILENAME			"data\\TEXTURE\\explosion000.png"
#define TEXTURE_SCALE_X				(64.0f)
#define TEXTURE_SCALE_Y				(64.0f)
#define TEXTURE_COLOR				D3DCOLOR_RGBA(255, 255, 0, 255)
#define TEXTURE_MAX_X				(5)										// テクスチャパターン数X
#define TEXTURE_MAX_Y				(1)										// テクスチャパターン数Y
#define TEXTURE_MAX_PATTERN			(TEXTURE_MAX_X * TEXTURE_MAX_Y)			// テクスチャ総パターン数
#define TEXTURE_INTERVAL			(5)										// パターン切り替えフレーム数

// グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureExplosion= NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion= NULL;		// 頂点バッファへのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];

float g_fLengthExplosion;				// 対角線の長さ
float g_fAngleExplosion;				// 対角線の角度

//=====================================================================
// 初期化処理
//=====================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	Explosion* pExplosion = &g_aExplosion[0];
	int nCntExplosion;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FILENAME, &g_pTextureExplosion);

	// 弾の情報の初期化
	for (nCntExplosion= 0; nCntExplosion< MAX_EXPLOSION; nCntExplosion++)
	{
		pExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pExplosion[nCntExplosion].col = TEXTURE_COLOR;
		pExplosion[nCntExplosion].nCountAnim = 1;
		pExplosion[nCntExplosion].nPatternAnim = 0;
		pExplosion[nCntExplosion].bUsed = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL
	);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion= 0; nCntExplosion< MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pExplosion->pos.x - TEXTURE_SCALE_X / 2, pExplosion->pos.y - TEXTURE_SCALE_X / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pExplosion->pos.x + TEXTURE_SCALE_X / 2, pExplosion->pos.y - TEXTURE_SCALE_X / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pExplosion->pos.x - TEXTURE_SCALE_X / 2, pExplosion->pos.y + TEXTURE_SCALE_X / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pExplosion->pos.x + TEXTURE_SCALE_X / 2, pExplosion->pos.y + TEXTURE_SCALE_X / 2, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = pExplosion->col;
		pVtx[1].col = pExplosion->col;
		pVtx[2].col = pExplosion->col;
		pVtx[3].col = pExplosion->col;

		// テクスチャ座標の設定
		pVtx[0].tex.x = 1.0f / TEXTURE_MAX_X * pExplosion->nPatternAnim;
		pVtx[0].tex.y = 1.0f / TEXTURE_MAX_Y * (pExplosion->nPatternAnim / TEXTURE_MAX_X);

		pVtx[1].tex.x = 1.0f / TEXTURE_MAX_X * (pExplosion->nPatternAnim + 1);
		pVtx[1].tex.y = 1.0f / TEXTURE_MAX_Y * (pExplosion->nPatternAnim / TEXTURE_MAX_X);

		pVtx[2].tex.x = 1.0f / TEXTURE_MAX_X * pExplosion->nPatternAnim;
		pVtx[2].tex.y = 1.0f / TEXTURE_MAX_Y * (pExplosion->nPatternAnim / TEXTURE_MAX_X + 1);

		pVtx[3].tex.x = 1.0f / TEXTURE_MAX_X * (pExplosion->nPatternAnim + 1);
		pVtx[3].tex.y = 1.0f / TEXTURE_MAX_Y * (pExplosion->nPatternAnim / TEXTURE_MAX_X + 1);

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitExplosion(void)
{
	// テクスチャの破棄
	if (g_pTextureExplosion!= NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion= NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffExplosion!= NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateExplosion(void)
{
	int nCntExplosion;
	VERTEX_2D* pVtx;
	Explosion* pExplosion= &g_aExplosion[0];

	// 対角線の長さを算出
	g_fLengthExplosion= sqrtf(TEXTURE_SCALE_X * TEXTURE_SCALE_X + TEXTURE_SCALE_Y * TEXTURE_SCALE_X) * 0.5;

	// 対角線の角度を算出
	g_fAngleExplosion= atan2f(TEXTURE_SCALE_X, TEXTURE_SCALE_Y);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion= 0; nCntExplosion< MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUsed == true)
		{// 使用中であれば
			pExplosion->nCountAnim++;

			if (pExplosion->nCountAnim % TEXTURE_INTERVAL == 0)
			{// パターン更新
				pExplosion->nPatternAnim++;

				// テクスチャ座標の設定
				pVtx[0].tex.x = 1.0f / TEXTURE_MAX_X * pExplosion->nPatternAnim;
				pVtx[0].tex.y = 1.0f / TEXTURE_MAX_Y * (pExplosion->nPatternAnim / TEXTURE_MAX_X);

				pVtx[1].tex.x = 1.0f / TEXTURE_MAX_X * (pExplosion->nPatternAnim + 1);
				pVtx[1].tex.y = 1.0f / TEXTURE_MAX_Y * (pExplosion->nPatternAnim / TEXTURE_MAX_X);

				pVtx[2].tex.x = 1.0f / TEXTURE_MAX_X * pExplosion->nPatternAnim;
				pVtx[2].tex.y = 1.0f / TEXTURE_MAX_Y * (pExplosion->nPatternAnim / TEXTURE_MAX_X + 1);

				pVtx[3].tex.x = 1.0f / TEXTURE_MAX_X * (pExplosion->nPatternAnim + 1);
				pVtx[3].tex.y = 1.0f / TEXTURE_MAX_Y * (pExplosion->nPatternAnim / TEXTURE_MAX_X + 1);

				if (pExplosion->nPatternAnim >= TEXTURE_MAX_PATTERN)
				{// 総パターン数を超えたら
					pExplosion->bUsed = false;
				}
			}

		}
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (nCntExplosion = 0; nCntExplosion< MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUsed == true)
		{
			 //ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion* 4, 2);
		}
	}
}

//=====================================================================
// 爆発の設定処理
//=====================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCntExplosion;
	VERTEX_2D* pVtx;
	Explosion* pExplosion = &g_aExplosion[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion= 0; nCntExplosion< MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUsed == false)
		{
			pExplosion->pos = pos;
			pExplosion->col = col;
			pExplosion->nCountAnim = 1;
			pExplosion->nPatternAnim = 1;
			pExplosion->bUsed = true;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(pExplosion->pos.x - TEXTURE_SCALE_X / 2, pExplosion->pos.y - TEXTURE_SCALE_X / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pExplosion->pos.x + TEXTURE_SCALE_X / 2, pExplosion->pos.y - TEXTURE_SCALE_X / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pExplosion->pos.x - TEXTURE_SCALE_X / 2, pExplosion->pos.y + TEXTURE_SCALE_X / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pExplosion->pos.x + TEXTURE_SCALE_X / 2, pExplosion->pos.y + TEXTURE_SCALE_X / 2, 0.0f);


			// 頂点カラーの設定
			pVtx[0].col = pExplosion->col;
			pVtx[1].col = pExplosion->col;
			pVtx[2].col = pExplosion->col;
			pVtx[3].col = pExplosion->col;

			break;
		}
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}
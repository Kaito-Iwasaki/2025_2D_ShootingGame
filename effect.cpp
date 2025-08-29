//=====================================================================
// 
// エフェクト処理 [effect.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "effect.h"

//*******************************************
// マクロ定義
//*******************************************
#define DEF_TEXTURE_FILENAME			"data\\TEXTURE\\effect000.jpg"			// テクスチャのファイル名
#define DEF_TEXTURE_SCALE_X				(64.0f)									// テクスチャのサイズX
#define DEF_TEXTURE_SCALE_Y				(64.0f)									// テクスチャのサイズY
#define DEF_TEXTURE_COLOR				D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f)		// テクスチャの色

#define DEF_EFFECT_LIFETIME				(400.0f)								// エフェクトが残る時間
#define DEF_EFFECT_SIZE					(30.0f)									// エフェクトのサイズ倍率

//*******************************************
// グローバル変数宣言
//*******************************************
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// 頂点バッファへのポインタ
Effect g_aEffect[MAX_EFFECT];

float g_fLengthEffect;				// 対角線の長さ
float g_fAngleEffect;				// 対角線の角度

//=====================================================================
// 初期化処理
//=====================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	Effect* pEffect = &g_aEffect[0];

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, DEF_TEXTURE_FILENAME, &g_pTextureEffect);

	// エフェクトの情報の初期化
	ZeroMemory(pEffect, sizeof(pEffect) * MAX_EFFECT);
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//strcpy(pEffect[nCntEffect].cTextureFilePath, DEF_TEXTURE_FILENAME);

		pEffect[nCntEffect].pos = D3DXVECTOR3_ZERO;
		pEffect[nCntEffect].fRot = 0.0f;
		pEffect[nCntEffect].size = { DEF_TEXTURE_SCALE_X, DEF_TEXTURE_SCALE_Y, 0.0f };
		pEffect[nCntEffect].fScale = 0.0f;

		pEffect[nCntEffect].col = DEF_TEXTURE_COLOR;
		pEffect[nCntEffect].fRad = 0.0f;
		pEffect[nCntEffect].fLifeTime = DEF_EFFECT_LIFETIME;
		pEffect[nCntEffect].fMaxLifeTime = DEF_EFFECT_LIFETIME;
		pEffect[nCntEffect].bUseTexture = true;
		pEffect[nCntEffect].bUsed = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL
	);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		// 頂点座標の設定
		pVtx[0].pos.x = pEffect->pos.x + sin(0 + g_fAngleEffect) * g_fLengthEffect;
		pVtx[0].pos.y = pEffect->pos.y + cos(0 + g_fAngleEffect) * g_fLengthEffect;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = pEffect->pos.x + sin(0 - g_fAngleEffect) * g_fLengthEffect;
		pVtx[1].pos.y = pEffect->pos.y + cos(0 - g_fAngleEffect) * g_fLengthEffect;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = pEffect->pos.x + sin(D3DX_PI - g_fAngleEffect) * g_fLengthEffect;
		pVtx[2].pos.y = pEffect->pos.y + cos(D3DX_PI - g_fAngleEffect) * g_fLengthEffect;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = pEffect->pos.x + sin(D3DX_PI + g_fAngleEffect) * g_fLengthEffect;
		pVtx[3].pos.y = pEffect->pos.y + cos(D3DX_PI + g_fAngleEffect) * g_fLengthEffect;
		pVtx[3].pos.z = 0.0f;

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = pEffect->col;
		pVtx[1].col = pEffect->col;
		pVtx[2].col = pEffect->col;
		pVtx[3].col = pEffect->col;

		// テクスチャ座標の設定
		pVtx[0].tex.x = 0.0f;
		pVtx[0].tex.y = 0.0f;

		pVtx[1].tex.x = 1.0f;
		pVtx[1].tex.y = 0.0f;

		pVtx[2].tex.x = 0.0f;
		pVtx[2].tex.y = 1.0f;

		pVtx[3].tex.x = 1.0f;
		pVtx[3].tex.y = 1.0f;

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitEffect(void)
{
	// テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateEffect(void)
{
	int nCntEffect;
	VERTEX_2D* pVtx;
	Effect* pEffect = &g_aEffect[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUsed == true)
		{
			// 値の設定
			pEffect->fLifeTime--;
			pEffect->pos.x = pEffect->pos.x + sin(pEffect->fRad) * pEffect->fSpeed;
			pEffect->pos.y = pEffect->pos.y + cos(pEffect->fRad) * pEffect->fSpeed;
			pEffect->fScale = pEffect->fMaxScale * (pEffect->fLifeTime / pEffect->fMaxLifeTime);
			pEffect->col.a = pEffect->fMaxAlpha * (pEffect->fLifeTime / pEffect->fMaxLifeTime);

			// 対角線の長さを算出
			g_fLengthEffect = sqrtf(pEffect->size.x * pEffect->size.x + pEffect->size.y * pEffect->size.y) * 0.5 * pEffect->fScale;

			// 対角線の角度を算出
			g_fAngleEffect = atan2f(pEffect->size.x, pEffect->size.y);

			// 頂点座標の設定
			pVtx[0].pos.x = pEffect->pos.x + sin(pEffect->fRot + 0.0f + g_fAngleEffect) * g_fLengthEffect;
			pVtx[0].pos.y = pEffect->pos.y + cos(pEffect->fRot + 0.0f + g_fAngleEffect) * g_fLengthEffect;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = pEffect->pos.x + sin(pEffect->fRot + 0.0f - g_fAngleEffect) * g_fLengthEffect;
			pVtx[1].pos.y = pEffect->pos.y + cos(pEffect->fRot + 0.0f - g_fAngleEffect) * g_fLengthEffect;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = pEffect->pos.x + sin(pEffect->fRot + D3DX_PI - g_fAngleEffect) * g_fLengthEffect;
			pVtx[2].pos.y = pEffect->pos.y + cos(pEffect->fRot + D3DX_PI - g_fAngleEffect) * g_fLengthEffect;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = pEffect->pos.x + sin(pEffect->fRot + D3DX_PI + g_fAngleEffect) * g_fLengthEffect;
			pVtx[3].pos.y = pEffect->pos.y + cos(pEffect->fRot + D3DX_PI + g_fAngleEffect) * g_fLengthEffect;
			pVtx[3].pos.z = 0.0f;

			// 頂点カラーの設定
			pVtx[0].col = pEffect->col;
			pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;
			pVtx[3].col = pEffect->col;

			if (pEffect->fLifeTime == 0)
			{// エフェクトの寿命が尽きたら消す
				pEffect->bUsed = false;
			}
		}
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUsed == true)
		{
			// テクスチャの設定
			if (g_aEffect[nCntEffect].bUseTexture)
			{
				pDevice->SetTexture(0, g_pTextureEffect);
			}
			else
			{
				pDevice->SetTexture(0, NULL);
			}

			 //ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=====================================================================
// エフェクトの設定処理
//=====================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRad, float fSpeed, float fScale, float fLifeTime, bool bUseTexture)
{
	VERTEX_2D* pVtx;
	Effect* pEffect = &g_aEffect[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUsed == false)
		{
			// 対角線の長さを算出
			g_fLengthEffect = sqrtf(pEffect->size.x * pEffect->size.x + pEffect->size.y * pEffect->size.y) * 0.5 * pEffect->fScale;

			// 対角線の角度を算出
			g_fAngleEffect = atan2f(pEffect->size.x, pEffect->size.y);

			// 位置の更新
			pEffect->pos = pos;
			pEffect->fRot = 0.0f;
			pEffect->size = { DEF_TEXTURE_SCALE_X, DEF_TEXTURE_SCALE_Y, 0.0f };
			pEffect->fScale = fScale;
			pEffect->fMaxScale = fScale;

			pEffect->col = col;
			pEffect->fMaxAlpha = col.a;
			pEffect->fRad = fRad;
			pEffect->fSpeed = fSpeed;
			pEffect->fLifeTime = fLifeTime;
			pEffect->fMaxLifeTime = fLifeTime;
			pEffect->bUseTexture = bUseTexture;
			pEffect->bUsed = true;

			// 頂点座標の設定
			pVtx[0].pos.x = pEffect->pos.x + sin(pEffect->fRot + 0.0f + g_fAngleEffect) * g_fLengthEffect;
			pVtx[0].pos.y = pEffect->pos.y + cos(pEffect->fRot + 0.0f + g_fAngleEffect) * g_fLengthEffect;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = pEffect->pos.x + sin(pEffect->fRot + 0.0f - g_fAngleEffect) * g_fLengthEffect;
			pVtx[1].pos.y = pEffect->pos.y + cos(pEffect->fRot + 0.0f - g_fAngleEffect) * g_fLengthEffect;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = pEffect->pos.x + sin(pEffect->fRot + D3DX_PI - g_fAngleEffect) * g_fLengthEffect;
			pVtx[2].pos.y = pEffect->pos.y + cos(pEffect->fRot + D3DX_PI - g_fAngleEffect) * g_fLengthEffect;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = pEffect->pos.x + sin(pEffect->fRot + D3DX_PI + g_fAngleEffect) * g_fLengthEffect;
			pVtx[3].pos.y = pEffect->pos.y + cos(pEffect->fRot + D3DX_PI + g_fAngleEffect) * g_fLengthEffect;
			pVtx[3].pos.z = 0.0f;

			// 頂点カラーの設定
			pVtx[0].col = pEffect->col;
			pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;
			pVtx[3].col = pEffect->col;

			break;
		}
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

void ClearAllEffect(void)
{
	Effect* pEffect = &g_aEffect[0];

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		pEffect->bUsed = false;
	}
}
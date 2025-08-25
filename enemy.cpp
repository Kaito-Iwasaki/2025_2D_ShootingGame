//=====================================================================
// 
// プレイヤーの弾処理 [enemy.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "enemy.h"
#include "explosion.h"
#include "player.h"
#include "bullet.h"
#include "score.h"
#include "sound.h"
#include "collision.h"
#include "util.h"

// マクロ定義
#define TEXTURE1_FILENAME			"data\\TEXTURE\\enemy000.png"		// 敵1のテクスチャ
#define TEXTURE2_FILENAME			"data\\TEXTURE\\enemy001.png"		// 敵2のテクスチャ
#define TEXTURE3_FILENAME			"data\\TEXTURE\\enemy002.png"		// 敵3のテクスチャ
#define TEXTURE4_FILENAME			"data\\TEXTURE\\enemy003.png"		// 敵4のテクスチャ
#define TEXTURE_SCALE_X				(48.0f)
#define TEXTURE_SCALE_Y				(48.0f)
#define TEXTURE_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define TEXTURE_COLOR_DAMAGED		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)

#define NUM_ENEMY					(4)		// 敵の種類

//*********************************************************************
// プロトタイプ宣言
//*********************************************************************
void EnemyAction(ENEMY* pEnemy);

// グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				// 頂点バッファへのポインタ
ENEMY g_aEnemy[MAX_ENEMY];									// 敵情報

float g_fLengthEnemy;				// 対角線の長さ
float g_fAngleEnemy;				// 対角線の角度

//=====================================================================
// 初期化処理
//=====================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	ENEMY* pEnemy = &g_aEnemy[0];
	int nCntEnemy;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE1_FILENAME, &g_apTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE2_FILENAME, &g_apTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE3_FILENAME, &g_apTextureEnemy[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE4_FILENAME, &g_apTextureEnemy[3]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL
	);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0, pEnemy = &g_aEnemy[0]; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		pEnemy->pos = D3DXVECTOR3_ZERO;
		pEnemy->posOffset = D3DXVECTOR3_ZERO;
		pEnemy->move = D3DXVECTOR3_ZERO;
		pEnemy->nType = 0;
		pEnemy->size = D3DXVECTOR3(TEXTURE_SCALE_X, TEXTURE_SCALE_Y, 0.0f);
		pEnemy->bUsed = false;
		pEnemy->nCounterState = 0;
		pEnemy->nCounterStep = 0;
		pEnemy->nShotInterval = 0;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->size.x / 2, pEnemy->pos.y - pEnemy->size.y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->size.x / 2, pEnemy->pos.y - pEnemy->size.y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->size.x / 2, pEnemy->pos.y + pEnemy->size.y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->size.x / 2, pEnemy->pos.y + pEnemy->size.y / 2, 0.0f);

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

		pVtx[1].tex.x = 1.0f;
		pVtx[1].tex.y = 0.0f;

		pVtx[2].tex.x = 0.0f;
		pVtx[2].tex.y = 1.0f;

		pVtx[3].tex.x = 1.0f;
		pVtx[3].tex.y = 1.0f;

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitEnemy(void)
{
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		// テクスチャの破棄
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateEnemy(void)
{
	VERTEX_2D* pVtx;
	ENEMY* pEnemy = &g_aEnemy[0];
	Player* pPlayer = GetPlayer();

	float fDirection;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUsed == true)
		{
			// プレイヤーとの衝突判定
			if (Collision(pEnemy->pos + pEnemy->posOffset, pEnemy->size, pPlayer->pos, pPlayer->size))
			{
				HitPlayer(1);
			}

			switch (pEnemy->nType)
			{
			case 0:
				pEnemy->posOffset.x = sin(pEnemy->nCounterStep / pEnemy->move.x) * 200.0f;
				pEnemy->pos.y += pEnemy->move.y;
				pEnemy->nCounterStep++;

				if (pEnemy->pos.y > SCREEN_HEIGHT + pEnemy->size.y / 2)
				{
					pEnemy->bUsed = false;
				}
				break;

			case 1:
				fDirection = atan2f(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);

				pEnemy->pos += D3DXVECTOR3(sin(fDirection), cos(fDirection), 0.0f) * 4.0f;

				break;

			case 2:
				pEnemy->nCounterUState++;

				if (pEnemy->pos.y < 100)
				{
					pEnemy->pos.y += 3;
				}

				if (pEnemy->nCounterUState % 100 == 0)
				{
					fDirection = atan2f(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);

					SetBullet(
						pEnemy->pos + pEnemy->posOffset,
						5.0f,
						fDirection - 0.5f,
						BULLETTYPE_ENEMY,
						BULLETPATTERN_SPHERE
					);
					SetBullet(
						pEnemy->pos + pEnemy->posOffset,
						5.0f,
						fDirection,
						BULLETTYPE_ENEMY,
						BULLETPATTERN_SPHERE
					);
					SetBullet(
						pEnemy->pos + pEnemy->posOffset,
						5.0f,
						fDirection + 0.5f,
						BULLETTYPE_ENEMY,
						BULLETPATTERN_SPHERE
					);
				}
				break;

			case 4:
				pEnemy->nCounterUState++;

				if (pEnemy->nCounterUState % 100 == 0)
				{
					fDirection = atan2f(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);

					SetBullet(
						pEnemy->pos + pEnemy->posOffset,
						3.0f,
						fDirection,
						BULLETTYPE_ENEMY,
						BULLETPATTERN_SPHERE
					);
				}

				pEnemy->pos.y += pEnemy->move.y;

				if (pEnemy->pos.y > SCREEN_HEIGHT + pEnemy->size.y / 2)
				{
					pEnemy->bUsed = false;
				}
				break;

			case 5:
				pEnemy->nCounterUState++;

				if (pEnemy->pos.y < 100)
				{
					pEnemy->pos.y += 3;
				}
				else
				{
					if (pEnemy->nCounterUState % 3 == 0)
					{
						SetBullet(
							pEnemy->pos + pEnemy->posOffset,
							RandRange(5,10),
							(float)RandRange(0, 314 * 2) * 0.01f,
							BULLETTYPE_ENEMY,
							BULLETPATTERN_SPHERE
						);
					};

					if (pEnemy->pos.x < SCREEN_WIDTH / 2 - 500 || pEnemy->pos.x > SCREEN_WIDTH / 2 + 500)
					{
						pEnemy->move *= -1;
					}

					pEnemy->pos.x += pEnemy->move.x;

					break;
				}
				break;
				
			default:
				pEnemy->pos.y += pEnemy->move.y;

				if (pEnemy->pos.y > SCREEN_HEIGHT + pEnemy->size.y / 2)
				{
					pEnemy->bUsed = false;
				}
			}

			switch (pEnemy->state)
			{
			case ENEMYSTATE_NORMAL:
				break;

			case ENEMYSTATE_DAMAGED:
				pEnemy->nCounterState--;

				if (pEnemy->nCounterState < 1)
				{
					pEnemy->state = ENEMYSTATE_NORMAL;

					// 頂点カラーの設定
					pVtx[0].col = TEXTURE_COLOR;
					pVtx[1].col = TEXTURE_COLOR;
					pVtx[2].col = TEXTURE_COLOR;
					pVtx[3].col = TEXTURE_COLOR;
				}
				break;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3((pEnemy->pos.x + pEnemy->posOffset.x) - pEnemy->size.x / 2, (pEnemy->pos.y + pEnemy->posOffset.y) - pEnemy->size.y / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((pEnemy->pos.x + pEnemy->posOffset.x) + pEnemy->size.x / 2, (pEnemy->pos.y + pEnemy->posOffset.y) - pEnemy->size.y / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((pEnemy->pos.x + pEnemy->posOffset.x) - pEnemy->size.x / 2, (pEnemy->pos.y + pEnemy->posOffset.y) + pEnemy->size.y / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((pEnemy->pos.x + pEnemy->posOffset.x) + pEnemy->size.x / 2, (pEnemy->pos.y + pEnemy->posOffset.y) + pEnemy->size.y / 2, 0.0f);
		}
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUsed == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nTex]);

			 //ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//=====================================================================
// 敵の設定処理
//=====================================================================
ENEMY* SetEnemy(D3DXVECTOR3 pos, int nType)
{
	VERTEX_2D* pVtx;
	ENEMY* pEnemy = &g_aEnemy[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUsed == false)
		{
			// 各値を初期化
			pEnemy->pos = pos;
			pEnemy->posOffset = D3DXVECTOR3_ZERO;
			pEnemy->move = { 5.0f, 4.0f, 0.0f };
			pEnemy->size = D3DXVECTOR3(TEXTURE_SCALE_X, TEXTURE_SCALE_Y, 0.0f);
			pEnemy->nType = nType;
			pEnemy->nLife = 1;
			pEnemy->state = ENEMYSTATE_NORMAL;
			pEnemy->nCounterState = 0;
			pEnemy->nCounterUState = 0;
			pEnemy->nCounterStep = 0;
			pEnemy->nShotInterval = 0;
			pEnemy->bUsed = true;

			switch (pEnemy->nType)
			{
			case 0:
				pEnemy->nTex = 0;
				break;

			case 1:
				pEnemy->nTex = 1;
				break;

			case 2:
				pEnemy->nTex = 2;
				break;

			case 3:
				pEnemy->nTex = 3;
				break;

			case 4:
				pEnemy->nTex = 2;
				break;

			case 5:
				pEnemy->nTex = 0;
				pEnemy->size *= 2;
			}

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3((pEnemy->pos.x + pEnemy->posOffset.x) - pEnemy->size.x / 2, (pEnemy->pos.y + pEnemy->posOffset.y) - pEnemy->size.y / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((pEnemy->pos.x + pEnemy->posOffset.x) + pEnemy->size.x / 2, (pEnemy->pos.y + pEnemy->posOffset.y) - pEnemy->size.y / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((pEnemy->pos.x + pEnemy->posOffset.x) - pEnemy->size.x / 2, (pEnemy->pos.y + pEnemy->posOffset.y) + pEnemy->size.y / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((pEnemy->pos.x + pEnemy->posOffset.x) + pEnemy->size.x / 2, (pEnemy->pos.y + pEnemy->posOffset.y) + pEnemy->size.y / 2, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = TEXTURE_COLOR;
			pVtx[1].col = TEXTURE_COLOR;
			pVtx[2].col = TEXTURE_COLOR;
			pVtx[3].col = TEXTURE_COLOR;

			break;
		}

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();

	return pEnemy;
}

//=====================================================================
// 敵のヒット処理
//=====================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife < 1)
	{
		SetExplosion(g_aEnemy[nCntEnemy].pos + g_aEnemy[nCntEnemy].posOffset, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
		g_aEnemy[nCntEnemy].bUsed = false;
		AddScore(100);
		PlaySound(SOUND_LABEL_SE_HIT000);
	}
	else if (g_aEnemy[nCntEnemy].state != ENEMYSTATE_DAMAGED)
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGED;
		g_aEnemy[nCntEnemy].nCounterState = 5;

		pVtx += (nCntEnemy * 4);

		// 頂点カラーの設定
		pVtx[0].col = TEXTURE_COLOR_DAMAGED;
		pVtx[1].col = TEXTURE_COLOR_DAMAGED;
		pVtx[2].col = TEXTURE_COLOR_DAMAGED;
		pVtx[3].col = TEXTURE_COLOR_DAMAGED;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}


//=====================================================================
// 敵情報の取得
//=====================================================================
ENEMY* GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=====================================================================
// 敵の残存数を取得
//=====================================================================
int GetEnemyLeft(void)
{
	int nCntEnemy;
	int nLeft = 0;

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUsed == true)
		{
			nLeft++;
		}
	}

	return nLeft;
}
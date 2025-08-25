//=====================================================================
// 
// プレイヤーの弾処理 [bullet.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "sound.h"

#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "score.h"
#include "effect.h"
#include "collision.h"

// マクロ定義
#define TEXTURE_FILENAME1			"data\\TEXTURE\\bullet000.png"				// テクスチャのファイル名
#define TEXTURE_FILENAME2			"data\\TEXTURE\\enemybullet000.png"		// テクスチャのファイル名
#define TEXTURE_FILENAME3			"data\\TEXTURE\\enemybullet001.png"		// テクスチャのファイル名
#define TEXTURE_SCALE_X				(8.0f)									// テクスチャのサイズX
#define TEXTURE_SCALE_Y				(64.0f)									// テクスチャのサイズY
#define TEXTURE_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)		// テクスチャの色

#define BULLET_LIFETIME				(400.0f)								// 弾が残る時間
#define BULLET_SIZE					(30.0f)									// 弾のサイズ倍率

// プロトタイプ宣言
void CollisionEnemy(Bullet* pBullet);
void CollisionPlayer(Bullet* pBullet);
void CollisionBullet(Bullet* pBullet);

// グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureBullet[BULLETPATTERN_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;				// 頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];

float g_fLengthBullet;				// 対角線の長さ
float g_fAngleBullet;				// 対角線の角度

//=====================================================================
// 初期化処理
//=====================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	Bullet* pBullet = &g_aBullet[0];
	int nCntBullet;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FILENAME1, &g_apTextureBullet[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FILENAME2, &g_apTextureBullet[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FILENAME3, &g_apTextureBullet[2]);

	// 弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		pBullet[nCntBullet].pos = D3DXVECTOR3_ZERO;
		pBullet[nCntBullet].size = { TEXTURE_SCALE_X, TEXTURE_SCALE_Y, 0.0f };
		pBullet[nCntBullet].fMove = 0.0f;
		pBullet[nCntBullet].fRot = 0.0f;
		pBullet[nCntBullet].nLifeTime = BULLET_LIFETIME;
		pBullet[nCntBullet].nType = BULLETTYPE_PLAYER;
		pBullet[nCntBullet].pattern = BULLETPATTERN_PLAYER;
		pBullet[nCntBullet].bUsed = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pBullet->pos.x - pBullet->size.x / 2, pBullet->pos.y - pBullet->size.y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pBullet->pos.x + pBullet->size.x / 2, pBullet->pos.y - pBullet->size.y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pBullet->pos.x - pBullet->size.x / 2, pBullet->pos.y + pBullet->size.y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pBullet->pos.x + pBullet->size.x / 2, pBullet->pos.y + pBullet->size.y / 2, 0.0f);

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
	g_pVtxBuffBullet->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitBullet(void)
{
	// テクスチャの破棄
	for (int nCntBullet = 0; nCntBullet < BULLETPATTERN_MAX; nCntBullet++)
	{
		if (g_apTextureBullet[nCntBullet] != NULL)
		{
			g_apTextureBullet[nCntBullet]->Release();
			g_apTextureBullet[nCntBullet] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateBullet(void)
{
	int nCntBullet;
	VERTEX_2D* pVtx;
	Bullet* pBullet = &g_aBullet[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUsed == true)
		{
			// 対角線の長さを算出
			g_fLengthBullet = sqrtf(pBullet->size.x * pBullet->size.x + pBullet->size.y * pBullet->size.y) * 0.25;

			// 対角線の角度を算出
			g_fAngleBullet = atan2f(pBullet->size.x, pBullet->size.y);

			// 位置の更新
			pBullet->pos.x += pBullet->fMove * sinf(pBullet->fRot);
			pBullet->pos.y += pBullet->fMove * cosf(pBullet->fRot);

			// 頂点座標の設定
			pVtx[0].pos.x = pBullet->pos.x + sin(pBullet->fRot + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.y = pBullet->pos.y + cos(pBullet->fRot + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = pBullet->pos.x + sin(pBullet->fRot - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.y = pBullet->pos.y + cos(pBullet->fRot - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = pBullet->pos.x + sin(pBullet->fRot + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.y = pBullet->pos.y + cos(pBullet->fRot + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = pBullet->pos.x + sin(pBullet->fRot + D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.y = pBullet->pos.y + cos(pBullet->fRot + D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.z = 0.0f;

			// 当たり判定処理
			switch (pBullet->nType)
			{
			case BULLETTYPE_PLAYER:
				CollisionEnemy(pBullet);
				CollisionBullet(pBullet);

				break;

			case BULLETTYPE_ENEMY:
				CollisionPlayer(pBullet);
				break;
			}

			pBullet->nLifeTime--;
			if (pBullet->pos.x < 0 || pBullet->pos.x > SCREEN_WIDTH || pBullet->pos.y < 0 || pBullet->pos.y > SCREEN_HEIGHT)
			{// 弾が画面外に来たら消す
				pBullet->bUsed = false;
			}
			else if (pBullet->nLifeTime == 0)
			{// 弾の寿命が尽きたら消す
				SetExplosion(pBullet->pos, D3DXCOLOR(100, 100, 100, 255));
				pBullet->bUsed = false;
			}
		}
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUsed == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureBullet[g_aBullet[nCntBullet].pattern]);

			 //ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//=====================================================================
// 弾の設定処理
//=====================================================================
void SetBullet(D3DXVECTOR3 pos, float fMove, float fRot, BULLETTYPE type, BULLETPATTERN pattern)
{
	VERTEX_2D* pVtx;
	Bullet* pBullet = &g_aBullet[0];
	int nCntBullet;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUsed == false)
		{
			// 位置の更新
			pBullet->pos = pos;
			pBullet->fMove = fMove;
			pBullet->fRot = fRot;
			pBullet->nLifeTime = BULLET_LIFETIME;
			pBullet->nType = type;
			pBullet->pattern = pattern;
			pBullet->bUsed = true;

			switch (pattern)
			{
			case BULLETPATTERN_PLAYER:
				pBullet->nLife = 2;
				pBullet->size = { TEXTURE_SCALE_X, TEXTURE_SCALE_Y, 0.0f };
				break;

			default:
				pBullet->nLife = 1;
				pBullet->size = { 24.0f, 24.0f, 0.0f };
				break;
			}

			// 頂点座標の設定
			pVtx[0].pos.x = pBullet->pos.x + sin(pBullet->fRot + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.y = pBullet->pos.y + cos(pBullet->fRot + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = pBullet->pos.x + sin(pBullet->fRot - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.y = pBullet->pos.y + cos(pBullet->fRot - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = pBullet->pos.x + sin(pBullet->fRot + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.y = pBullet->pos.y + cos(pBullet->fRot + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = pBullet->pos.x + sin(pBullet->fRot + D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.y = pBullet->pos.y + cos(pBullet->fRot + D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.z = 0.0f;

			break;
		}
		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

//=====================================================================
// 敵の当たり判定処理
//=====================================================================
void CollisionEnemy(Bullet* pBullet)
{
	// 敵の取得
	ENEMY* pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUsed == true)
		{
			if (Collision(pBullet->pos, pBullet->size, pEnemy->pos + pEnemy->posOffset, pEnemy->size))
			{// 敵に弾が当たった
				HitEnemy(nCntEnemy, 1);

				pBullet->bUsed = false;
			}
		}
	}
}

//=====================================================================
// プレイヤーの当たり判定処理
//=====================================================================
void CollisionPlayer(Bullet* pBullet)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	if (IsPlayerDead() == true)
	{
		return;
	}

	if (Collision(pBullet->pos, pBullet->size, pPlayer->pos, pPlayer->size))
	{// プレイヤーに弾が当たった
		HitPlayer(1);

		pBullet->bUsed = false;
	}
}

//=====================================================================
// 弾と弾の当たり判定処理
//=====================================================================
void CollisionBullet(Bullet* pBullet)
{
	// 敵の取得
	Bullet* pEnemyBullet = &g_aBullet[0];

	for (int nCntBullet = 0; nCntBullet < MAX_ENEMY; nCntBullet++, pEnemyBullet++)
	{
		if (pEnemyBullet->nType == BULLETTYPE_ENEMY && pEnemyBullet->bUsed == true)
		{
			if (Collision(pBullet->pos, pBullet->size, pEnemyBullet->pos, pEnemyBullet->size))
			{// 敵に弾が当たった

				if (pEnemyBullet->nLife < 1)
				{
					pEnemyBullet->bUsed = false;

					SetExplosion(pBullet->pos, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
					AddScore(10);
					PlaySound(SOUND_LABEL_SE_HIT000);

				}
				else
				{
					pEnemyBullet->nLife--;
				}

				if (pBullet->nLife < 1)
				{
					pBullet->bUsed = false;
				}
				else
				{
					pBullet->nLife--;
				}
			}
		}
	}
}
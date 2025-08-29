//=====================================================================
// 
// パーティクル処理 [particle.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "input.h"

#include "particle.h"
#include "effect.h"

//*******************************************
// マクロ定義
//*******************************************
#define DEF_TEXTURE_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)		// テクスチャの色

#define DEF_PARTICLE_LIFETIME			(100.0f)								// エフェクトが残る時間
#define DEF_PARTICLE_SIZE				(30.0f)									// エフェクトのサイズ倍率

//*******************************************
// グローバル変数宣言
//*******************************************
Particle g_aParticle[MAX_PARTICLE];

//=====================================================================
// 初期化処理
//=====================================================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	Particle* pParticle = &g_aParticle[0];

	// デバイスの取得
	pDevice = GetDevice();

	// エフェクトの情報の初期化
	memset(pParticle, 0, sizeof(Particle) * MAX_PARTICLE);
	for (int nCount = 0; nCount < MAX_PARTICLE; nCount++, pParticle++)
	{
		pParticle->pos = D3DXVECTOR3_ZERO;
		pParticle->fRot = 0.0f;
		pParticle->fScale = 1.0f;

		pParticle->col = DEF_TEXTURE_COLOR;
		pParticle->fRangeRad = D3DX_PI * 2;
		pParticle->fSpeed = 1.0f;
		pParticle->fLifeTime = DEF_PARTICLE_LIFETIME;
		pParticle->bUseTexture = true;
	}
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitParticle(void)
{

}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateParticle(void)
{
	Particle* pParticle = &g_aParticle[0];
	for (int nCount = 0; nCount < MAX_PARTICLE; nCount++, pParticle++)
	{
		if (pParticle->bUsed == true)
		{
			pParticle->nPartcileLife -= 1;

			SetEffect(pParticle->pos, pParticle->col, (float)((rand() % 629) - 314) / 100.0f, pParticle->fSpeed, pParticle->fScale,  pParticle->fLifeTime, false);
			
			if (pParticle->nPartcileLife < 1)
			{
				pParticle->bUsed = false;
			}
		}
	}
}

//=====================================================================
// パーティクルの設定処理
//=====================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRangeRad, float fSpeed, float fScale, float fLifeTime)
{
	Particle* pParticle = &g_aParticle[0];

	for (int nCount = 0; nCount < MAX_PARTICLE; nCount++, pParticle++)
	{
		if (pParticle->bUsed == false)
		{
			pParticle->pos = pos;
			pParticle->fRot = (float)(rand() / (314 * 2)) * 0.01f;
			pParticle->fScale = fScale;

			pParticle->col = col;
			pParticle->fRangeRad = 1.0f;
			pParticle->fSpeed = 8.0f;
			pParticle->fLifeTime = 15.0f;
			pParticle->nPartcileLife = 20;
			pParticle->bUseTexture = true;
			pParticle->bUsed = true;
			break;
		}
	}
}

//=====================================================================
// パーティクル取得処理
//=====================================================================
Particle* GetParticle(void)
{
	return &g_aParticle[0];
}

////=====================================================================
//// エフェクト保存処理
////=====================================================================
//bool SaveParticle(const char* cFilePath)
//{
//	Particle* pParticle = &g_aParticle;
//	FILE* pFile;
//
//	pFile = fopen(cFilePath, "wb");
//
//	if (pFile != NULL)
//	{
//		fwrite(pParticle, sizeof(Particle), 1, pFile);
//
//		fclose(pFile);
//	}
//	else
//	{
//		return false;
//	}
//
//	g_bUnsaved = false;
//
//	return true;
//}
//
////=====================================================================
//// エフェクト読込処理
////=====================================================================
//bool LoadParticle(const char* cFilePath)
//{
//	Particle* pParticle = &g_aParticle;
//	FILE* pFile;
//
//	pFile = fopen(cFilePath, "rb");
//
//	if (pFile != NULL)
//	{
//		fread(pParticle, sizeof(Particle), 1, pFile);
//
//		pParticle->pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f };
//		pParticle->fRot= 0.0f;
//		pParticle->bUseTexture = true;
//
//		fclose(pFile);
//	}
//	else
//	{
//		return false;
//	}
//
//	g_bUnsaved = false;
//
//	ClearAllEffect();
//
//	return true;
//}
//
//bool IsParticleUnsaved(void)
//{
//	return g_bUnsaved;
//}
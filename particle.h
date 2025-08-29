//=====================================================================
// 
// particle.cppのヘッダファイル [particle.h]
// Author:岩崎桧翔
//
//=====================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_		// 二重インクルード防止のマクロ

#include "main.h"

//*******************************************
// マクロ定義
//*******************************************
#define MAX_PARTICLE		(128)	// パーティクルの最大数

//*******************************************
// エフェクト情報構造体
//*******************************************
typedef struct
{
	char cTextureFilePath[MAX_PATH];

	D3DXVECTOR3 pos;		// 位置
	float fRot;				// 回転
	float fScale;			// サイズ倍率

	D3DXCOLOR col;			// 色
	float fRangeRad;		// 発射角度範囲
	float fSpeed;			// 速度
	float fLifeTime;		// 持続時間
	int nPartcileLife;
	bool bUseTexture;
	bool bUsed;				// 使用しているかどうか
}Particle;

//*******************************************
// プロトタイプ宣言
//*******************************************
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRangeRad, float fSpeed, float fScale, float fLifeTime);
Particle* GetParticle(void);
bool SaveParticle(const char* cFilePath);
bool LoadParticle(const char* cFilePath);
bool IsParticleUnsaved(void);

#endif
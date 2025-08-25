//=====================================================================
// 
// effect.cppのヘッダファイル [effect.h]
// Author:岩崎桧翔
//
//=====================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_		// 二重インクルード防止のマクロ

#include "main.h"

// マクロ定義
#define MAX_EFFECT		(4096)	// エフェクトの最大数

// エフェクトの種類
typedef enum
{
	EFFECTTYPE_NORMAL = 0,
	EFFECTTYPE_MAX
}EFFECTTYPE;

// エフェクト情報構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 size;		// サイズ
	float fScale;			// サイズ倍率
	D3DXCOLOR col;			// 色
	float fRad;				// 角度
	int nLifeTime;			// 持続時間
	int nMaxLifeTime;		// 最大持続時間
	EFFECTTYPE type;		// 種類
	bool bUsed;				// 使用しているかどうか
}Effect;

// プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRad, float fScale, int nLife);

#endif
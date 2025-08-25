//=====================================================================
// 
// Explosion.cppのヘッダファイル [Explosion.h]
// Author:岩崎桧翔
//
//=====================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_		// 二重インクルード防止のマクロ

#include "main.h"

#define MAX_EXPLOSION				(512)

// 弾情報構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	int nCountAnim;			// アニメーションカウント
	int nPatternAnim;		// アニメーションパターンNo
	bool bUsed;				// 使用しているかどうか
}Explosion;

// プロトタイプ宣言
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col);

#endif
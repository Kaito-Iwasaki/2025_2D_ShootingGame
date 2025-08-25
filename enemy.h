//=====================================================================
// 
// enemy.cppのヘッダファイル [enemy.h]
// Author:岩崎桧翔
//
//=====================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_		// 二重インクルード防止のマクロ

#include "main.h"

// マクロ定義
#define MAX_ENEMY		(128)	// 敵の最大数

// 敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGED,
	ENEMYSTATE_MAX
}ENEMYSTATE;

// 敵状態情報構造体
typedef struct
{

}ENEMYPARAM;

// 敵情報構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 posOffset;	// 位置オフセット
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 size;		// 大きさ
	int nType;				// 敵の種類
	int nTex;				// テクスチャの種類
	int nLife;
	ENEMYSTATE state;
	int nItemDrop;

	int nCounterState;
	int nCounterUState;
	int nCounterStep;
	int nShotInterval;

	bool bUsed;				// 使用しているかどうか
}ENEMY;

// プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY* SetEnemy(D3DXVECTOR3 pos, int nType);
void HitEnemy(int nCntEnemy, int nDamage);
ENEMY* GetEnemy(void);
int GetEnemyLeft(void);

#endif
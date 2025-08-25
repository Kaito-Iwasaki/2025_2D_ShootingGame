//=====================================================================
// 
// bullet.cppのヘッダファイル [bullet.h]
// Author:岩崎桧翔
//
//=====================================================================
#ifndef _BULLET_H_
#define _BULLET_H_		// 二重インクルード防止のマクロ

#include "main.h"

// マクロ定義
#define MAX_BULLET		(1024)	// 弾の最大数

// 弾の種類
typedef enum
{
	BULLETTYPE_PLAYER = 0,
	BULLETTYPE_ENEMY,
	BULLETTYPE_MAX
}BULLETTYPE;

// 弾の形状の種類
typedef enum
{
	BULLETPATTERN_PLAYER = 0,
	BULLETPATTERN_SPHERE,
	BULLETPATTERN_THIN,
	BULLETPATTERN_MAX
}BULLETPATTERN;

// 弾情報構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 size;
	float fMove;			// 移動量
	float fRot;				// 方向
	int nLifeTime;			// 寿命
	int nLife;				// 体力
	BULLETTYPE nType;		// 種類
	BULLETPATTERN pattern;	// 形状
	bool bUsed;				// 使用しているかどうか
}Bullet;

// プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, float fMove, float fRot, BULLETTYPE type, BULLETPATTERN pattern);

#endif
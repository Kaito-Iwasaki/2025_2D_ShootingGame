//=====================================================================
// 
// player.cppのヘッダファイル [player.h]
// Author:岩崎桧翔
//
//=====================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_		// 二重インクルード防止のマクロ

#include "main.h"

// プレイヤーの状態の種類
typedef enum
{
	PLAYERSTATE_NORMAL = 0,
	PLAYERSTATE_DAMAGED,
	PLAYERSTATE_FLASH,
	PLAYERSTATE_DEAD,
	PLAYERSTATE_END,
	PLAYERSTATE_MAX
}PLAYERSTATE;

// プレイヤー構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 posMove;	// 移動量
	float fRot;				// 回転			
	float fRotMove;			// 回転量
	D3DXVECTOR3 size;		// サイズ

	int nCountAnim;			// アニメーションカウント
	int nPatternAnim;		// アニメーションパターン

	PLAYERSTATE state;		// 状態
	int nCountState;		// 状態カウント
	float fLife;				// 体力
	int nLastShot;			// 最後に撃ってからの経過時間
	int nUpgrade;			// アップグレード
	int nCharge;			// チャージ

	bool bVisible;			// 描画するか	

}Player;

// プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void HitPlayer(int nDamage);
Player* GetPlayer(void);
bool IsPlayerDead(void);

#endif
//=====================================================================
// 
// ステージ1処理 [stage1.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "stage1.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "util.h"

#include "bg.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "effect.h"
#include "healthbar.h"
#include "pause.h"
#include "particle.h"

//*********************************************************************
// マクロ定義
//*********************************************************************
#define WAVE_INTERVAL		(50)
#define WAVE_MAX			(11)

//*********************************************************************
// グローバル変数
//*********************************************************************
STAGE stage1;
ENEMYTIMELINE timeline[256];

//=====================================================================
// 初期化処理
//=====================================================================
void InitStage1(void)
{
	//***********************************
	// グローバル変数の初期化
	//***********************************
	stage1.bPaused = false;
	stage1.nGameCount = 0;
	stage1.waveState = WAVE_START;
	stage1.nWave = 0;



	memset(&timeline[0], 0, sizeof(ENEMYTIMELINE) * 256);

	//LoadStage(&timeline[0], "data\\STAGE\\stage01.txt");

	//***********************************
	// 各種オブジェクトの初期化処理
	//***********************************
	InitPlayer();		// プレイヤー
	InitBG();			// 背景
	InitBullet();		// 弾
	InitExplosion();	// 爆発エフェクト
	InitEnemy();		// 敵
	InitScore();		// スコア
	InitEffect();		// エフェクト
	InitParticle();
	InitHealthbar();
	InitPause();
	

	//***********************************
	// サウンドの再生
	//***********************************
	PlaySound(SOUND_LABEL_STAGE01);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitStage1(void)
{
	//***********************************
	// サウンドの停止
	//***********************************
	StopSound();

	//***********************************
	// 各種オブジェクトの終了処理
	//***********************************
	UninitPlayer();		// プレイヤー
	UninitBG();			// 背景
	UninitBullet();		// 弾
	UninitExplosion();	// 爆発エフェクト
	UninitEnemy();		// 敵
	UninitScore();		// スコア
	UninitEffect();		// エフェクト
	UninitHealthbar();
	UninitPause();
	UninitParticle();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateStage1(void)
{
	if (stage1.bPaused == false)
	{// ポーズ中でなければ

		// ウェーブ処理
		switch (stage1.waveState)
		{
		case WAVE_START:			// ウェーブ開始
			SetWave(stage1.nWave);

		case WAVE_PROGRESSING:		// ウェーブ中

			stage1.nGameCount++;	// ゲームカウントを進める

			for (int nCount = 0; nCount < 256; nCount++)
			{
				if (timeline[nCount].nWave == stage1.nWave && timeline[nCount].nCountTime == stage1.nGameCount)
				{
					ENEMY* pEnemy;

					pEnemy = SetEnemy(timeline[nCount].nPos, timeline[nCount].nType);
					pEnemy->move.x = 15.0f;
					if (timeline[nCount].bInversed == true)
					{
						pEnemy->move *= -1;
					}
				}
			}

			if (GetEnemyLeft() < 1)
			{
				stage1.nGameCount = 0;
				stage1.waveState = WAVE_END;

			}

			break;

		case WAVE_END:				// ウェーブ終了
			stage1.nGameCount++;
			
			if (stage1.nGameCount > WAVE_INTERVAL)
			{
				stage1.nWave++;

				if (stage1.nWave < WAVE_MAX)
				{
					SetWave(stage1.nWave);
				}
				else
				{
					if (stage1.nGameCount > WAVE_INTERVAL + 50)
					{
						Player* pPlayer = GetPlayer();
						pPlayer->state = PLAYERSTATE_END;
					}
				}
			}
			break;
		}

		//***********************************
		// 各種オブジェクトの更新処理
		//***********************************
		UpdatePlayer();		// プレイヤー
		UpdateBG();			// 背景
		UpdateBullet();		// 弾
		UpdateExplosion();	// 爆発エフェクト
		UpdateEnemy();		// 敵
		UpdateScore();		// スコア
		UpdateEffect();		// エフェクト
		UpdateHealthbar();
		UpdateParticle();
	}
	else
	{
		UpdatePause();
	}

	if (GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START))
	{// ポーズ
		stage1.bPaused = stage1.bPaused ^ 1;
		PlaySound(SOUND_LABEL_SE_PAUSE, 0.1f);

		if (stage1.bPaused == true)
		{
			PauseSound(SOUND_LABEL_STAGE01);
			SetPauseMenuCursor(PAUSE_MENU_CONTINUE);
		}
		else
		{
			UnPauseSound(SOUND_LABEL_STAGE01);
		}
	}

	if (GetPlayer()->state == PLAYERSTATE_END)
	{
		SetFade(MODE_RESULT);
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawStage1(void)
{
	//***********************************
	// 各種オブジェクトの描画処理
	//***********************************
	DrawBG();			// [0]背景
	DrawExplosion();	// [1]爆発エフェクト
	DrawEffect();		// [5]エフェクト
	DrawEnemy();		// [2]敵
	DrawPlayer();		// [3]プレイヤー
	DrawBullet();		// [4]弾
	DrawScore();		// [6]スコア
	DrawHealthbar();

	if (stage1.bPaused)
	{
		DrawPause();
	}
}

//=====================================================================
// ウェーブ設定処理
//=====================================================================
void SetWave(int nWave)
{
	ENEMY* pEnemy;

	switch (nWave)
	{
	case 0:
		for (int nCntEnemy = 0; nCntEnemy < 10; nCntEnemy++)
		{
			pEnemy = SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300, -50 + (-50 * nCntEnemy), 0), 0);
			pEnemy->nCounterStep -= 3 * nCntEnemy;
			pEnemy->move.x = 15.0f;
		}
		break;

	case 1:
		for (int nCntEnemy = 0; nCntEnemy < 10; nCntEnemy++)
		{
			pEnemy = SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, -50 + (-50 * nCntEnemy), 0), 0);
			pEnemy->nCounterStep -= 3 * nCntEnemy;
			pEnemy->move.x = -15.0f;
		}
		break;

	case 2:
		for (int nCntEnemy = 0; nCntEnemy < 10; nCntEnemy++)
		{
			pEnemy = SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300, -50 + (-50 * nCntEnemy), 0), 0);
			pEnemy->nCounterStep -= 3 * nCntEnemy;
			pEnemy->move.x = 15.0f;
		}

		for (int nCntEnemy = 0; nCntEnemy < 10; nCntEnemy++)
		{
			pEnemy = SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, -50 + (-50 * nCntEnemy), 0), 0);
			pEnemy->nCounterStep -= 3 * nCntEnemy;
			pEnemy->move.x = -15.0f;
		}
		break;

	case 3:
		for (int nCntEnemy = 0; nCntEnemy < 20; nCntEnemy++)
		{
			pEnemy = SetEnemy(
				D3DXVECTOR3(
					SCREEN_WIDTH / 2 + (rand() % 1000 + 1) - 500,
					-50.0 - (rand() % (100 + 1)),
					0
				), 1
			);
			pEnemy->nCounterStep -= 3 * nCntEnemy;
			pEnemy->move.x = -15.0f;
		}
		break;

	case 4:
		for (int nCntEnemy = 0; nCntEnemy < 3; nCntEnemy++)
		{
			pEnemy = SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + (-400 + (nCntEnemy * 400)), -50, 0), 2);
			pEnemy->nLife = 10;
		}

		break;

	case 5:
		for (int nCntEnemy = 0; nCntEnemy < 10; nCntEnemy++)
		{
			pEnemy = SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300, -50 + (-50 * nCntEnemy), 0), 0);
			pEnemy->nCounterStep -= 3 * nCntEnemy;
			pEnemy->move.x = 15.0f;
		}

		for (int nCntEnemy = 0; nCntEnemy < 10; nCntEnemy++)
		{
			pEnemy = SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, -50 + (-50 * nCntEnemy), 0), 0);
			pEnemy->nCounterStep -= 3 * nCntEnemy;
			pEnemy->move.x = -15.0f;
		}

		for (int nCntEnemy = 0; nCntEnemy < 3; nCntEnemy++)
		{
			pEnemy = SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + (-400 + (nCntEnemy * 400)), -50, 0), 2);
			pEnemy->nLife = 10;
		}

		break;

	case 6:
		for (int nCntEnemy = 0; nCntEnemy < 20; nCntEnemy++)
		{
			pEnemy = SetEnemy(D3DXVECTOR3(RandRange(0,SCREEN_WIDTH), -50 + (-20 * nCntEnemy), 0), 3);
			pEnemy->move.y = RandRange(4, 7);
		}
		break;

	case 7:
		for (int nCntEnemy = 0; nCntEnemy < 10; nCntEnemy++)
		{
			pEnemy = SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300, -50 + (-50 * nCntEnemy), 0), 4);
			pEnemy->move.y = 3;
			pEnemy->nLife = 3;
			pEnemy = SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, -50 + (-50 * nCntEnemy), 0), 4);
			pEnemy->move.y = 3;
			pEnemy->nLife = 3;
		}
		break;

	case 8:
		pEnemy = SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, -100, 0), 5);
		pEnemy->move = { 3.0f, 5.0f , 5.0f};
		pEnemy->nLife = 50;
		break;

	case 9:
		pEnemy = SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300, -100, 0), 5);
		pEnemy->move = { 0.0f, 5.0f , 5.0f };
		pEnemy->nLife = 50;
		pEnemy = SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, -100, 0), 5);
		pEnemy->move = { 0.0f, 5.0f , 5.0f };
		pEnemy->nLife = 50;
		break;

	case 10:
		pEnemy = SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 400, -100, 0), 5);
		pEnemy->move = { 0.0f, 5.0f , 5.0f };
		pEnemy->nLife = 50;
		pEnemy = SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 400, -100, 0), 5);
		pEnemy->move = { 0.0f, 5.0f , 5.0f };
		pEnemy->nLife = 50;
		pEnemy = SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, -100, 0), 5);
		pEnemy->move = { 0.0f, 5.0f , 5.0f };
		pEnemy->nLife = 50;
		break;
	}

	stage1.nGameCount = 0;
	stage1.waveState = WAVE_PROGRESSING;
}

void TogglePause(bool bPause)
{
	stage1.bPaused = bPause;
}

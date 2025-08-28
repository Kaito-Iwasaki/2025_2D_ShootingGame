//=====================================================================
// 
// ゲーム画面処理 [game.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "game.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "bg.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "effect.h"

// グローバル変数
bool g_bPaused;

void InitGame(void)
{
	// グローバル変数の初期化
	g_bPaused = false;

	// << 各種オブジェクトの初期化処理 >>
	// プレイヤー
	InitPlayer();

	// 背景
	InitBG();

	// 弾
	InitBullet();

	// 爆発エフェクト
	InitExplosion();

	// 敵
	InitEnemy();

	// スコア
	InitScore();

	// エフェクト
	InitEffect();

	// 敵の配置
	for (int nCntEnemy = 0; nCntEnemy < 10; nCntEnemy++)
	{
		SetEnemy(D3DXVECTOR3(300 + nCntEnemy * 80, 50, 0.0f), 3);
		SetEnemy(D3DXVECTOR3(300 + nCntEnemy * 80, 100, 0.0f), 2);
		SetEnemy(D3DXVECTOR3(300 + nCntEnemy * 80, 150, 0.0f), 1);
		SetEnemy(D3DXVECTOR3(300 + nCntEnemy * 80, 200, 0.0f), 0);
	}

	// サウンドの再生
	PlaySound(SOUND_LABEL_BGM004);


}

void UninitGame(void)
{
	// サウンドの停止
	StopSound();

	//// << 各種オブジェクトの終了処理 >>
	// プレイヤーの初期化処理
	UninitPlayer();

	// 背景の初期化処理
	UninitBG();

	// 弾の初期化処理
	UninitBullet();

	// 爆発エフェクトの初期化処理
	UninitExplosion();

	// 敵の初期化処理
	UninitEnemy();

	// スコアの初期化処理
	UninitScore();

	// エフェクト
	UninitEffect();
}

void UpdateGame(void)
{
	if (g_bPaused == false)
	{
		// << 各種オブジェクトの更新処理 >>
		// プレイヤー
		UpdatePlayer();

		// 背景
		UpdateBG();

		// 弾
		UpdateBullet();

		// 爆発エフェクト
		UpdateExplosion();

		// 敵
		UpdateEnemy();

		// スコア
		UpdateScore();

		// エフェクト
		UpdateEffect();
	}

	if (GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START))
	{// ポーズ
		g_bPaused = g_bPaused ^ 1;
	}

	if (GetPlayer()->state == PLAYERSTATE_END)
	{
		SetFade(MODE_RESULT);
	}
}

void DrawGame(void)
{
	// << 各種オブジェクトの更新処理 >>
	// 背景
	DrawBG();

	// 爆発エフェクト
	DrawExplosion();

	// 敵
	DrawEnemy();

	// プレイヤー
	DrawPlayer();

	// 弾
	DrawBullet();

	// エフェクト
	DrawEffect();

	// スコア
	DrawScore();
}

bool ReadLine(char* buffer, int nMaxCount, FILE* pFile)
{
	if (fgets(buffer, nMaxCount, pFile) != NULL)
	{
		for (int i = 0; i < strlen(buffer); i++)
		{
			if (buffer[i] == '#')
			{
				buffer[i] = '\0';
				break;
			}
			else if (buffer[i] == EOF)
			{
				return false;
			}
		}

		return true;
	}
	
	return false;
}

void LoadStage(ENEMYTIMELINE* pTimeline, const char* pFileName)
{
	FILE* pFile;
	char aFileLine[MAX_PATH] = {};

	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{// テキストファイル読み込み
		while (ReadLine(aFileLine, 256, pFile))
		{
			if (strstr(aFileLine, "TIMELINESET"))
			{
				do
				{
					fscanf(pFile, "%s", aFileLine);

					if (strcmp(aFileLine, "WAVE") == 0)
					{
						fscanf(pFile, "%*s%d", &pTimeline->nWave);
					}
					else if (strcmp(aFileLine, "TIME") == 0)
					{
						fscanf(pFile, "%*s%d", &pTimeline->nCountTime);
					}
					else if (strcmp(aFileLine, "TYPE") == 0)
					{
						fscanf(pFile, "%*s%d", &pTimeline->nType);
					}
					else if (strcmp(aFileLine, "POS") == 0)
					{
						fscanf(pFile, "%*s%f", &pTimeline->nPos.x);
						fscanf(pFile, "%f", &pTimeline->nPos.y);
					}
				} while (strcmp(aFileLine, "END_TIMELINESET") != 0);

				pTimeline++;
			}
		}
	}
}

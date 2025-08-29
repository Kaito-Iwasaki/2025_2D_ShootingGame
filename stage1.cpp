//=====================================================================
// 
// �X�e�[�W1���� [stage1.cpp]
// Author:���O��
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
// �}�N����`
//*********************************************************************
#define WAVE_INTERVAL		(50)
#define WAVE_MAX			(11)

//*********************************************************************
// �O���[�o���ϐ�
//*********************************************************************
STAGE stage1;
ENEMYTIMELINE timeline[256];

//=====================================================================
// ����������
//=====================================================================
void InitStage1(void)
{
	//***********************************
	// �O���[�o���ϐ��̏�����
	//***********************************
	stage1.bPaused = false;
	stage1.nGameCount = 0;
	stage1.waveState = WAVE_START;
	stage1.nWave = 0;



	memset(&timeline[0], 0, sizeof(ENEMYTIMELINE) * 256);

	//LoadStage(&timeline[0], "data\\STAGE\\stage01.txt");

	//***********************************
	// �e��I�u�W�F�N�g�̏���������
	//***********************************
	InitPlayer();		// �v���C���[
	InitBG();			// �w�i
	InitBullet();		// �e
	InitExplosion();	// �����G�t�F�N�g
	InitEnemy();		// �G
	InitScore();		// �X�R�A
	InitEffect();		// �G�t�F�N�g
	InitParticle();
	InitHealthbar();
	InitPause();
	

	//***********************************
	// �T�E���h�̍Đ�
	//***********************************
	PlaySound(SOUND_LABEL_STAGE01);
}

//=====================================================================
// �I������
//=====================================================================
void UninitStage1(void)
{
	//***********************************
	// �T�E���h�̒�~
	//***********************************
	StopSound();

	//***********************************
	// �e��I�u�W�F�N�g�̏I������
	//***********************************
	UninitPlayer();		// �v���C���[
	UninitBG();			// �w�i
	UninitBullet();		// �e
	UninitExplosion();	// �����G�t�F�N�g
	UninitEnemy();		// �G
	UninitScore();		// �X�R�A
	UninitEffect();		// �G�t�F�N�g
	UninitHealthbar();
	UninitPause();
	UninitParticle();
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateStage1(void)
{
	if (stage1.bPaused == false)
	{// �|�[�Y���łȂ����

		// �E�F�[�u����
		switch (stage1.waveState)
		{
		case WAVE_START:			// �E�F�[�u�J�n
			SetWave(stage1.nWave);

		case WAVE_PROGRESSING:		// �E�F�[�u��

			stage1.nGameCount++;	// �Q�[���J�E���g��i�߂�

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

		case WAVE_END:				// �E�F�[�u�I��
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
		// �e��I�u�W�F�N�g�̍X�V����
		//***********************************
		UpdatePlayer();		// �v���C���[
		UpdateBG();			// �w�i
		UpdateBullet();		// �e
		UpdateExplosion();	// �����G�t�F�N�g
		UpdateEnemy();		// �G
		UpdateScore();		// �X�R�A
		UpdateEffect();		// �G�t�F�N�g
		UpdateHealthbar();
		UpdateParticle();
	}
	else
	{
		UpdatePause();
	}

	if (GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START))
	{// �|�[�Y
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
// �`�揈��
//=====================================================================
void DrawStage1(void)
{
	//***********************************
	// �e��I�u�W�F�N�g�̕`�揈��
	//***********************************
	DrawBG();			// [0]�w�i
	DrawExplosion();	// [1]�����G�t�F�N�g
	DrawEffect();		// [5]�G�t�F�N�g
	DrawEnemy();		// [2]�G
	DrawPlayer();		// [3]�v���C���[
	DrawBullet();		// [4]�e
	DrawScore();		// [6]�X�R�A
	DrawHealthbar();

	if (stage1.bPaused)
	{
		DrawPause();
	}
}

//=====================================================================
// �E�F�[�u�ݒ菈��
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

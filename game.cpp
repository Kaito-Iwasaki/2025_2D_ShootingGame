//=====================================================================
// 
// �Q�[����ʏ��� [game.cpp]
// Author:���O��
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

// �O���[�o���ϐ�
bool g_bPaused;

void InitGame(void)
{
	// �O���[�o���ϐ��̏�����
	g_bPaused = false;

	// << �e��I�u�W�F�N�g�̏��������� >>
	// �v���C���[
	InitPlayer();

	// �w�i
	InitBG();

	// �e
	InitBullet();

	// �����G�t�F�N�g
	InitExplosion();

	// �G
	InitEnemy();

	// �X�R�A
	InitScore();

	// �G�t�F�N�g
	InitEffect();

	// �G�̔z�u
	for (int nCntEnemy = 0; nCntEnemy < 10; nCntEnemy++)
	{
		SetEnemy(D3DXVECTOR3(300 + nCntEnemy * 80, 50, 0.0f), 3);
		SetEnemy(D3DXVECTOR3(300 + nCntEnemy * 80, 100, 0.0f), 2);
		SetEnemy(D3DXVECTOR3(300 + nCntEnemy * 80, 150, 0.0f), 1);
		SetEnemy(D3DXVECTOR3(300 + nCntEnemy * 80, 200, 0.0f), 0);
	}

	// �T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM004);
}

void UninitGame(void)
{
	// �T�E���h�̒�~
	StopSound();

	// << �e��I�u�W�F�N�g�̏I������ >>
	// �v���C���[�̏���������
	UninitPlayer();

	// �w�i�̏���������
	UninitBG();

	// �e�̏���������
	UninitBullet();

	// �����G�t�F�N�g�̏���������
	UninitExplosion();

	// �G�̏���������
	UninitEnemy();

	// �X�R�A�̏���������
	UninitScore();

	// �G�t�F�N�g
	UninitEffect();
}

void UpdateGame(void)
{
	if (g_bPaused == false)
	{
		// << �e��I�u�W�F�N�g�̍X�V���� >>
		// �v���C���[
		UpdatePlayer();

		// �w�i
		UpdateBG();

		// �e
		UpdateBullet();

		// �����G�t�F�N�g
		UpdateExplosion();

		// �G
		UpdateEnemy();

		// �X�R�A
		UpdateScore();

		// �G�t�F�N�g
		UpdateEffect();
	}

	if (GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START))
	{// �|�[�Y
		g_bPaused = g_bPaused ^ 1;
	}

	if (GetPlayer()->state == PLAYERSTATE_END)
	{
		SetFade(MODE_RESULT);
	}
}

void DrawGame(void)
{
	// << �e��I�u�W�F�N�g�̍X�V���� >>
	// �w�i
	DrawBG();

	// �����G�t�F�N�g
	DrawExplosion();

	// �G
	DrawEnemy();

	// �v���C���[
	DrawPlayer();

	// �e
	DrawBullet();

	// �G�t�F�N�g
	DrawEffect();

	// �X�R�A
	DrawScore();
}
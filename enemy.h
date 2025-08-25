//=====================================================================
// 
// enemy.cpp�̃w�b�_�t�@�C�� [enemy.h]
// Author:���O��
//
//=====================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_		// ��d�C���N���[�h�h�~�̃}�N��

#include "main.h"

// �}�N����`
#define MAX_ENEMY		(128)	// �G�̍ő吔

// �G�̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGED,
	ENEMYSTATE_MAX
}ENEMYSTATE;

// �G��ԏ��\����
typedef struct
{

}ENEMYPARAM;

// �G���\����
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 posOffset;	// �ʒu�I�t�Z�b�g
	D3DXVECTOR3 move;		// �ړ���
	D3DXVECTOR3 size;		// �傫��
	int nType;				// �G�̎��
	int nTex;				// �e�N�X�`���̎��
	int nLife;
	ENEMYSTATE state;
	int nItemDrop;

	int nCounterState;
	int nCounterUState;
	int nCounterStep;
	int nShotInterval;

	bool bUsed;				// �g�p���Ă��邩�ǂ���
}ENEMY;

// �v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY* SetEnemy(D3DXVECTOR3 pos, int nType);
void HitEnemy(int nCntEnemy, int nDamage);
ENEMY* GetEnemy(void);
int GetEnemyLeft(void);

#endif
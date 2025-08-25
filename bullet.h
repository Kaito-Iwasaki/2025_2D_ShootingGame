//=====================================================================
// 
// bullet.cpp�̃w�b�_�t�@�C�� [bullet.h]
// Author:���O��
//
//=====================================================================
#ifndef _BULLET_H_
#define _BULLET_H_		// ��d�C���N���[�h�h�~�̃}�N��

#include "main.h"

// �}�N����`
#define MAX_BULLET		(1024)	// �e�̍ő吔

// �e�̎��
typedef enum
{
	BULLETTYPE_PLAYER = 0,
	BULLETTYPE_ENEMY,
	BULLETTYPE_MAX
}BULLETTYPE;

// �e�̌`��̎��
typedef enum
{
	BULLETPATTERN_PLAYER = 0,
	BULLETPATTERN_SPHERE,
	BULLETPATTERN_THIN,
	BULLETPATTERN_MAX
}BULLETPATTERN;

// �e���\����
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 size;
	float fMove;			// �ړ���
	float fRot;				// ����
	int nLifeTime;			// ����
	int nLife;				// �̗�
	BULLETTYPE nType;		// ���
	BULLETPATTERN pattern;	// �`��
	bool bUsed;				// �g�p���Ă��邩�ǂ���
}Bullet;

// �v���g�^�C�v�錾
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, float fMove, float fRot, BULLETTYPE type, BULLETPATTERN pattern);

#endif
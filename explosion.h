//=====================================================================
// 
// Explosion.cpp�̃w�b�_�t�@�C�� [Explosion.h]
// Author:���O��
//
//=====================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_		// ��d�C���N���[�h�h�~�̃}�N��

#include "main.h"

#define MAX_EXPLOSION				(512)

// �e���\����
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXCOLOR col;			// �F
	int nCountAnim;			// �A�j���[�V�����J�E���g
	int nPatternAnim;		// �A�j���[�V�����p�^�[��No
	bool bUsed;				// �g�p���Ă��邩�ǂ���
}Explosion;

// �v���g�^�C�v�錾
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col);

#endif
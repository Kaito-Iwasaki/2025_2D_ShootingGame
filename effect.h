//=====================================================================
// 
// effect.cpp�̃w�b�_�t�@�C�� [effect.h]
// Author:���O��
//
//=====================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_		// ��d�C���N���[�h�h�~�̃}�N��

#include "main.h"

// �}�N����`
#define MAX_EFFECT		(4096)	// �G�t�F�N�g�̍ő吔

// �G�t�F�N�g�̎��
typedef enum
{
	EFFECTTYPE_NORMAL = 0,
	EFFECTTYPE_MAX
}EFFECTTYPE;

// �G�t�F�N�g���\����
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 size;		// �T�C�Y
	float fScale;			// �T�C�Y�{��
	D3DXCOLOR col;			// �F
	float fRad;				// �p�x
	int nLifeTime;			// ��������
	int nMaxLifeTime;		// �ő厝������
	EFFECTTYPE type;		// ���
	bool bUsed;				// �g�p���Ă��邩�ǂ���
}Effect;

// �v���g�^�C�v�錾
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRad, float fScale, int nLife);

#endif
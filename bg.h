//=====================================================================
// 
// bg.cpp�̃w�b�_�t�@�C�� [bg.h]
// Author:���O��
//
//=====================================================================
#ifndef _BG_H_
#define _BG_H_		// ��d�C���N���[�h�h�~�̃}�N��

#include "main.h"

// �w�i�\����
typedef struct
{
	D3DXVECTOR3 pos;
	float fMove;
	D3DXCOLOR col;
}Background;

// �v���g�^�C�v�錾
void InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

#endif
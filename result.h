//=====================================================================
// 
// result.cpp�̃w�b�_�t�@�C�� [result.h]
// Author:���O��
//
//=====================================================================
#ifndef _RESULT_H_
#define _RESULT_H_		// ��d�C���N���[�h�h�~�̃}�N��

#include "main.h"

// �w�i�\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
}Result;

// �v���g�^�C�v�錾
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif
//=====================================================================
// 
// title.cpp�̃w�b�_�t�@�C�� [title.h]
// Author:���O��
//
//=====================================================================
#ifndef _TITLE_H_
#define _TITLE_H_		// ��d�C���N���[�h�h�~�̃}�N��

#include "main.h"

// �w�i�\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
}Title;

// �v���g�^�C�v�錾
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif
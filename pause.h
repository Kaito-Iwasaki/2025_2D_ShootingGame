//=====================================================================
// 
// pause.cpp�̃w�b�_�t�@�C�� [pause.h]
// Author:���O��
//
//=====================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_		// ��d�C���N���[�h�h�~�̃}�N��

#include "main.h"

// �w�i�\����
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,
	PAUSE_MENU_RETRY,
	PAUSE_MENU_QUIT,
	PAUSE_MENU_MAX
}PAUSE_MENU;

// �v���g�^�C�v�錾
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void SetPauseMenuCursor(int nCursor);

#endif
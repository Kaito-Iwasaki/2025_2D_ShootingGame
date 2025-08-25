//=====================================================================
// 
// input.cpp�̃w�b�_�t�@�C�� [input.h]
// Author:���O��
//
//=====================================================================
#ifndef _INPUT_H_
#define _INPUT_H_		// ��d�C���N���[�h�h�~�̃}�N��

#include "main.h"

//*********************************************
// �}�N����`
//*********************************************
#define INPUT_MAX_MAGNITUDE		(32767)		// �X�e�B�b�N�̍ő�ړ���
#define INPUT_DEADZONE			(3000)		// �X�e�B�b�N�̃f�b�h�]�[���i0�`32767�j

//*********************************************
// �W���C�p�b�h�̃L�[�̎��
//*********************************************
typedef enum
{
	JOYKEY_UP = 0,		// �\���L�[��
	JOYKEY_DOWN,		// �\���L�[��
	JOYKEY_LEFT,		// �\���L�[��
	JOYKEY_RIGHT,		// �\���L�[�E
	JOYKEY_START,		// �X�^�[�g
	JOYKEY_BACK,		// �o�b�N�i�Z���N�g�j
	JOYKEY_LTHUMB,		// ���X�e�B�b�N��������
	JOYKEY_RTHUMB,		// �E�X�e�B�b�N��������
	JOYKEY_LSHOULDER,	// LB
	JOYKEY_RSHOULDER,	// RB
	JOYKEY_A = 12,		// A
	JOYKEY_B,			// B
	JOYKEY_X,			// X
	JOYKEY_Y,			// Y
	JOYKEY_MAX			// �\���L�[��
}JOYKEY;

//*********************************************
// �v���g�^�C�v�錾
//*********************************************
// [�L�[�{�[�h]
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey);
// [�W���C�p�b�h]
HRESULT InitJoypad(void);
void UninitJoypad(void);
void UpdateJoypad(void);
XINPUT_STATE* GetJoypad(void);
bool GetJoypadPress(JOYKEY key);
bool GetJoypadTrigger(JOYKEY key);
bool GetJoypadRelease(JOYKEY key);
void SetVibration(WORD wLeftMotorSpeed, WORD wRightMotorSpeed);

#endif
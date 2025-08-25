//=====================================================================
// 
// main.cpp�̃w�b�_�t�@�C�� [main.h]
// Author:���O��
//
//=====================================================================
#ifndef _MAIN_H_	// if not defined (main.h����`����Ă��Ȃ�������)
#define _MAIN_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����(�����#pragma once�Ƃ܂����������Ӗ�)

#include <windows.h>
#include <stdio.h>
#include "d3dx9.h"							// �`�揈���ɕK�v
#define	DIRECTINPUT_VERSION		(0x0800)	
#include "dinput.h"
#include "xaudio2.h"
#include "xinput.h"

// ���C�u�����̃����N
#pragma comment(lib, "d3d9.lib")		// �`�揈���ɕK�v
#pragma comment(lib, "d3dx9.lib")		// [d3d9.lib]�̊g�����C�u����
#pragma comment(lib, "dxguid.lib")		// DirectX�R���|�[�l���g�i���i�j�g�p�ɕK�v
#pragma comment(lib, "winmm.lib")		// �V�X�e�������擾�ɕK�v
#pragma comment(lib, "dinput8.lib")		// ���͏����ɕK�v
#pragma comment(lib, "xinput.lib")		// �W���C�p�b�h�����ɕK�v

// �}�N����`
#define SCREEN_WIDTH		(1280)		// �E�B���h�E�̕�
#define SCREEN_HEIGHT		(720)		// �E�B���h�E�̍���
#define WINDOWED			(true)		// �E�B���h�E�\�����邩
#define BG_COLOR			(D3DCOLOR_RGBA(0, 0, 0, 0))
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// ���_�t�H�[�}�b�g[2D]
#define D3DXVECTOR3_ZERO	D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define VIBRATION_ENABLED		(true)		// �U�����L����

// ��ʁi���[�h�j�̎��
typedef enum
{
	MODE_TITLE = 0,
	MODE_GAME,
	MODE_STAGE1,
	MODE_RESULT,
	MODE_MAX
}MODE;

// ���_���[2D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_2D;

// �v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);
HWND GetMainWindow(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif
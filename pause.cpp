//=====================================================================
// 
// �w�i���� [pause.cpp]
// Author:���O��
//
//=====================================================================

// �C���N���[�h�t�@�C��
#include "pause.h"
#include "player.h"
#include "input.h"
#include "fade.h"
#include "stage1.h"

// �}�N����`
#define TEXTURE1_FILENAME		"data\\TEXTURE\\pause000.png"
#define TEXTURE2_FILENAME		"data\\TEXTURE\\pause001.png"
#define TEXTURE3_FILENAME		"data\\TEXTURE\\pause002.png"
#define TEXTURE_POS_X			(SCREEN_WIDTH / 2)
#define TEXTURE_POS_Y			(SCREEN_HEIGHT / 2 - 100)
#define TEXTURE_SIZE			(1)
#define TEXTURE_SCALE_X			(500)
#define TEXTURE_SCALE_Y			(100)

#define NUM_PAUSE				(3)

#define COLOR_SELECTED			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_DESELECTED		D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f)

// �O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTexturePause[NUM_PAUSE] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
int g_nPauseMenu;

D3DXVECTOR3 g_pauseMenuPos;

//=====================================================================
// ����������
//=====================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE1_FILENAME,
		&g_apTexturePause[0]
	);
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE2_FILENAME,
		&g_apTexturePause[1]
	);
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE3_FILENAME,
		&g_apTexturePause[2]
	);

	g_pauseMenuPos = D3DXVECTOR3(TEXTURE_POS_X, TEXTURE_POS_Y, 0.0f);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_PAUSE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL
	);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_pauseMenuPos.x - TEXTURE_SCALE_X / 2, g_pauseMenuPos.y + (100 * nCntPause) - TEXTURE_SCALE_Y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_pauseMenuPos.x + TEXTURE_SCALE_X / 2, g_pauseMenuPos.y + (100 * nCntPause) - TEXTURE_SCALE_Y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_pauseMenuPos.x - TEXTURE_SCALE_X / 2, g_pauseMenuPos.y + (100 * nCntPause) + TEXTURE_SCALE_Y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_pauseMenuPos.x + TEXTURE_SCALE_X / 2, g_pauseMenuPos.y + (100 * nCntPause) + TEXTURE_SCALE_Y / 2, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = COLOR_DESELECTED;
		pVtx[1].col = COLOR_DESELECTED;
		pVtx[2].col = COLOR_DESELECTED;
		pVtx[3].col = COLOR_DESELECTED;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f * TEXTURE_SIZE, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f * TEXTURE_SIZE);
		pVtx[3].tex = D3DXVECTOR2(1.0f * TEXTURE_SIZE, 1.0f * TEXTURE_SIZE);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause->Unlock();
}
//=====================================================================
// �I������
//=====================================================================
void UninitPause(void)
{
	int nCntPause;

	for (nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		// �e�N�X�`���̔j��
		if (g_apTexturePause[nCntPause] != NULL)
		{
			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdatePause(void)
{
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		if (nCntPause == g_nPauseMenu)
		{
			pVtx[0].col = COLOR_SELECTED;
			pVtx[1].col = COLOR_SELECTED;
			pVtx[2].col = COLOR_SELECTED;
			pVtx[3].col = COLOR_SELECTED;
		}
		else
		{
			pVtx[0].col = COLOR_DESELECTED;
			pVtx[1].col = COLOR_DESELECTED;
			pVtx[2].col = COLOR_DESELECTED;
			pVtx[3].col = COLOR_DESELECTED;
		}

		pVtx += 4;
	}

	if (GetKeyboardTrigger(DIK_RETURN))
	{
		switch (g_nPauseMenu)
		{
		case PAUSE_MENU_CONTINUE:
			TogglePause(false);
			break;

		case PAUSE_MENU_RETRY:
			SetFade(MODE_STAGE1);
			break;

		case PAUSE_MENU_QUIT:
			SetFade(MODE_TITLE);
			break;
		}
	}

	if (GetKeyboardRepeat(DIK_UP))
	{
		g_nPauseMenu = g_nPauseMenu - 1;
	}
	if (GetKeyboardRepeat(DIK_DOWN))
	{
		g_nPauseMenu = g_nPauseMenu + 1;
	}
	g_nPauseMenu = g_nPauseMenu % 3;
	if (g_nPauseMenu < 0)
	{
		g_nPauseMenu = PAUSE_MENU_MAX - 1;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause->Unlock();
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntPause;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTexturePause[nCntPause]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}

void SetPauseMenuCursor(int nCursor)
{
	g_nPauseMenu = nCursor;
}
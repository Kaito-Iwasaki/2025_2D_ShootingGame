//=====================================================================
// 
// �w�i���� [result.cpp]
// Author:���O��
//
//=====================================================================
#include "result.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "player.h"
#include "score.h"

// �}�N����`
#define TEXTURE_FILENAME			"data\\TEXTURE\\sakai2.png"
#define TEXTURE_POS_X				(SCREEN_WIDTH / 2)
#define TEXTURE_POS_Y				(SCREEN_HEIGHT / 2)
#define TEXTURE_SCALE_X				(SCREEN_WIDTH)
#define TEXTURE_SCALE_Y				(SCREEN_HEIGHT)
#define TEXTURE_COLOR_CLEAR			D3DCOLOR_RGBA(255, 255, 255, 255)
#define TEXTURE_COLOR_GAMEOVER		D3DCOLOR_RGBA(255, 0, 0, 255)

#define MAX_LABEL		(2)

// �O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureResult = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
Result g_result;
LPD3DXFONT g_pFontLabel[MAX_LABEL] = {};						// �t�H���g�ւ̃|�C���^

//=====================================================================
// ����������
//=====================================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^
	Result* pResult = &g_result;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �f�o�b�O�\���p�̃t�H���g��ݒ�
	D3DXFONT_DESC fontDesc = {
	40,                        // Height
	0,                         // Width (0 = ����)
	FW_NORMAL,                 // Weight
	1,                         // MipLevels
	FALSE,                     // Italic
	DEFAULT_CHARSET,          // CharSet
	OUT_DEFAULT_PRECIS,       // OutputPrecision
	CLIP_DEFAULT_PRECIS,      // Quality
	DEFAULT_PITCH | FF_DONTCARE, // PitchAndFamily
	"x10y12pxDonguriDuel"              // FaceName�i.ttf���j
	};

	// �f�o�b�O�\���p�̃t�H���g���쐬
	for (int nCountFont = 0; nCountFont < MAX_LABEL; nCountFont++)
	{
		D3DXCreateFontIndirect(pDevice, &fontDesc, &g_pFontLabel[nCountFont]);
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE_FILENAME,
		&g_pTextureResult
	);

	// �e�̏��̏�����
	pResult->pos = D3DXVECTOR3(TEXTURE_POS_X, TEXTURE_POS_Y, 0.0f);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL
	);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pResult->pos.x - TEXTURE_SCALE_X / 2, pResult->pos.y - TEXTURE_SCALE_Y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pResult->pos.x + TEXTURE_SCALE_X / 2, pResult->pos.y - TEXTURE_SCALE_Y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pResult->pos.x - TEXTURE_SCALE_X / 2, pResult->pos.y + TEXTURE_SCALE_Y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pResult->pos.x + TEXTURE_SCALE_X / 2, pResult->pos.y + TEXTURE_SCALE_Y / 2, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = pResult->col;
	pVtx[1].col = pResult->col;
	pVtx[2].col = pResult->col;
	pVtx[3].col = pResult->col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffResult->Unlock();

	PlaySound(SOUND_LABEL_SE_GAMEOVER);
}

//=====================================================================
// �I������
//=====================================================================
void UninitResult(void)
{
	// �t�H���g�̔j��
	for (int nCountFont = 0; nCountFont < MAX_LABEL; nCountFont++)
	{
		if (g_pFontLabel[nCountFont] != NULL)
		{
			g_pFontLabel[nCountFont]->Release();
			g_pFontLabel[nCountFont] = NULL;
		}

	}

	// �e�N�X�`���̔j��
	if (g_pTextureResult != NULL)
	{
		g_pTextureResult->Release();
		g_pTextureResult = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadPress(JOYKEY_START))
	{
		SetFade(MODE_TITLE);
	}
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	RECT rect = { SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 200, SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT / 2 + 200 };
	char aMessage[256];

	if (IsPlayerDead() == true)
	{
		g_pFontLabel[0]->DrawText(NULL, "GAME OVER", -1, &rect, DT_CENTER | DT_VCENTER, D3DXCOLOR(255, 0, 0, 255));
	}
	else
	{
		wsprintf(aMessage, "GAME CLEAR\n YOUR SCORE:%d", GetScore());
		g_pFontLabel[0]->DrawText(NULL, aMessage, -1, &rect, DT_CENTER | DT_VCENTER, D3DXCOLOR(255, 255, 0, 255));
	}
}
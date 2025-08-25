//=====================================================================
// 
// �w�i���� [bg.cpp]
// Author:���O��
//
//=====================================================================
#include "bg.h"
#include "player.h"

// �}�N����`
#define TEXTURE1_FILENAME		"data\\TEXTURE\\bg100.png"
#define TEXTURE2_FILENAME		"data\\TEXTURE\\bg101.png"
#define TEXTURE3_FILENAME		"data\\TEXTURE\\bg102.png"
#define TEXTURE_POS_X			(SCREEN_WIDTH / 2)
#define TEXTURE_POS_Y			(SCREEN_HEIGHT / 2)
#define TEXTURE_SIZE			(2)
#define TEXTURE_SCALE_X			(SCREEN_WIDTH * TEXTURE_SIZE)
#define TEXTURE_SCALE_Y			(SCREEN_HEIGHT * TEXTURE_SIZE)

#define NUM_BG		(3)
#define BG_COLOR	D3DCOLOR_RGBA(255, 255, 255, 255)

// �O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureBG[NUM_BG] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
Background g_aBG[NUM_BG];

//=====================================================================
// ����������
//=====================================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^
	Background* pBG = &g_aBG[0];
	int nCntBG;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE3_FILENAME,
		&g_apTextureBG[0]
	);
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE2_FILENAME,
		&g_apTextureBG[1]
	);
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE1_FILENAME,
		&g_apTextureBG[2]
	);


	// �e�̏��̏�����
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		pBG[nCntBG].pos = D3DXVECTOR3(TEXTURE_POS_X, TEXTURE_POS_Y, 0.0f);
		pBG[nCntBG].fMove = 1.0f * (nCntBG + 1);
		pBG[nCntBG].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL
	);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++, pBG++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pBG->pos.x - TEXTURE_SCALE_X / 2, pBG->pos.y - TEXTURE_SCALE_Y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pBG->pos.x + TEXTURE_SCALE_X / 2, pBG->pos.y - TEXTURE_SCALE_Y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pBG->pos.x - TEXTURE_SCALE_X / 2, pBG->pos.y + TEXTURE_SCALE_Y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pBG->pos.x + TEXTURE_SCALE_X / 2, pBG->pos.y + TEXTURE_SCALE_Y / 2, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = BG_COLOR;
		pVtx[1].col = BG_COLOR;
		pVtx[2].col = BG_COLOR;
		pVtx[3].col = BG_COLOR;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f * TEXTURE_SIZE, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f * TEXTURE_SIZE);
		pVtx[3].tex = D3DXVECTOR2(1.0f * TEXTURE_SIZE, 1.0f * TEXTURE_SIZE);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBG->Unlock();
}
//=====================================================================
// �I������
//=====================================================================
void UninitBG(void)
{
	int nCntBG;

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		// �e�N�X�`���̔j��
		if (g_apTextureBG[nCntBG] != NULL)
		{
			g_apTextureBG[nCntBG]->Release();
			g_apTextureBG[nCntBG] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateBG(void)
{
	VERTEX_2D* pVtx;
	Background* pBG = &g_aBG[0];
	int nCntBG;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++, pBG++)
	{
		// �v���C���[�̈ʒu���擾
		Player* pPlayer = GetPlayer();

		// �w�i�̈ʒu���v�Z
		pBG->pos.x = (SCREEN_WIDTH - pPlayer->pos.x) * pBG->fMove * 0.3;
		pBG->pos.y += pBG->fMove;

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pBG->pos.x - TEXTURE_SCALE_X / 2, pBG->pos.y - TEXTURE_SCALE_Y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pBG->pos.x + TEXTURE_SCALE_X / 2, pBG->pos.y - TEXTURE_SCALE_Y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pBG->pos.x - TEXTURE_SCALE_X / 2, pBG->pos.y + TEXTURE_SCALE_Y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pBG->pos.x + TEXTURE_SCALE_X / 2, pBG->pos.y + TEXTURE_SCALE_Y / 2, 0.0f);

		if (pBG->pos.y >= SCREEN_HEIGHT)
		{// �w�i�����ɍs�������ɖ߂��ă��[�v������
			pBG->pos.y = 0;
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBG->Unlock();
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBG;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureBG[nCntBG]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * 4, 2);
	}
}
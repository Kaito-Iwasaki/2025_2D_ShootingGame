//=====================================================================
// 
// �̗̓o�[ [Healthbar.cpp]
// Author:���O��
//
//=====================================================================
#include "healthbar.h"
#include "player.h"

//*********************************************************************
// 
// ***** �}�N����` *****
// 
//*********************************************************************
#define INIT_SIZE_X		(128.0f)
#define INIT_SIZE_Y		(16.0f)
#define INIT_POS_X		(10)
#define INIT_POS_Y		(SCREEN_HEIGHT - INIT_SIZE_Y - 10)

#define COLOR_REMAIN	D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_DAMAGE	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)

//*********************************************************************
// 
// ***** �O���[�o���ϐ� *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHealthbar = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
HEALTHBAR g_aHealthbar[2];
float g_fLengthHealthbar;				// �Ίp���̒���
float g_fAngleHealthbar;				// �Ίp���̊p�x

//*********************************************************************
// 
// ***** �v���g�^�C�v�錾 *****
// 
//*********************************************************************


//=====================================================================
// 
// ***** ���������� *****
// 
//=====================================================================
void InitHealthbar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;
	HEALTHBAR* pHealthbar = &g_aHealthbar[0];

	memset(pHealthbar, 0, sizeof(HEALTHBAR) * 2);
	for (int nCount = 0; nCount < 2; nCount++)
	{
		pHealthbar[nCount].pos = { INIT_POS_X, INIT_POS_Y, 0 };
		pHealthbar[nCount].size = { INIT_SIZE_X, INIT_SIZE_Y, 0 };
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHealthbar,
		NULL
	);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHealthbar->Lock(0, 0, (void**)&pVtx, 0);

	pHealthbar = &g_aHealthbar[0];

	for (int nCount = 0; nCount < 2; nCount++, pHealthbar++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pHealthbar->pos.x, pHealthbar->pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pHealthbar->pos.x + pHealthbar->size.x, pHealthbar->pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pHealthbar->pos.x, pHealthbar->pos.y + pHealthbar->size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pHealthbar->pos.x + pHealthbar->size.x, pHealthbar->pos.y + pHealthbar->size.y, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		if (nCount == 0)
		{
			pVtx[0].col = COLOR_DAMAGE;
			pVtx[1].col = COLOR_DAMAGE;
			pVtx[2].col = COLOR_DAMAGE;
			pVtx[3].col = COLOR_DAMAGE;
		}
		else if (nCount == 1)
		{
			pVtx[0].col = COLOR_REMAIN;
			pVtx[1].col = COLOR_REMAIN;
			pVtx[2].col = COLOR_REMAIN;
			pVtx[3].col = COLOR_REMAIN;
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffHealthbar->Unlock();
}

//=====================================================================
// 
// ***** �I������ *****
// 
//=====================================================================
void UninitHealthbar(void)
{
	if (g_pVtxBuffHealthbar != NULL)
	{
		g_pVtxBuffHealthbar->Release();
		g_pVtxBuffHealthbar = NULL;
	}
}

//=====================================================================
// 
// ***** �X�V���� *****
// 
//=====================================================================
void UpdateHealthbar(void)
{
	Player* pPlayer = GetPlayer();
	VERTEX_2D* pVtx;
	HEALTHBAR* pHealthbar = &g_aHealthbar[0];


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHealthbar->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pHealthbar->pos.x, pHealthbar->pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pHealthbar->pos.x + pHealthbar->size.x, pHealthbar->pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pHealthbar->pos.x, pHealthbar->pos.y + pHealthbar->size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pHealthbar->pos.x + pHealthbar->size.x, pHealthbar->pos.y + pHealthbar->size.y, 0.0f);

	pHealthbar++;
	pVtx += 4;

	float fSizeX = INIT_SIZE_X * (pPlayer->fLife / 3.0f);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pHealthbar->pos.x, pHealthbar->pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pHealthbar->pos.x + fSizeX, pHealthbar->pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pHealthbar->pos.x, pHealthbar->pos.y + pHealthbar->size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pHealthbar->pos.x + fSizeX, pHealthbar->pos.y + pHealthbar->size.y, 0.0f);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffHealthbar->Unlock();
}

//=====================================================================
// 
// ***** �`�揈�� *****
// 
//=====================================================================
void DrawHealthbar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffHealthbar, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < 2; nCount++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
	}
}
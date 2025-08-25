//=====================================================================
// 
// �����G�t�F�N�g���� [explosion.cpp]
// Author:���O��
//
//=====================================================================
#include "explosion.h"

// �}�N����`
#define TEXTURE_FILENAME			"data\\TEXTURE\\explosion000.png"
#define TEXTURE_SCALE_X				(64.0f)
#define TEXTURE_SCALE_Y				(64.0f)
#define TEXTURE_COLOR				D3DCOLOR_RGBA(255, 255, 0, 255)
#define TEXTURE_MAX_X				(5)										// �e�N�X�`���p�^�[����X
#define TEXTURE_MAX_Y				(1)										// �e�N�X�`���p�^�[����Y
#define TEXTURE_MAX_PATTERN			(TEXTURE_MAX_X * TEXTURE_MAX_Y)			// �e�N�X�`�����p�^�[����
#define TEXTURE_INTERVAL			(5)										// �p�^�[���؂�ւ��t���[����

// �O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureExplosion= NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion= NULL;		// ���_�o�b�t�@�ւ̃|�C���^
Explosion g_aExplosion[MAX_EXPLOSION];

float g_fLengthExplosion;				// �Ίp���̒���
float g_fAngleExplosion;				// �Ίp���̊p�x

//=====================================================================
// ����������
//=====================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	Explosion* pExplosion = &g_aExplosion[0];
	int nCntExplosion;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FILENAME, &g_pTextureExplosion);

	// �e�̏��̏�����
	for (nCntExplosion= 0; nCntExplosion< MAX_EXPLOSION; nCntExplosion++)
	{
		pExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pExplosion[nCntExplosion].col = TEXTURE_COLOR;
		pExplosion[nCntExplosion].nCountAnim = 1;
		pExplosion[nCntExplosion].nPatternAnim = 0;
		pExplosion[nCntExplosion].bUsed = false;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL
	);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion= 0; nCntExplosion< MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pExplosion->pos.x - TEXTURE_SCALE_X / 2, pExplosion->pos.y - TEXTURE_SCALE_X / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pExplosion->pos.x + TEXTURE_SCALE_X / 2, pExplosion->pos.y - TEXTURE_SCALE_X / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pExplosion->pos.x - TEXTURE_SCALE_X / 2, pExplosion->pos.y + TEXTURE_SCALE_X / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pExplosion->pos.x + TEXTURE_SCALE_X / 2, pExplosion->pos.y + TEXTURE_SCALE_X / 2, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = pExplosion->col;
		pVtx[1].col = pExplosion->col;
		pVtx[2].col = pExplosion->col;
		pVtx[3].col = pExplosion->col;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex.x = 1.0f / TEXTURE_MAX_X * pExplosion->nPatternAnim;
		pVtx[0].tex.y = 1.0f / TEXTURE_MAX_Y * (pExplosion->nPatternAnim / TEXTURE_MAX_X);

		pVtx[1].tex.x = 1.0f / TEXTURE_MAX_X * (pExplosion->nPatternAnim + 1);
		pVtx[1].tex.y = 1.0f / TEXTURE_MAX_Y * (pExplosion->nPatternAnim / TEXTURE_MAX_X);

		pVtx[2].tex.x = 1.0f / TEXTURE_MAX_X * pExplosion->nPatternAnim;
		pVtx[2].tex.y = 1.0f / TEXTURE_MAX_Y * (pExplosion->nPatternAnim / TEXTURE_MAX_X + 1);

		pVtx[3].tex.x = 1.0f / TEXTURE_MAX_X * (pExplosion->nPatternAnim + 1);
		pVtx[3].tex.y = 1.0f / TEXTURE_MAX_Y * (pExplosion->nPatternAnim / TEXTURE_MAX_X + 1);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}

//=====================================================================
// �I������
//=====================================================================
void UninitExplosion(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureExplosion!= NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion= NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffExplosion!= NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateExplosion(void)
{
	int nCntExplosion;
	VERTEX_2D* pVtx;
	Explosion* pExplosion= &g_aExplosion[0];

	// �Ίp���̒������Z�o
	g_fLengthExplosion= sqrtf(TEXTURE_SCALE_X * TEXTURE_SCALE_X + TEXTURE_SCALE_Y * TEXTURE_SCALE_X) * 0.5;

	// �Ίp���̊p�x���Z�o
	g_fAngleExplosion= atan2f(TEXTURE_SCALE_X, TEXTURE_SCALE_Y);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion= 0; nCntExplosion< MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUsed == true)
		{// �g�p���ł����
			pExplosion->nCountAnim++;

			if (pExplosion->nCountAnim % TEXTURE_INTERVAL == 0)
			{// �p�^�[���X�V
				pExplosion->nPatternAnim++;

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex.x = 1.0f / TEXTURE_MAX_X * pExplosion->nPatternAnim;
				pVtx[0].tex.y = 1.0f / TEXTURE_MAX_Y * (pExplosion->nPatternAnim / TEXTURE_MAX_X);

				pVtx[1].tex.x = 1.0f / TEXTURE_MAX_X * (pExplosion->nPatternAnim + 1);
				pVtx[1].tex.y = 1.0f / TEXTURE_MAX_Y * (pExplosion->nPatternAnim / TEXTURE_MAX_X);

				pVtx[2].tex.x = 1.0f / TEXTURE_MAX_X * pExplosion->nPatternAnim;
				pVtx[2].tex.y = 1.0f / TEXTURE_MAX_Y * (pExplosion->nPatternAnim / TEXTURE_MAX_X + 1);

				pVtx[3].tex.x = 1.0f / TEXTURE_MAX_X * (pExplosion->nPatternAnim + 1);
				pVtx[3].tex.y = 1.0f / TEXTURE_MAX_Y * (pExplosion->nPatternAnim / TEXTURE_MAX_X + 1);

				if (pExplosion->nPatternAnim >= TEXTURE_MAX_PATTERN)
				{// ���p�^�[�����𒴂�����
					pExplosion->bUsed = false;
				}
			}

		}
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (nCntExplosion = 0; nCntExplosion< MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUsed == true)
		{
			 //�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion* 4, 2);
		}
	}
}

//=====================================================================
// �����̐ݒ菈��
//=====================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCntExplosion;
	VERTEX_2D* pVtx;
	Explosion* pExplosion = &g_aExplosion[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion= 0; nCntExplosion< MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUsed == false)
		{
			pExplosion->pos = pos;
			pExplosion->col = col;
			pExplosion->nCountAnim = 1;
			pExplosion->nPatternAnim = 1;
			pExplosion->bUsed = true;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pExplosion->pos.x - TEXTURE_SCALE_X / 2, pExplosion->pos.y - TEXTURE_SCALE_X / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pExplosion->pos.x + TEXTURE_SCALE_X / 2, pExplosion->pos.y - TEXTURE_SCALE_X / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pExplosion->pos.x - TEXTURE_SCALE_X / 2, pExplosion->pos.y + TEXTURE_SCALE_X / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pExplosion->pos.x + TEXTURE_SCALE_X / 2, pExplosion->pos.y + TEXTURE_SCALE_X / 2, 0.0f);


			// ���_�J���[�̐ݒ�
			pVtx[0].col = pExplosion->col;
			pVtx[1].col = pExplosion->col;
			pVtx[2].col = pExplosion->col;
			pVtx[3].col = pExplosion->col;

			break;
		}
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}
//=====================================================================
// 
// �G�t�F�N�g���� [effect.cpp]
// Author:���O��
//
//=====================================================================
#include "sound.h"

#include "effect.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "score.h"

// �}�N����`
#define TEXTURE_FILENAME1			"data\\TEXTURE\\effect000.jpg"				// �e�N�X�`���̃t�@�C����
#define TEXTURE_SCALE_X				(64.0f)									// �e�N�X�`���̃T�C�YX
#define TEXTURE_SCALE_Y				(64.0f)									// �e�N�X�`���̃T�C�YY
#define TEXTURE_COLOR				D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f)		// �e�N�X�`���̐F

#define EFFECT_LIFETIME				(400.0f)								// �G�t�F�N�g���c�鎞��
#define EFFECT_SIZE					(30.0f)									// �G�t�F�N�g�̃T�C�Y�{��

// �O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureEffect[EFFECTTYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
Effect g_aEffect[MAX_EFFECT];

float g_fLengthEffect;				// �Ίp���̒���
float g_fAngleEffect;				// �Ίp���̊p�x

//=====================================================================
// ����������
//=====================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	Effect* pEffect = &g_aEffect[0];

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FILENAME1, &g_apTextureEffect[0]);

	// �G�t�F�N�g�̏��̏�����
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		pEffect[nCntEffect].pos = D3DXVECTOR3_ZERO;
		pEffect[nCntEffect].size = { TEXTURE_SCALE_X , TEXTURE_SCALE_Y, 0.0f };
		pEffect[nCntEffect].col = TEXTURE_COLOR;
		pEffect[nCntEffect].fRad = 0.0f;
		pEffect[nCntEffect].nLifeTime = EFFECT_LIFETIME;
		pEffect[nCntEffect].bUsed = false;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL
	);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos.x = pEffect->pos.x + sin(0 + g_fAngleEffect) * g_fLengthEffect;
		pVtx[0].pos.y = pEffect->pos.y + cos(0 + g_fAngleEffect) * g_fLengthEffect;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = pEffect->pos.x + sin(0 - g_fAngleEffect) * g_fLengthEffect;
		pVtx[1].pos.y = pEffect->pos.y + cos(0 - g_fAngleEffect) * g_fLengthEffect;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = pEffect->pos.x + sin(D3DX_PI - g_fAngleEffect) * g_fLengthEffect;
		pVtx[2].pos.y = pEffect->pos.y + cos(D3DX_PI - g_fAngleEffect) * g_fLengthEffect;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = pEffect->pos.x + sin(D3DX_PI + g_fAngleEffect) * g_fLengthEffect;
		pVtx[3].pos.y = pEffect->pos.y + cos(D3DX_PI + g_fAngleEffect) * g_fLengthEffect;
		pVtx[3].pos.z = 0.0f;

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = pEffect->col;
		pVtx[1].col = pEffect->col;
		pVtx[2].col = pEffect->col;
		pVtx[3].col = pEffect->col;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex.x = 0.0f;
		pVtx[0].tex.y = 0.0f;

		pVtx[1].tex.x = 1.0f;
		pVtx[1].tex.y = 0.0f;

		pVtx[2].tex.x = 0.0f;
		pVtx[2].tex.y = 1.0f;

		pVtx[3].tex.x = 1.0f;
		pVtx[3].tex.y = 1.0f;

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEffect->Unlock();
}

//=====================================================================
// �I������
//=====================================================================
void UninitEffect(void)
{
	// �e�N�X�`���̔j��
	for (int nCntEffect = 0; nCntEffect < EFFECTTYPE_MAX; nCntEffect++)
	{
		if (g_apTextureEffect[nCntEffect] != NULL)
		{
			g_apTextureEffect[nCntEffect]->Release();
			g_apTextureEffect[nCntEffect] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateEffect(void)
{
	int nCntEffect;
	VERTEX_2D* pVtx;
	Effect* pEffect = &g_aEffect[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUsed == true)
		{
			// �Ίp���̒������Z�o
			g_fLengthEffect = sqrtf(pEffect->size.x * pEffect->size.x + pEffect->size.y * pEffect->size.y) * 0.5 * pEffect->fScale;

			// �Ίp���̊p�x���Z�o
			g_fAngleEffect = atan2f(pEffect->size.x, pEffect->size.y);

			// ���_���W�̐ݒ�
			pVtx[0].pos.x = pEffect->pos.x + sin(0.0f + g_fAngleEffect) * g_fLengthEffect;
			pVtx[0].pos.y = pEffect->pos.y + cos(0.0f + g_fAngleEffect) * g_fLengthEffect;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = pEffect->pos.x + sin(0.0f - g_fAngleEffect) * g_fLengthEffect;
			pVtx[1].pos.y = pEffect->pos.y + cos(0.0f - g_fAngleEffect) * g_fLengthEffect;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = pEffect->pos.x + sin(D3DX_PI - g_fAngleEffect) * g_fLengthEffect;
			pVtx[2].pos.y = pEffect->pos.y + cos(D3DX_PI - g_fAngleEffect) * g_fLengthEffect;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = pEffect->pos.x + sin(D3DX_PI + g_fAngleEffect) * g_fLengthEffect;
			pVtx[3].pos.y = pEffect->pos.y + cos(D3DX_PI + g_fAngleEffect) * g_fLengthEffect;
			pVtx[3].pos.z = 0.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = pEffect->col;
			pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;
			pVtx[3].col = pEffect->col;

			pEffect->nLifeTime--;
			pEffect->fScale = (float)pEffect->nLifeTime / (float)pEffect->nMaxLifeTime;


			if (pEffect->nLifeTime == 0)
			{// �G�t�F�N�g�̎������s���������
				pEffect->bUsed = false;
			}
		}
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEffect->Unlock();
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUsed == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEffect[g_aEffect[nCntEffect].type]);

			 //�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=====================================================================
// �G�t�F�N�g�̐ݒ菈��
//=====================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRad, float fScale, int nLife)
{
	VERTEX_2D* pVtx;
	Effect* pEffect = &g_aEffect[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUsed == false)
		{
			// �ʒu�̍X�V
			pEffect->pos = pos;
			pEffect->size = { TEXTURE_SCALE_X , TEXTURE_SCALE_Y, 0.0f };
			pEffect->fScale = 1.0f;
			pEffect->col = col;
			pEffect->fRad = 0.0f;
			pEffect->fScale = fScale;
			pEffect->nLifeTime = nLife;
			pEffect->nMaxLifeTime = nLife;
			pEffect->type = EFFECTTYPE_NORMAL;
			pEffect->bUsed = true;

			// ���_���W�̐ݒ�
			pVtx[0].pos.x = pEffect->pos.x + sin(0 + g_fAngleEffect) * g_fLengthEffect;
			pVtx[0].pos.y = pEffect->pos.y + cos(0 + g_fAngleEffect) * g_fLengthEffect;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = pEffect->pos.x + sin(0 - g_fAngleEffect) * g_fLengthEffect;
			pVtx[1].pos.y = pEffect->pos.y + cos(0 - g_fAngleEffect) * g_fLengthEffect;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = pEffect->pos.x + sin(D3DX_PI - g_fAngleEffect) * g_fLengthEffect;
			pVtx[2].pos.y = pEffect->pos.y + cos(D3DX_PI - g_fAngleEffect) * g_fLengthEffect;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = pEffect->pos.x + sin(D3DX_PI + g_fAngleEffect) * g_fLengthEffect;
			pVtx[3].pos.y = pEffect->pos.y + cos(D3DX_PI + g_fAngleEffect) * g_fLengthEffect;
			pVtx[3].pos.z = 0.0f;

			break;
		}
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEffect->Unlock();
}
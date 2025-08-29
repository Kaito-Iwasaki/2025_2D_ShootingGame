//=====================================================================
// 
// �G�t�F�N�g���� [effect.cpp]
// Author:���O��
//
//=====================================================================
#include "effect.h"

//*******************************************
// �}�N����`
//*******************************************
#define DEF_TEXTURE_FILENAME			"data\\TEXTURE\\effect000.jpg"			// �e�N�X�`���̃t�@�C����
#define DEF_TEXTURE_SCALE_X				(64.0f)									// �e�N�X�`���̃T�C�YX
#define DEF_TEXTURE_SCALE_Y				(64.0f)									// �e�N�X�`���̃T�C�YY
#define DEF_TEXTURE_COLOR				D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f)		// �e�N�X�`���̐F

#define DEF_EFFECT_LIFETIME				(400.0f)								// �G�t�F�N�g���c�鎞��
#define DEF_EFFECT_SIZE					(30.0f)									// �G�t�F�N�g�̃T�C�Y�{��

//*******************************************
// �O���[�o���ϐ��錾
//*******************************************
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
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
	D3DXCreateTextureFromFile(pDevice, DEF_TEXTURE_FILENAME, &g_pTextureEffect);

	// �G�t�F�N�g�̏��̏�����
	ZeroMemory(pEffect, sizeof(pEffect) * MAX_EFFECT);
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//strcpy(pEffect[nCntEffect].cTextureFilePath, DEF_TEXTURE_FILENAME);

		pEffect[nCntEffect].pos = D3DXVECTOR3_ZERO;
		pEffect[nCntEffect].fRot = 0.0f;
		pEffect[nCntEffect].size = { DEF_TEXTURE_SCALE_X, DEF_TEXTURE_SCALE_Y, 0.0f };
		pEffect[nCntEffect].fScale = 0.0f;

		pEffect[nCntEffect].col = DEF_TEXTURE_COLOR;
		pEffect[nCntEffect].fRad = 0.0f;
		pEffect[nCntEffect].fLifeTime = DEF_EFFECT_LIFETIME;
		pEffect[nCntEffect].fMaxLifeTime = DEF_EFFECT_LIFETIME;
		pEffect[nCntEffect].bUseTexture = true;
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
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
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
			// �l�̐ݒ�
			pEffect->fLifeTime--;
			pEffect->pos.x = pEffect->pos.x + sin(pEffect->fRad) * pEffect->fSpeed;
			pEffect->pos.y = pEffect->pos.y + cos(pEffect->fRad) * pEffect->fSpeed;
			pEffect->fScale = pEffect->fMaxScale * (pEffect->fLifeTime / pEffect->fMaxLifeTime);
			pEffect->col.a = pEffect->fMaxAlpha * (pEffect->fLifeTime / pEffect->fMaxLifeTime);

			// �Ίp���̒������Z�o
			g_fLengthEffect = sqrtf(pEffect->size.x * pEffect->size.x + pEffect->size.y * pEffect->size.y) * 0.5 * pEffect->fScale;

			// �Ίp���̊p�x���Z�o
			g_fAngleEffect = atan2f(pEffect->size.x, pEffect->size.y);

			// ���_���W�̐ݒ�
			pVtx[0].pos.x = pEffect->pos.x + sin(pEffect->fRot + 0.0f + g_fAngleEffect) * g_fLengthEffect;
			pVtx[0].pos.y = pEffect->pos.y + cos(pEffect->fRot + 0.0f + g_fAngleEffect) * g_fLengthEffect;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = pEffect->pos.x + sin(pEffect->fRot + 0.0f - g_fAngleEffect) * g_fLengthEffect;
			pVtx[1].pos.y = pEffect->pos.y + cos(pEffect->fRot + 0.0f - g_fAngleEffect) * g_fLengthEffect;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = pEffect->pos.x + sin(pEffect->fRot + D3DX_PI - g_fAngleEffect) * g_fLengthEffect;
			pVtx[2].pos.y = pEffect->pos.y + cos(pEffect->fRot + D3DX_PI - g_fAngleEffect) * g_fLengthEffect;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = pEffect->pos.x + sin(pEffect->fRot + D3DX_PI + g_fAngleEffect) * g_fLengthEffect;
			pVtx[3].pos.y = pEffect->pos.y + cos(pEffect->fRot + D3DX_PI + g_fAngleEffect) * g_fLengthEffect;
			pVtx[3].pos.z = 0.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = pEffect->col;
			pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;
			pVtx[3].col = pEffect->col;

			if (pEffect->fLifeTime == 0)
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
			if (g_aEffect[nCntEffect].bUseTexture)
			{
				pDevice->SetTexture(0, g_pTextureEffect);
			}
			else
			{
				pDevice->SetTexture(0, NULL);
			}

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
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRad, float fSpeed, float fScale, float fLifeTime, bool bUseTexture)
{
	VERTEX_2D* pVtx;
	Effect* pEffect = &g_aEffect[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUsed == false)
		{
			// �Ίp���̒������Z�o
			g_fLengthEffect = sqrtf(pEffect->size.x * pEffect->size.x + pEffect->size.y * pEffect->size.y) * 0.5 * pEffect->fScale;

			// �Ίp���̊p�x���Z�o
			g_fAngleEffect = atan2f(pEffect->size.x, pEffect->size.y);

			// �ʒu�̍X�V
			pEffect->pos = pos;
			pEffect->fRot = 0.0f;
			pEffect->size = { DEF_TEXTURE_SCALE_X, DEF_TEXTURE_SCALE_Y, 0.0f };
			pEffect->fScale = fScale;
			pEffect->fMaxScale = fScale;

			pEffect->col = col;
			pEffect->fMaxAlpha = col.a;
			pEffect->fRad = fRad;
			pEffect->fSpeed = fSpeed;
			pEffect->fLifeTime = fLifeTime;
			pEffect->fMaxLifeTime = fLifeTime;
			pEffect->bUseTexture = bUseTexture;
			pEffect->bUsed = true;

			// ���_���W�̐ݒ�
			pVtx[0].pos.x = pEffect->pos.x + sin(pEffect->fRot + 0.0f + g_fAngleEffect) * g_fLengthEffect;
			pVtx[0].pos.y = pEffect->pos.y + cos(pEffect->fRot + 0.0f + g_fAngleEffect) * g_fLengthEffect;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = pEffect->pos.x + sin(pEffect->fRot + 0.0f - g_fAngleEffect) * g_fLengthEffect;
			pVtx[1].pos.y = pEffect->pos.y + cos(pEffect->fRot + 0.0f - g_fAngleEffect) * g_fLengthEffect;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = pEffect->pos.x + sin(pEffect->fRot + D3DX_PI - g_fAngleEffect) * g_fLengthEffect;
			pVtx[2].pos.y = pEffect->pos.y + cos(pEffect->fRot + D3DX_PI - g_fAngleEffect) * g_fLengthEffect;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = pEffect->pos.x + sin(pEffect->fRot + D3DX_PI + g_fAngleEffect) * g_fLengthEffect;
			pVtx[3].pos.y = pEffect->pos.y + cos(pEffect->fRot + D3DX_PI + g_fAngleEffect) * g_fLengthEffect;
			pVtx[3].pos.z = 0.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = pEffect->col;
			pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;
			pVtx[3].col = pEffect->col;

			break;
		}
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEffect->Unlock();
}

void ClearAllEffect(void)
{
	Effect* pEffect = &g_aEffect[0];

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		pEffect->bUsed = false;
	}
}
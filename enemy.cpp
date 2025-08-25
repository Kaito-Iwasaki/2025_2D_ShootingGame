//=====================================================================
// 
// �v���C���[�̒e���� [enemy.cpp]
// Author:���O��
//
//=====================================================================
#include "enemy.h"
#include "explosion.h"
#include "player.h"
#include "bullet.h"
#include "score.h"
#include "sound.h"
#include "collision.h"
#include "util.h"

// �}�N����`
#define TEXTURE1_FILENAME			"data\\TEXTURE\\enemy000.png"		// �G1�̃e�N�X�`��
#define TEXTURE2_FILENAME			"data\\TEXTURE\\enemy001.png"		// �G2�̃e�N�X�`��
#define TEXTURE3_FILENAME			"data\\TEXTURE\\enemy002.png"		// �G3�̃e�N�X�`��
#define TEXTURE4_FILENAME			"data\\TEXTURE\\enemy003.png"		// �G4�̃e�N�X�`��
#define TEXTURE_SCALE_X				(48.0f)
#define TEXTURE_SCALE_Y				(48.0f)
#define TEXTURE_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define TEXTURE_COLOR_DAMAGED		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)

#define NUM_ENEMY					(4)		// �G�̎��

//*********************************************************************
// �v���g�^�C�v�錾
//*********************************************************************
void EnemyAction(ENEMY* pEnemy);

// �O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
ENEMY g_aEnemy[MAX_ENEMY];									// �G���

float g_fLengthEnemy;				// �Ίp���̒���
float g_fAngleEnemy;				// �Ίp���̊p�x

//=====================================================================
// ����������
//=====================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	ENEMY* pEnemy = &g_aEnemy[0];
	int nCntEnemy;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE1_FILENAME, &g_apTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE2_FILENAME, &g_apTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE3_FILENAME, &g_apTextureEnemy[2]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE4_FILENAME, &g_apTextureEnemy[3]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL
	);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0, pEnemy = &g_aEnemy[0]; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		pEnemy->pos = D3DXVECTOR3_ZERO;
		pEnemy->posOffset = D3DXVECTOR3_ZERO;
		pEnemy->move = D3DXVECTOR3_ZERO;
		pEnemy->nType = 0;
		pEnemy->size = D3DXVECTOR3(TEXTURE_SCALE_X, TEXTURE_SCALE_Y, 0.0f);
		pEnemy->bUsed = false;
		pEnemy->nCounterState = 0;
		pEnemy->nCounterStep = 0;
		pEnemy->nShotInterval = 0;

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->size.x / 2, pEnemy->pos.y - pEnemy->size.y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->size.x / 2, pEnemy->pos.y - pEnemy->size.y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x - pEnemy->size.x / 2, pEnemy->pos.y + pEnemy->size.y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + pEnemy->size.x / 2, pEnemy->pos.y + pEnemy->size.y / 2, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = TEXTURE_COLOR;
		pVtx[1].col = TEXTURE_COLOR;
		pVtx[2].col = TEXTURE_COLOR;
		pVtx[3].col = TEXTURE_COLOR;

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
	g_pVtxBuffEnemy->Unlock();
}

//=====================================================================
// �I������
//=====================================================================
void UninitEnemy(void)
{
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		// �e�N�X�`���̔j��
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateEnemy(void)
{
	VERTEX_2D* pVtx;
	ENEMY* pEnemy = &g_aEnemy[0];
	Player* pPlayer = GetPlayer();

	float fDirection;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUsed == true)
		{
			// �v���C���[�Ƃ̏Փ˔���
			if (Collision(pEnemy->pos + pEnemy->posOffset, pEnemy->size, pPlayer->pos, pPlayer->size))
			{
				HitPlayer(1);
			}

			switch (pEnemy->nType)
			{
			case 0:
				pEnemy->posOffset.x = sin(pEnemy->nCounterStep / pEnemy->move.x) * 200.0f;
				pEnemy->pos.y += pEnemy->move.y;
				pEnemy->nCounterStep++;

				if (pEnemy->pos.y > SCREEN_HEIGHT + pEnemy->size.y / 2)
				{
					pEnemy->bUsed = false;
				}
				break;

			case 1:
				fDirection = atan2f(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);

				pEnemy->pos += D3DXVECTOR3(sin(fDirection), cos(fDirection), 0.0f) * 4.0f;

				break;

			case 2:
				pEnemy->nCounterUState++;

				if (pEnemy->pos.y < 100)
				{
					pEnemy->pos.y += 3;
				}

				if (pEnemy->nCounterUState % 100 == 0)
				{
					fDirection = atan2f(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);

					SetBullet(
						pEnemy->pos + pEnemy->posOffset,
						5.0f,
						fDirection - 0.5f,
						BULLETTYPE_ENEMY,
						BULLETPATTERN_SPHERE
					);
					SetBullet(
						pEnemy->pos + pEnemy->posOffset,
						5.0f,
						fDirection,
						BULLETTYPE_ENEMY,
						BULLETPATTERN_SPHERE
					);
					SetBullet(
						pEnemy->pos + pEnemy->posOffset,
						5.0f,
						fDirection + 0.5f,
						BULLETTYPE_ENEMY,
						BULLETPATTERN_SPHERE
					);
				}
				break;

			case 4:
				pEnemy->nCounterUState++;

				if (pEnemy->nCounterUState % 100 == 0)
				{
					fDirection = atan2f(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);

					SetBullet(
						pEnemy->pos + pEnemy->posOffset,
						3.0f,
						fDirection,
						BULLETTYPE_ENEMY,
						BULLETPATTERN_SPHERE
					);
				}

				pEnemy->pos.y += pEnemy->move.y;

				if (pEnemy->pos.y > SCREEN_HEIGHT + pEnemy->size.y / 2)
				{
					pEnemy->bUsed = false;
				}
				break;

			case 5:
				pEnemy->nCounterUState++;

				if (pEnemy->pos.y < 100)
				{
					pEnemy->pos.y += 3;
				}
				else
				{
					if (pEnemy->nCounterUState % 3 == 0)
					{
						SetBullet(
							pEnemy->pos + pEnemy->posOffset,
							RandRange(5,10),
							(float)RandRange(0, 314 * 2) * 0.01f,
							BULLETTYPE_ENEMY,
							BULLETPATTERN_SPHERE
						);
					};

					if (pEnemy->pos.x < SCREEN_WIDTH / 2 - 500 || pEnemy->pos.x > SCREEN_WIDTH / 2 + 500)
					{
						pEnemy->move *= -1;
					}

					pEnemy->pos.x += pEnemy->move.x;

					break;
				}
				break;
				
			default:
				pEnemy->pos.y += pEnemy->move.y;

				if (pEnemy->pos.y > SCREEN_HEIGHT + pEnemy->size.y / 2)
				{
					pEnemy->bUsed = false;
				}
			}

			switch (pEnemy->state)
			{
			case ENEMYSTATE_NORMAL:
				break;

			case ENEMYSTATE_DAMAGED:
				pEnemy->nCounterState--;

				if (pEnemy->nCounterState < 1)
				{
					pEnemy->state = ENEMYSTATE_NORMAL;

					// ���_�J���[�̐ݒ�
					pVtx[0].col = TEXTURE_COLOR;
					pVtx[1].col = TEXTURE_COLOR;
					pVtx[2].col = TEXTURE_COLOR;
					pVtx[3].col = TEXTURE_COLOR;
				}
				break;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3((pEnemy->pos.x + pEnemy->posOffset.x) - pEnemy->size.x / 2, (pEnemy->pos.y + pEnemy->posOffset.y) - pEnemy->size.y / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((pEnemy->pos.x + pEnemy->posOffset.x) + pEnemy->size.x / 2, (pEnemy->pos.y + pEnemy->posOffset.y) - pEnemy->size.y / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((pEnemy->pos.x + pEnemy->posOffset.x) - pEnemy->size.x / 2, (pEnemy->pos.y + pEnemy->posOffset.y) + pEnemy->size.y / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((pEnemy->pos.x + pEnemy->posOffset.x) + pEnemy->size.x / 2, (pEnemy->pos.y + pEnemy->posOffset.y) + pEnemy->size.y / 2, 0.0f);
		}
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEnemy->Unlock();
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUsed == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nTex]);

			 //�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//=====================================================================
// �G�̐ݒ菈��
//=====================================================================
ENEMY* SetEnemy(D3DXVECTOR3 pos, int nType)
{
	VERTEX_2D* pVtx;
	ENEMY* pEnemy = &g_aEnemy[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUsed == false)
		{
			// �e�l��������
			pEnemy->pos = pos;
			pEnemy->posOffset = D3DXVECTOR3_ZERO;
			pEnemy->move = { 5.0f, 4.0f, 0.0f };
			pEnemy->size = D3DXVECTOR3(TEXTURE_SCALE_X, TEXTURE_SCALE_Y, 0.0f);
			pEnemy->nType = nType;
			pEnemy->nLife = 1;
			pEnemy->state = ENEMYSTATE_NORMAL;
			pEnemy->nCounterState = 0;
			pEnemy->nCounterUState = 0;
			pEnemy->nCounterStep = 0;
			pEnemy->nShotInterval = 0;
			pEnemy->bUsed = true;

			switch (pEnemy->nType)
			{
			case 0:
				pEnemy->nTex = 0;
				break;

			case 1:
				pEnemy->nTex = 1;
				break;

			case 2:
				pEnemy->nTex = 2;
				break;

			case 3:
				pEnemy->nTex = 3;
				break;

			case 4:
				pEnemy->nTex = 2;
				break;

			case 5:
				pEnemy->nTex = 0;
				pEnemy->size *= 2;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3((pEnemy->pos.x + pEnemy->posOffset.x) - pEnemy->size.x / 2, (pEnemy->pos.y + pEnemy->posOffset.y) - pEnemy->size.y / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((pEnemy->pos.x + pEnemy->posOffset.x) + pEnemy->size.x / 2, (pEnemy->pos.y + pEnemy->posOffset.y) - pEnemy->size.y / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((pEnemy->pos.x + pEnemy->posOffset.x) - pEnemy->size.x / 2, (pEnemy->pos.y + pEnemy->posOffset.y) + pEnemy->size.y / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((pEnemy->pos.x + pEnemy->posOffset.x) + pEnemy->size.x / 2, (pEnemy->pos.y + pEnemy->posOffset.y) + pEnemy->size.y / 2, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = TEXTURE_COLOR;
			pVtx[1].col = TEXTURE_COLOR;
			pVtx[2].col = TEXTURE_COLOR;
			pVtx[3].col = TEXTURE_COLOR;

			break;
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEnemy->Unlock();

	return pEnemy;
}

//=====================================================================
// �G�̃q�b�g����
//=====================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife < 1)
	{
		SetExplosion(g_aEnemy[nCntEnemy].pos + g_aEnemy[nCntEnemy].posOffset, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f));
		g_aEnemy[nCntEnemy].bUsed = false;
		AddScore(100);
		PlaySound(SOUND_LABEL_SE_HIT000);
	}
	else if (g_aEnemy[nCntEnemy].state != ENEMYSTATE_DAMAGED)
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGED;
		g_aEnemy[nCntEnemy].nCounterState = 5;

		pVtx += (nCntEnemy * 4);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = TEXTURE_COLOR_DAMAGED;
		pVtx[1].col = TEXTURE_COLOR_DAMAGED;
		pVtx[2].col = TEXTURE_COLOR_DAMAGED;
		pVtx[3].col = TEXTURE_COLOR_DAMAGED;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEnemy->Unlock();
}


//=====================================================================
// �G���̎擾
//=====================================================================
ENEMY* GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=====================================================================
// �G�̎c�������擾
//=====================================================================
int GetEnemyLeft(void)
{
	int nCntEnemy;
	int nLeft = 0;

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUsed == true)
		{
			nLeft++;
		}
	}

	return nLeft;
}
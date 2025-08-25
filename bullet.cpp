//=====================================================================
// 
// �v���C���[�̒e���� [bullet.cpp]
// Author:���O��
//
//=====================================================================
#include "sound.h"

#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "score.h"
#include "effect.h"
#include "collision.h"

// �}�N����`
#define TEXTURE_FILENAME1			"data\\TEXTURE\\bullet000.png"				// �e�N�X�`���̃t�@�C����
#define TEXTURE_FILENAME2			"data\\TEXTURE\\enemybullet000.png"		// �e�N�X�`���̃t�@�C����
#define TEXTURE_FILENAME3			"data\\TEXTURE\\enemybullet001.png"		// �e�N�X�`���̃t�@�C����
#define TEXTURE_SCALE_X				(8.0f)									// �e�N�X�`���̃T�C�YX
#define TEXTURE_SCALE_Y				(64.0f)									// �e�N�X�`���̃T�C�YY
#define TEXTURE_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)		// �e�N�X�`���̐F

#define BULLET_LIFETIME				(400.0f)								// �e���c�鎞��
#define BULLET_SIZE					(30.0f)									// �e�̃T�C�Y�{��

// �v���g�^�C�v�錾
void CollisionEnemy(Bullet* pBullet);
void CollisionPlayer(Bullet* pBullet);
void CollisionBullet(Bullet* pBullet);

// �O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureBullet[BULLETPATTERN_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET];

float g_fLengthBullet;				// �Ίp���̒���
float g_fAngleBullet;				// �Ίp���̊p�x

//=====================================================================
// ����������
//=====================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	Bullet* pBullet = &g_aBullet[0];
	int nCntBullet;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FILENAME1, &g_apTextureBullet[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FILENAME2, &g_apTextureBullet[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FILENAME3, &g_apTextureBullet[2]);

	// �e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		pBullet[nCntBullet].pos = D3DXVECTOR3_ZERO;
		pBullet[nCntBullet].size = { TEXTURE_SCALE_X, TEXTURE_SCALE_Y, 0.0f };
		pBullet[nCntBullet].fMove = 0.0f;
		pBullet[nCntBullet].fRot = 0.0f;
		pBullet[nCntBullet].nLifeTime = BULLET_LIFETIME;
		pBullet[nCntBullet].nType = BULLETTYPE_PLAYER;
		pBullet[nCntBullet].pattern = BULLETPATTERN_PLAYER;
		pBullet[nCntBullet].bUsed = false;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pBullet->pos.x - pBullet->size.x / 2, pBullet->pos.y - pBullet->size.y / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pBullet->pos.x + pBullet->size.x / 2, pBullet->pos.y - pBullet->size.y / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pBullet->pos.x - pBullet->size.x / 2, pBullet->pos.y + pBullet->size.y / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pBullet->pos.x + pBullet->size.x / 2, pBullet->pos.y + pBullet->size.y / 2, 0.0f);

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
	g_pVtxBuffBullet->Unlock();
}

//=====================================================================
// �I������
//=====================================================================
void UninitBullet(void)
{
	// �e�N�X�`���̔j��
	for (int nCntBullet = 0; nCntBullet < BULLETPATTERN_MAX; nCntBullet++)
	{
		if (g_apTextureBullet[nCntBullet] != NULL)
		{
			g_apTextureBullet[nCntBullet]->Release();
			g_apTextureBullet[nCntBullet] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateBullet(void)
{
	int nCntBullet;
	VERTEX_2D* pVtx;
	Bullet* pBullet = &g_aBullet[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUsed == true)
		{
			// �Ίp���̒������Z�o
			g_fLengthBullet = sqrtf(pBullet->size.x * pBullet->size.x + pBullet->size.y * pBullet->size.y) * 0.25;

			// �Ίp���̊p�x���Z�o
			g_fAngleBullet = atan2f(pBullet->size.x, pBullet->size.y);

			// �ʒu�̍X�V
			pBullet->pos.x += pBullet->fMove * sinf(pBullet->fRot);
			pBullet->pos.y += pBullet->fMove * cosf(pBullet->fRot);

			// ���_���W�̐ݒ�
			pVtx[0].pos.x = pBullet->pos.x + sin(pBullet->fRot + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.y = pBullet->pos.y + cos(pBullet->fRot + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = pBullet->pos.x + sin(pBullet->fRot - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.y = pBullet->pos.y + cos(pBullet->fRot - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = pBullet->pos.x + sin(pBullet->fRot + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.y = pBullet->pos.y + cos(pBullet->fRot + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = pBullet->pos.x + sin(pBullet->fRot + D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.y = pBullet->pos.y + cos(pBullet->fRot + D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.z = 0.0f;

			// �����蔻�菈��
			switch (pBullet->nType)
			{
			case BULLETTYPE_PLAYER:
				CollisionEnemy(pBullet);
				CollisionBullet(pBullet);

				break;

			case BULLETTYPE_ENEMY:
				CollisionPlayer(pBullet);
				break;
			}

			pBullet->nLifeTime--;
			if (pBullet->pos.x < 0 || pBullet->pos.x > SCREEN_WIDTH || pBullet->pos.y < 0 || pBullet->pos.y > SCREEN_HEIGHT)
			{// �e����ʊO�ɗ��������
				pBullet->bUsed = false;
			}
			else if (pBullet->nLifeTime == 0)
			{// �e�̎������s���������
				SetExplosion(pBullet->pos, D3DXCOLOR(100, 100, 100, 255));
				pBullet->bUsed = false;
			}
		}
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBullet->Unlock();
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUsed == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureBullet[g_aBullet[nCntBullet].pattern]);

			 //�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//=====================================================================
// �e�̐ݒ菈��
//=====================================================================
void SetBullet(D3DXVECTOR3 pos, float fMove, float fRot, BULLETTYPE type, BULLETPATTERN pattern)
{
	VERTEX_2D* pVtx;
	Bullet* pBullet = &g_aBullet[0];
	int nCntBullet;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUsed == false)
		{
			// �ʒu�̍X�V
			pBullet->pos = pos;
			pBullet->fMove = fMove;
			pBullet->fRot = fRot;
			pBullet->nLifeTime = BULLET_LIFETIME;
			pBullet->nType = type;
			pBullet->pattern = pattern;
			pBullet->bUsed = true;

			switch (pattern)
			{
			case BULLETPATTERN_PLAYER:
				pBullet->nLife = 2;
				pBullet->size = { TEXTURE_SCALE_X, TEXTURE_SCALE_Y, 0.0f };
				break;

			default:
				pBullet->nLife = 1;
				pBullet->size = { 24.0f, 24.0f, 0.0f };
				break;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos.x = pBullet->pos.x + sin(pBullet->fRot + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.y = pBullet->pos.y + cos(pBullet->fRot + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = pBullet->pos.x + sin(pBullet->fRot - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.y = pBullet->pos.y + cos(pBullet->fRot - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = pBullet->pos.x + sin(pBullet->fRot + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.y = pBullet->pos.y + cos(pBullet->fRot + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = pBullet->pos.x + sin(pBullet->fRot + D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.y = pBullet->pos.y + cos(pBullet->fRot + D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.z = 0.0f;

			break;
		}
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBullet->Unlock();
}

//=====================================================================
// �G�̓����蔻�菈��
//=====================================================================
void CollisionEnemy(Bullet* pBullet)
{
	// �G�̎擾
	ENEMY* pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUsed == true)
		{
			if (Collision(pBullet->pos, pBullet->size, pEnemy->pos + pEnemy->posOffset, pEnemy->size))
			{// �G�ɒe����������
				HitEnemy(nCntEnemy, 1);

				pBullet->bUsed = false;
			}
		}
	}
}

//=====================================================================
// �v���C���[�̓����蔻�菈��
//=====================================================================
void CollisionPlayer(Bullet* pBullet)
{
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	if (IsPlayerDead() == true)
	{
		return;
	}

	if (Collision(pBullet->pos, pBullet->size, pPlayer->pos, pPlayer->size))
	{// �v���C���[�ɒe����������
		HitPlayer(1);

		pBullet->bUsed = false;
	}
}

//=====================================================================
// �e�ƒe�̓����蔻�菈��
//=====================================================================
void CollisionBullet(Bullet* pBullet)
{
	// �G�̎擾
	Bullet* pEnemyBullet = &g_aBullet[0];

	for (int nCntBullet = 0; nCntBullet < MAX_ENEMY; nCntBullet++, pEnemyBullet++)
	{
		if (pEnemyBullet->nType == BULLETTYPE_ENEMY && pEnemyBullet->bUsed == true)
		{
			if (Collision(pBullet->pos, pBullet->size, pEnemyBullet->pos, pEnemyBullet->size))
			{// �G�ɒe����������

				if (pEnemyBullet->nLife < 1)
				{
					pEnemyBullet->bUsed = false;

					SetExplosion(pBullet->pos, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
					AddScore(10);
					PlaySound(SOUND_LABEL_SE_HIT000);

				}
				else
				{
					pEnemyBullet->nLife--;
				}

				if (pBullet->nLife < 1)
				{
					pBullet->bUsed = false;
				}
				else
				{
					pBullet->nLife--;
				}
			}
		}
	}
}
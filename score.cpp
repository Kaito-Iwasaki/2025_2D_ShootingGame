//=====================================================================
// 
// �����G�t�F�N�g���� [explosion.cpp]
// Author:���O��
//
//=====================================================================
#include "score.h"
#include <math.h>

// �}�N����`
#define TEXTURE_FILENAME			"data\\TEXTURE\\number000.png"
#define TEXTURE_SCALE_X				(16.0f)
#define TEXTURE_SCALE_Y				(32.0f)
#define TEXTURE_COLOR				D3DCOLOR_RGBA(255, 255, 255, 255)
#define TEXTURE_MAX_X				(10)									// �e�N�X�`���p�^�[����X
#define TEXTURE_MAX_Y				(1)										// �e�N�X�`���p�^�[����Y
#define TEXTURE_MAX_PATTERN			(TEXTURE_MAX_X * TEXTURE_MAX_Y)			// �e�N�X�`�����p�^�[����
#define TEXTURE_INTERVAL			(3)										// �p�^�[���؂�ւ��t���[����

// �O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posScore;
int g_nScore;

//=====================================================================
// ����������
//=====================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	int nCntScore;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_FILENAME, &g_pTextureScore);

	// �X�R�A�̏��̏�����
	g_posScore = D3DXVECTOR3(10.0f, 10.0f, 0.0f);
	g_nScore = 0;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL
	);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x + (nCntScore * TEXTURE_SCALE_X), g_posScore.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x + ((nCntScore + 1) * TEXTURE_SCALE_X), g_posScore.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x + (nCntScore * TEXTURE_SCALE_X), g_posScore.y + TEXTURE_SCALE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x + ((nCntScore + 1) * TEXTURE_SCALE_X), g_posScore.y + TEXTURE_SCALE_Y, 0.0f);

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

		pVtx[1].tex.x = 1.0f / TEXTURE_MAX_X;
		pVtx[1].tex.y = 0.0f;

		pVtx[2].tex.x = 0.0f;
		pVtx[2].tex.y = 1.0f / TEXTURE_MAX_Y;

		pVtx[3].tex.x = 1.0f / TEXTURE_MAX_X;
		pVtx[3].tex.y = 1.0f / TEXTURE_MAX_Y;

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();
}

//=====================================================================
// �I������
//=====================================================================
void UninitScore(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateScore(void)
{

}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureScore);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//=====================================================================
// �X�R�A�̐ݒ菈��
//=====================================================================
void SetScore(int nScore)
{
	VERTEX_2D* pVtx;
	int aTexU[NUM_PLACE];
	int nCntScore;

	g_nScore = nScore;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		aTexU[nCntScore] = g_nScore % (int)pow(10, (double)NUM_PLACE - nCntScore) / (int)pow(10, (double)NUM_PLACE - 1 - nCntScore);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex.x = 1.0f / TEXTURE_MAX_X * aTexU[nCntScore];
		pVtx[0].tex.y = 0.0f;

		pVtx[1].tex.x = 1.0f / TEXTURE_MAX_X * (aTexU[nCntScore] + 1);
		pVtx[1].tex.y = 0.0f;

		pVtx[2].tex.x = 1.0f / TEXTURE_MAX_X * aTexU[nCntScore];
		pVtx[2].tex.y = 1.0f;

		pVtx[3].tex.x = 1.0f / TEXTURE_MAX_X * (aTexU[nCntScore] + 1);
		pVtx[3].tex.y = 1.0f;

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();
}

//=====================================================================
// �X�R�A�̉��Z����
//=====================================================================
void AddScore(int nValue)
{
	VERTEX_2D* pVtx;
	int aTexU[NUM_PLACE];
	int nCntScore;

	g_nScore += nValue;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		aTexU[nCntScore] = g_nScore % (int)pow(10, (double)NUM_PLACE - nCntScore) / (int)pow(10, (double)NUM_PLACE - 1 - nCntScore);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex.x = 1.0f / TEXTURE_MAX_X * aTexU[nCntScore];
		pVtx[0].tex.y = 0.0f;

		pVtx[1].tex.x = 1.0f / TEXTURE_MAX_X * (aTexU[nCntScore] + 1);
		pVtx[1].tex.y = 0.0f;

		pVtx[2].tex.x = 1.0f / TEXTURE_MAX_X * aTexU[nCntScore];
		pVtx[2].tex.y = 1.0f;

		pVtx[3].tex.x = 1.0f / TEXTURE_MAX_X * (aTexU[nCntScore] + 1);
		pVtx[3].tex.y = 1.0f;

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();
}

int GetScore(void)
{
	return g_nScore;
}
//=====================================================================
// 
// �w�i���� [title.cpp]
// Author:���O��
//
//=====================================================================
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

// �}�N����`
#define TEXTURE_FILENAME		"data\\TEXTURE\\title000.png"
#define TEXTURE_POS_X			(SCREEN_WIDTH / 2)
#define TEXTURE_POS_Y			(SCREEN_HEIGHT / 2)
#define TEXTURE_SCALE_X			(SCREEN_WIDTH)
#define TEXTURE_SCALE_Y			(SCREEN_HEIGHT)
#define TEXTURE_COLOR			D3DCOLOR_RGBA(255, 255, 255, 255)

// �O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureTitle = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
Title g_title;

//=====================================================================
// ����������
//=====================================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^
	Title* pTitle = &g_title;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE_FILENAME,
		&g_pTextureTitle
	);

	// �e�̏��̏�����
	pTitle->pos = D3DXVECTOR3(TEXTURE_POS_X, TEXTURE_POS_Y, 0.0f);
	pTitle->col = TEXTURE_COLOR;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL
	);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pTitle->pos.x - TEXTURE_SCALE_X / 2, pTitle->pos.y - TEXTURE_SCALE_Y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pTitle->pos.x + TEXTURE_SCALE_X / 2, pTitle->pos.y - TEXTURE_SCALE_Y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pTitle->pos.x - TEXTURE_SCALE_X / 2, pTitle->pos.y + TEXTURE_SCALE_Y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pTitle->pos.x + TEXTURE_SCALE_X / 2, pTitle->pos.y + TEXTURE_SCALE_Y / 2, 0.0f);

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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle->Unlock();

	PlaySound(SOUND_LABEL_BGM003);
}
//=====================================================================
// �I������
//=====================================================================
void UninitTitle(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateTitle(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadPress(JOYKEY_START))
	{
		SetFade(MODE_STAGE1);
	}
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTitle);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
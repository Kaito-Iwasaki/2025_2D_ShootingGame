//=====================================================================
// 
// �v���C���[���� [player.cpp]
// Author:���O��
//
//=====================================================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "util.h"

#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"

// �}�N����`
#define TEXTURE_FILENAME		"data\\TEXTURE\\player000.png"				// �e�N�X�`���t�@�C����
#define TEXTURE_POS_X			(SCREEN_WIDTH / 2)						// �|���S���ʒuX
#define TEXTURE_POS_Y			(SCREEN_HEIGHT * 0.9)					// �|���S���ʒuY
#define TEXTURE_SCALE_X			(64.0f)									// �|���S���T�C�YX
#define TEXTURE_SCALE_Y			(64.0f)									// �|���S���T�C�YY
#define TEXTURE_SCALE_MIN		(0.5f)									// �|���S���ő�k����
#define TEXTURE_SCALE_MAX		(1.5f)									// �|���S���ő�g�嗦
#define TEXTURE_MAX_X			(1)										// �e�N�X�`���p�^�[����X
#define TEXTURE_MAX_Y			(1)										// �e�N�X�`���p�^�[����Y
#define TEXTURE_MAX_PATTERN		(TEXTURE_MAX_X * TEXTURE_MAX_Y)			// �e�N�X�`�����p�^�[����
#define TEXTURE_INTERVAL		(1)										// �p�^�[���؂�ւ��t���[����
#define TEXTURE_COLOR			D3DCOLOR_RGBA(255, 255, 255, 255);

#define PLAYER_SPEED			(8.0f)									// �v���C���[�̃X�s�[�h
#define SHOOT_SPEED				(20.0f)									// �e��	
#define SHOOT_INTERVAL			(5)										// �e���˂̊Ԋu

// �v���g�^�C�v�錾
void ShootBullet(void);

// �O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer;		// ���_�o�b�t�@�ւ̃|�C���^
Player g_aPlayer;

float g_fLengthPlayer;
float g_fAnglePlayer;

//=====================================================================
// ����������
//=====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx;			// �f�o�C�X

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �l�̏�����
	g_aPlayer.pos = { TEXTURE_POS_X, TEXTURE_POS_Y, 0.0f};
	g_aPlayer.posMove = { 0.0f, 0.0f, 0.0f };
	g_aPlayer.fRot = 0.0f;
	g_aPlayer.fRotMove = 0.0f;
	g_aPlayer.size = { TEXTURE_SCALE_X, TEXTURE_SCALE_Y, 0.0f };

	g_aPlayer.state = PLAYERSTATE_NORMAL;
	g_aPlayer.nCountState = 0;
	g_aPlayer.fLife = 3.0f;
	g_aPlayer.nLastShot = 0;
	g_aPlayer.nUpgrade = 0;
	g_aPlayer.nCharge = 0;

	g_aPlayer.bVisible = true;

	// �Ίp���̒������Z�o
	g_fLengthPlayer = sqrtf(g_aPlayer.size.x * g_aPlayer.size.x + g_aPlayer.size.y * g_aPlayer.size.y) * 0.5f;

	// �Ίp���̊p�x���Z�o
	g_fAnglePlayer = atan2f(g_aPlayer.size.x, g_aPlayer.size.y);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE_FILENAME,
		&g_pTexturePlayer
	);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL
	);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.pos.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_aPlayer.pos.x + cosf(g_aPlayer.pos.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.pos.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_aPlayer.pos.x + cosf(g_aPlayer.pos.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.pos.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_aPlayer.pos.x + cosf(g_aPlayer.pos.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.pos.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_aPlayer.pos.x + cosf(g_aPlayer.pos.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

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
	pVtx[0].tex.x = 1.0f / TEXTURE_MAX_X * g_aPlayer.nPatternAnim;
	pVtx[0].tex.y = 1.0f / TEXTURE_MAX_Y * (g_aPlayer.nPatternAnim / TEXTURE_MAX_X);

	pVtx[1].tex.x = 1.0f / TEXTURE_MAX_X * (g_aPlayer.nPatternAnim + 1);
	pVtx[1].tex.y = 1.0f / TEXTURE_MAX_Y * (g_aPlayer.nPatternAnim / TEXTURE_MAX_X);

	pVtx[2].tex.x = 1.0f / TEXTURE_MAX_X * g_aPlayer.nPatternAnim;
	pVtx[2].tex.y = 1.0f / TEXTURE_MAX_Y * (g_aPlayer.nPatternAnim / TEXTURE_MAX_X + 1);

	pVtx[3].tex.x = 1.0f / TEXTURE_MAX_X * (g_aPlayer.nPatternAnim + 1);
	pVtx[3].tex.y = 1.0f / TEXTURE_MAX_Y * (g_aPlayer.nPatternAnim / TEXTURE_MAX_X + 1);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();
}

//=====================================================================
// �I������
//=====================================================================
void UninitPlayer(void)
{
	// �e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=====================================================================
// �X�V����
//=====================================================================
void UpdatePlayer(void)
{
	VERTEX_2D* pVtx;
	D3DXVECTOR3 keyboardInput = D3DXVECTOR3_ZERO;
	D3DXVECTOR3 joypadInput = D3DXVECTOR3_ZERO;
	XINPUT_STATE* joypad = GetJoypad();

	//**************************************************
	// �v���C���[�̏�Ԃɂ�鏈��
	//**************************************************
	switch (g_aPlayer.state)
	{
	case PLAYERSTATE_NORMAL:		// �ʏ�
		g_aPlayer.bVisible = true;
		
		if (g_aPlayer.fLife < 3)
		{
			g_aPlayer.fLife += 0.001f;
		}
		break;

	case PLAYERSTATE_DAMAGED:		// �_���[�W���
		g_aPlayer.nCountState++;


		if (g_aPlayer.nCountState % 5 == 0)
		{
			g_aPlayer.bVisible = g_aPlayer.bVisible ^ 1;
		}

		if (g_aPlayer.nCountState > 20)
		{
			SetVibration(0, 0);
		}
		else
		{
			SetVibration(60000, 60000);
		}

		if (g_aPlayer.nCountState > 100)
		{
			g_aPlayer.state = PLAYERSTATE_NORMAL;
			g_aPlayer.nCountState = 0;
			g_aPlayer.bVisible = true;
		}
		break;

	case PLAYERSTATE_DEAD:		// ���S���
		g_aPlayer.bVisible = false;

		SetVibration(60000, 60000);

		g_aPlayer.nCountState++;

		if (g_aPlayer.nCountState > 100)
		{
			g_aPlayer.state = PLAYERSTATE_END;
		}
		break;

	case PLAYERSTATE_END:		// �I��
		SetVibration(0, 0);
		break;
	}

	if (IsPlayerDead() == true)
	{// �v���C���[������ł����珈���𒆒f
		return;
	}

	//**************************************************
	// �L�[�{�[�h���R���g���[���[����
	//**************************************************
	float fThumbLX = joypad->Gamepad.sThumbLX;		// ���X�e�B�b�N��X�ʒu
	float fThumbLY = joypad->Gamepad.sThumbLY;		// ���X�e�B�b�N��Y�ʒu
	float fNormalizedLX;							// ���K�����ꂽ���X�e�B�b�N��X�ʒu
	float fNormalizedLY;							// ���K�����ꂽ���X�e�B�b�N��Y�ʒu
	float fJoypadMagnitude;							// �W���C�p�b�h�ł̈ړ���
	float fKeyboardMagnitude;						// �L�[�{�[�h�ł̈ړ���

	// �W���C�p�b�h�ł̈ړ��ʂ��擾
	fJoypadMagnitude = sqrt(fThumbLX * fThumbLX + fThumbLY * fThumbLY);

	if (fJoypadMagnitude > INPUT_DEADZONE)
	{// �f�b�h�]�[�����Ŗ�����Έړ��ʂ𐳋K��
		fNormalizedLX = (fThumbLX) / (32767 - INPUT_DEADZONE);
		fNormalizedLY = (fThumbLY) / (32767 - INPUT_DEADZONE);

	}
	else
	{// �f�b�h�]�[�����ł���Έړ��ʂ�0�ɂ���i���͂𖳌��ɂ���j
		fJoypadMagnitude = 0.0f;
		fNormalizedLX = 0.0f;
		fNormalizedLY = 0.0f;
	}

	// �L�[�{�[�h&�\���L�[����
	if (GetKeyboardPress(DIK_UPARROW) == true || GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
	{// ��
		keyboardInput.y -= 1;
	}
	if (GetKeyboardPress(DIK_DOWNARROW) == true || GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
	{// ��
		keyboardInput.y += 1;
	}
	if (GetKeyboardPress(DIK_LEFTARROW) == true || GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
	{// ��
		keyboardInput.x -= 1;
	}
	if (GetKeyboardPress(DIK_RIGHTARROW) == true || GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
	{// �E
		keyboardInput.x += 1;
	}

	// �L�[�{�[�h�ł̈ړ��ʂ��擾
	fKeyboardMagnitude = sqrtf(keyboardInput.x * keyboardInput.x + keyboardInput.y * keyboardInput.y);

	if (fKeyboardMagnitude == 0)
	{// �L�[�{�[�h�ł̈ړ��ʂ�0�ɂȂ��Ă�����1�ɂ���i0���Z���Ȃ��悤�Ɂj
		fKeyboardMagnitude = 1;
	}

	if (fJoypadMagnitude != 0)
	{// �����W���C�p�b�h�ł̓��͂����f����
		g_aPlayer.pos += D3DXVECTOR3(fNormalizedLX, -fNormalizedLY, 0.0f) * PLAYER_SPEED;
	}
	else
	{// �W���C�p�b�h�̓��͂��Ȃ���΃L�[�{�[�h�ł̓��͂𔽉f����
		g_aPlayer.pos += D3DXVECTOR3(keyboardInput.x / fKeyboardMagnitude, keyboardInput.y / fKeyboardMagnitude, 0.0f) * PLAYER_SPEED;
	}

	// �V���b�g
	g_aPlayer.nLastShot += 1;
	if (GetKeyboardPress(DIK_H) || GetKeyboardPress(DIK_J) || GetJoypadPress(JOYKEY_A) || GetJoypadPress(JOYKEY_X))
	{// �X�y�[�X�L�[�������ꂽ
		if (g_aPlayer.nLastShot >= SHOOT_INTERVAL)
		{// �O�񔭎˂��Ă���SHOOT_INTERVAL���o�߂��Ă����甭��
			g_aPlayer.nLastShot = 0;

			if (GetKeyboardPress(DIK_H) || GetJoypadPress(JOYKEY_A))
			{
				g_aPlayer.nUpgrade = 0;
			}
			else if (GetKeyboardPress(DIK_J) || GetJoypadPress(JOYKEY_X))
			{
				g_aPlayer.nUpgrade = 1;
			}
			ShootBullet();
		}
	}

	if (g_aPlayer.pos.x < g_aPlayer.size.x / 2)
	{// ������
		g_aPlayer.pos.x = g_aPlayer.size.x / 2;
	}
	if (g_aPlayer.pos.x  > SCREEN_WIDTH - g_aPlayer.size.x / 2)
	{// �E����
		g_aPlayer.pos.x = SCREEN_WIDTH - g_aPlayer.size.x / 2;
	}
	if (g_aPlayer.pos.y < g_aPlayer.size.y / 2)
	{// �㐧��
		g_aPlayer.pos.y = g_aPlayer.size.y / 2;
	}
	if (g_aPlayer.pos.y > SCREEN_HEIGHT - g_aPlayer.size.y / 2)
	{// ������
		g_aPlayer.pos.y = SCREEN_HEIGHT - g_aPlayer.size.y / 2;
	}

	//**************************************************
	// �|���S���ʒu�̐ݒ�
	//**************************************************
	
	// �A�j���[�V�����̃J�E���g�𑝉�
	g_aPlayer.nCountAnim++;
	if (g_aPlayer.nCountAnim % TEXTURE_INTERVAL == 0)
	{
		g_aPlayer.nPatternAnim = (g_aPlayer.nPatternAnim + 1) % TEXTURE_MAX_PATTERN;
	}

	// �Ίp���̒������Z�o
	g_fLengthPlayer = sqrtf(TEXTURE_SCALE_X * TEXTURE_SCALE_X + TEXTURE_SCALE_Y * TEXTURE_SCALE_X) * 0.5;

	// �Ίp���̊p�x���Z�o
	g_fAnglePlayer = atan2f(TEXTURE_SCALE_X, TEXTURE_SCALE_Y);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.fRot + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.fRot + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.fRot + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.fRot + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.fRot - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.fRot - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.fRot + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.fRot + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex.x = 1.0f / TEXTURE_MAX_X * g_aPlayer.nPatternAnim;
	pVtx[0].tex.y = 1.0f / TEXTURE_MAX_Y * (g_aPlayer.nPatternAnim / TEXTURE_MAX_X);

	pVtx[1].tex.x = 1.0f / TEXTURE_MAX_X * (g_aPlayer.nPatternAnim + 1);
	pVtx[1].tex.y = 1.0f / TEXTURE_MAX_Y * (g_aPlayer.nPatternAnim / TEXTURE_MAX_X);

	pVtx[2].tex.x = 1.0f / TEXTURE_MAX_X * g_aPlayer.nPatternAnim;
	pVtx[2].tex.y = 1.0f / TEXTURE_MAX_Y * (g_aPlayer.nPatternAnim / TEXTURE_MAX_X + 1);

	pVtx[3].tex.x = 1.0f / TEXTURE_MAX_X * (g_aPlayer.nPatternAnim + 1);
	pVtx[3].tex.y = 1.0f / TEXTURE_MAX_Y * (g_aPlayer.nPatternAnim / TEXTURE_MAX_X + 1);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();
}

//=====================================================================
// �`�揈��
//=====================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (g_aPlayer.bVisible == true)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=====================================================================
// �v���C���[�̃q�b�g����
//=====================================================================
void HitPlayer(int nDamage)
{
	if (g_aPlayer.state == PLAYERSTATE_NORMAL)
	{
		g_aPlayer.fLife--;

		if (IsPlayerDead() == true)
		{
			SetExplosion(g_aPlayer.pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			g_aPlayer.state = PLAYERSTATE_DEAD;
			g_aPlayer.nCountState = 0;

			PlaySound(SOUND_LABEL_SE_EXPLOSION);
		}
		else
		{
			g_aPlayer.state = PLAYERSTATE_DAMAGED;
			PlaySound(SOUND_LABEL_SE_DAMAGE);
		}

	}
}

//=====================================================================
// �v���C���[�̈ʒu���擾���鏈��
//=====================================================================
Player* GetPlayer(void)
{
	return &g_aPlayer;
}

//=====================================================================
// �v���C���[���������Ă��邩�m�F���鏈��
//=====================================================================
bool IsPlayerDead(void)
{
	return (g_aPlayer.state == PLAYERSTATE_DEAD || (g_aPlayer.fLife <= 0.0f));
}

//=====================================================================
// �e���ˏ���
//=====================================================================
void ShootBullet(void)
{
	PlaySound(SOUND_LABEL_SE_SHOT, 0.25f);

	switch (g_aPlayer.nUpgrade)
	{
	case 1:
		SetBullet(g_aPlayer.pos, SHOOT_SPEED, g_aPlayer.fRot + D3DX_PI, BULLETTYPE_PLAYER, BULLETPATTERN_PLAYER);
		SetBullet(g_aPlayer.pos + D3DXVECTOR3(15, 10, 0), SHOOT_SPEED, g_aPlayer.fRot + D3DX_PI, BULLETTYPE_PLAYER, BULLETPATTERN_PLAYER);
		SetBullet(g_aPlayer.pos + D3DXVECTOR3(-15, 10, 0), SHOOT_SPEED, g_aPlayer.fRot + D3DX_PI, BULLETTYPE_PLAYER, BULLETPATTERN_PLAYER);
		break;

	case 0:		// 5Way
		SetBullet(g_aPlayer.pos, SHOOT_SPEED, g_aPlayer.fRot + D3DX_PI, BULLETTYPE_PLAYER, BULLETPATTERN_PLAYER);
		SetBullet(g_aPlayer.pos + D3DXVECTOR3(sinf(g_aPlayer.fRot), cosf(g_aPlayer.fRot), 0.0f) * -20.0f, SHOOT_SPEED, g_aPlayer.fRot + D3DX_PI + (D3DX_PI * 0.25), BULLETTYPE_PLAYER, BULLETPATTERN_PLAYER);
		SetBullet(g_aPlayer.pos + D3DXVECTOR3(sinf(g_aPlayer.fRot), cosf(g_aPlayer.fRot), 0.0f) * -20.0f, SHOOT_SPEED, g_aPlayer.fRot + D3DX_PI - (D3DX_PI * 0.25), BULLETTYPE_PLAYER, BULLETPATTERN_PLAYER);
		SetBullet(g_aPlayer.pos + D3DXVECTOR3(sinf(g_aPlayer.fRot), cosf(g_aPlayer.fRot), 0.0f) * -20.0f, SHOOT_SPEED, g_aPlayer.fRot + D3DX_PI + (D3DX_PI * 0.75), BULLETTYPE_PLAYER, BULLETPATTERN_PLAYER);
		SetBullet(g_aPlayer.pos + D3DXVECTOR3(sinf(g_aPlayer.fRot), cosf(g_aPlayer.fRot), 0.0f) * -20.0f, SHOOT_SPEED, g_aPlayer.fRot + D3DX_PI - (D3DX_PI * 0.75), BULLETTYPE_PLAYER, BULLETPATTERN_PLAYER);
		break;
	}
}
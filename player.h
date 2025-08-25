//=====================================================================
// 
// player.cpp�̃w�b�_�t�@�C�� [player.h]
// Author:���O��
//
//=====================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_		// ��d�C���N���[�h�h�~�̃}�N��

#include "main.h"

// �v���C���[�̏�Ԃ̎��
typedef enum
{
	PLAYERSTATE_NORMAL = 0,
	PLAYERSTATE_DAMAGED,
	PLAYERSTATE_FLASH,
	PLAYERSTATE_DEAD,
	PLAYERSTATE_END,
	PLAYERSTATE_MAX
}PLAYERSTATE;

// �v���C���[�\����
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 posMove;	// �ړ���
	float fRot;				// ��]			
	float fRotMove;			// ��]��
	D3DXVECTOR3 size;		// �T�C�Y

	int nCountAnim;			// �A�j���[�V�����J�E���g
	int nPatternAnim;		// �A�j���[�V�����p�^�[��

	PLAYERSTATE state;		// ���
	int nCountState;		// ��ԃJ�E���g
	float fLife;				// �̗�
	int nLastShot;			// �Ō�Ɍ����Ă���̌o�ߎ���
	int nUpgrade;			// �A�b�v�O���[�h
	int nCharge;			// �`���[�W

	bool bVisible;			// �`�悷�邩	

}Player;

// �v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void HitPlayer(int nDamage);
Player* GetPlayer(void);
bool IsPlayerDead(void);

#endif
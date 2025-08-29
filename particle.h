//=====================================================================
// 
// particle.cpp�̃w�b�_�t�@�C�� [particle.h]
// Author:���O��
//
//=====================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_		// ��d�C���N���[�h�h�~�̃}�N��

#include "main.h"

//*******************************************
// �}�N����`
//*******************************************
#define MAX_PARTICLE		(128)	// �p�[�e�B�N���̍ő吔

//*******************************************
// �G�t�F�N�g���\����
//*******************************************
typedef struct
{
	char cTextureFilePath[MAX_PATH];

	D3DXVECTOR3 pos;		// �ʒu
	float fRot;				// ��]
	float fScale;			// �T�C�Y�{��

	D3DXCOLOR col;			// �F
	float fRangeRad;		// ���ˊp�x�͈�
	float fSpeed;			// ���x
	float fLifeTime;		// ��������
	int nPartcileLife;
	bool bUseTexture;
	bool bUsed;				// �g�p���Ă��邩�ǂ���
}Particle;

//*******************************************
// �v���g�^�C�v�錾
//*******************************************
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRangeRad, float fSpeed, float fScale, float fLifeTime);
Particle* GetParticle(void);
bool SaveParticle(const char* cFilePath);
bool LoadParticle(const char* cFilePath);
bool IsParticleUnsaved(void);

#endif
//=====================================================================
// 
// �p�[�e�B�N������ [particle.cpp]
// Author:���O��
//
//=====================================================================
#include "input.h"

#include "particle.h"
#include "effect.h"

//*******************************************
// �}�N����`
//*******************************************
#define DEF_TEXTURE_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)		// �e�N�X�`���̐F

#define DEF_PARTICLE_LIFETIME			(100.0f)								// �G�t�F�N�g���c�鎞��
#define DEF_PARTICLE_SIZE				(30.0f)									// �G�t�F�N�g�̃T�C�Y�{��

//*******************************************
// �O���[�o���ϐ��錾
//*******************************************
Particle g_aParticle[MAX_PARTICLE];

//=====================================================================
// ����������
//=====================================================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	Particle* pParticle = &g_aParticle[0];

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �G�t�F�N�g�̏��̏�����
	memset(pParticle, 0, sizeof(Particle) * MAX_PARTICLE);
	for (int nCount = 0; nCount < MAX_PARTICLE; nCount++, pParticle++)
	{
		pParticle->pos = D3DXVECTOR3_ZERO;
		pParticle->fRot = 0.0f;
		pParticle->fScale = 1.0f;

		pParticle->col = DEF_TEXTURE_COLOR;
		pParticle->fRangeRad = D3DX_PI * 2;
		pParticle->fSpeed = 1.0f;
		pParticle->fLifeTime = DEF_PARTICLE_LIFETIME;
		pParticle->bUseTexture = true;
	}
}

//=====================================================================
// �I������
//=====================================================================
void UninitParticle(void)
{

}

//=====================================================================
// �X�V����
//=====================================================================
void UpdateParticle(void)
{
	Particle* pParticle = &g_aParticle[0];
	for (int nCount = 0; nCount < MAX_PARTICLE; nCount++, pParticle++)
	{
		if (pParticle->bUsed == true)
		{
			pParticle->nPartcileLife -= 1;

			SetEffect(pParticle->pos, pParticle->col, (float)((rand() % 629) - 314) / 100.0f, pParticle->fSpeed, pParticle->fScale,  pParticle->fLifeTime, false);
			
			if (pParticle->nPartcileLife < 1)
			{
				pParticle->bUsed = false;
			}
		}
	}
}

//=====================================================================
// �p�[�e�B�N���̐ݒ菈��
//=====================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRangeRad, float fSpeed, float fScale, float fLifeTime)
{
	Particle* pParticle = &g_aParticle[0];

	for (int nCount = 0; nCount < MAX_PARTICLE; nCount++, pParticle++)
	{
		if (pParticle->bUsed == false)
		{
			pParticle->pos = pos;
			pParticle->fRot = (float)(rand() / (314 * 2)) * 0.01f;
			pParticle->fScale = fScale;

			pParticle->col = col;
			pParticle->fRangeRad = 1.0f;
			pParticle->fSpeed = 8.0f;
			pParticle->fLifeTime = 15.0f;
			pParticle->nPartcileLife = 20;
			pParticle->bUseTexture = true;
			pParticle->bUsed = true;
			break;
		}
	}
}

//=====================================================================
// �p�[�e�B�N���擾����
//=====================================================================
Particle* GetParticle(void)
{
	return &g_aParticle[0];
}

////=====================================================================
//// �G�t�F�N�g�ۑ�����
////=====================================================================
//bool SaveParticle(const char* cFilePath)
//{
//	Particle* pParticle = &g_aParticle;
//	FILE* pFile;
//
//	pFile = fopen(cFilePath, "wb");
//
//	if (pFile != NULL)
//	{
//		fwrite(pParticle, sizeof(Particle), 1, pFile);
//
//		fclose(pFile);
//	}
//	else
//	{
//		return false;
//	}
//
//	g_bUnsaved = false;
//
//	return true;
//}
//
////=====================================================================
//// �G�t�F�N�g�Ǎ�����
////=====================================================================
//bool LoadParticle(const char* cFilePath)
//{
//	Particle* pParticle = &g_aParticle;
//	FILE* pFile;
//
//	pFile = fopen(cFilePath, "rb");
//
//	if (pFile != NULL)
//	{
//		fread(pParticle, sizeof(Particle), 1, pFile);
//
//		pParticle->pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f };
//		pParticle->fRot= 0.0f;
//		pParticle->bUseTexture = true;
//
//		fclose(pFile);
//	}
//	else
//	{
//		return false;
//	}
//
//	g_bUnsaved = false;
//
//	ClearAllEffect();
//
//	return true;
//}
//
//bool IsParticleUnsaved(void)
//{
//	return g_bUnsaved;
//}
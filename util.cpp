//=====================================================================
// 
// �ėp�֐� [util.cpp]
// Author : KAITO IWASAKI
//
//=====================================================================
#include "util.h"

//=====================================================================
// ���l��͈͓��Ɏ��߂鏈��
//=====================================================================
int Clamp(int nNum, int nMin, int nMax)
{
	if (nNum < nMin)
	{
		return nMin;
	}
	else if (nNum > nMax)
	{
		return nMax;
	}

	return nNum;
}

//=====================================================================
// ��̃x�N�g���̋��������߂鏈��
//=====================================================================
float Magnitude(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	D3DXVECTOR3 fDistance = to - from;

	return sqrtf(fDistance.x * fDistance.x + fDistance.y * fDistance.y);
}

//=====================================================================
// ����x�N�g������ʂ̃x�N�g���܂ł̊p�x�����߂鏈��
//=====================================================================
float Direction(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	return atan2f(to.x - from.x, to.y - from.y);
}

int RandRange(int nMin, int nMax)
{
	return (rand() % (nMax - nMin)) + nMin;
}

//=====================================================================
// �o�C�i���t�@�C���ǂݍ��ݏ���
//=====================================================================
bool LoadBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount)
{
	FILE* pFile;
	HWND hWnd = GetMainWindow();
	char aMessage[512];

	pFile = fopen(pFilePath, "rb");

	if (pFile != NULL)
	{
		fread(pBuffer, ElementSize, ElementCount, pFile);

		fclose(pFile);

		return true;
	}
	else
	{
		wsprintf(aMessage, "�t�@�C����ǂݍ��݂Ɏ��s���܂����F\n%s", pFilePath);

		MessageBox(hWnd, aMessage, "�G���[", MB_ICONERROR);

		return false;
	}
}

//=====================================================================
// �o�C�i���t�@�C�������o������
//=====================================================================
bool SaveBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount)
{
	FILE* pFile;
	HWND hWnd = GetMainWindow();
	char aMessage[512];

	pFile = fopen(pFilePath, "wb");

	if (pFile != NULL)
	{
		fwrite(pBuffer, ElementSize, ElementCount, pFile);

		fclose(pFile);

		return true;
	}
	else
	{
		wsprintf(aMessage, "�t�@�C���̏����o���Ɏ��s���܂����F\n%s", pFilePath);

		MessageBox(hWnd, aMessage, "�G���[", MB_ICONERROR);

		return false;
	}
}

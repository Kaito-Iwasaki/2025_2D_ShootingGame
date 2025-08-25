//=====================================================================
// 
// 汎用関数 [util.cpp]
// Author : KAITO IWASAKI
//
//=====================================================================
#include "util.h"

//=====================================================================
// 数値を範囲内に収める処理
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
// 二つのベクトルの距離を求める処理
//=====================================================================
float Magnitude(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	D3DXVECTOR3 fDistance = to - from;

	return sqrtf(fDistance.x * fDistance.x + fDistance.y * fDistance.y);
}

//=====================================================================
// あるベクトルから別のベクトルまでの角度を求める処理
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
// バイナリファイル読み込み処理
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
		wsprintf(aMessage, "ファイルを読み込みに失敗しました：\n%s", pFilePath);

		MessageBox(hWnd, aMessage, "エラー", MB_ICONERROR);

		return false;
	}
}

//=====================================================================
// バイナリファイル書き出し処理
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
		wsprintf(aMessage, "ファイルの書き出しに失敗しました：\n%s", pFilePath);

		MessageBox(hWnd, aMessage, "エラー", MB_ICONERROR);

		return false;
	}
}

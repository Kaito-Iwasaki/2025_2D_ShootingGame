//=====================================================================
// 
// 汎用関数 [util.h]
// Author:岩崎桧翔
//
//=====================================================================
#ifndef _UTIL_H_
#define _UTIL_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
int Clamp(int nNum, int nMin, int nMax);
float Magnitude(D3DXVECTOR3 from, D3DXVECTOR3 to);
float Direction(D3DXVECTOR3 from, D3DXVECTOR3 to);
int RandRange(int nMin, int nMax);
bool LoadBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount);
bool SaveBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount);
//bool SetPrimitive(LPDIRECT3DVERTEXBUFFER9 pVtxBuffer, D3DXVECTOR3 pos, D3DXVECTOR3 size);

#endif
#include "collision.h"

bool Collision(D3DXVECTOR3 posA, D3DXVECTOR3 sizeA, D3DXVECTOR3 posB, D3DXVECTOR3 sizeB)
{
	if (
		posA.x >= (posB.x - sizeB.x / 2)
		&& posA.x <= (posB.x + sizeB.x / 2)
		&& posA.y >= (posB.y - sizeB.y / 2)
		&& posA.y <= (posB.y + sizeB.y / 2)
		)
	{
		return true;
	}

	return false;
}

#include "IncludePCH.h"
#include "DualGun.h"
#include "Utilities.h"
using namespace DirectX;

DualGun::DualGun(float sideOffset, float switchrate)
{
	originLeft = DirectX::XMVectorSet(-sideOffset, 0.0f, 0.15f, 1.0f);
	originRight = DirectX::XMVectorSet(sideOffset, 0.0f, 0.15f, 1.0f);
}


DualGun::~DualGun()
{
}

void DualGun::Update(float dt , DirectX::XMMATRIX matrix,bool isFiring)
{
	
	matrix = XMMatrixInverse(&det, matrix);
	Guns[left].Update(XMVector3TransformCoord(originLeft, matrix), dt);
	Guns[right].Update(XMVector3TransformCoord(originRight, matrix), dt);
	
	if ((timer += dt) >= switchRate)
	{
		if (isFiring)
		{
			switchGuns = !switchGuns;
			timer = 0.0f;
		}
	};
}

DirectX::XMVECTOR DualGun::Fire(const DirectX::XMVECTOR & targetPos, DirectX::XMVECTOR& outGunPos)
{
	if (switchGuns)
	{
		if (Guns[left].Loaded())
		{
			outGunPos = XMLoadFloat3(&Guns[left].GetPosition());
			return Guns[left].Fire(targetPos);
		}
	}
	else
	{
		if (Guns[right].Loaded())
		{
			outGunPos = XMLoadFloat3(&Guns[right].GetPosition());
			return Guns[right].Fire(targetPos);
		}
	}
	return XMVectorSet(0.0f,0.0f,0.0f,1.0f);
}

DirectX::XMFLOAT3 DualGun::GetGunPosition(int gunIndex)
{
	return Guns[gunIndex].GetPosition();
}





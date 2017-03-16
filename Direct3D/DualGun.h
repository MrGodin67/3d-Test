#pragma once
#include "GunBase.h"
class DualGun
{
	DirectX::XMVECTOR originLeft;
	DirectX::XMVECTOR originRight;
	DirectX::XMVECTOR det = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	enum Guns
	{
		left,
		right,
		numGuns
	};
	GunBase Guns[numGuns];
	float timer;
	float switchRate = 0.5f;
	bool switchGuns = false;
public:
	
public:
	DualGun(float sideOffset,float switchrate);
	virtual ~DualGun();
	virtual void Update(float dt,DirectX::XMMATRIX matrix,bool isFiring = false);
	virtual DirectX::XMVECTOR Fire(const DirectX::XMVECTOR& targetPos, DirectX::XMVECTOR& outGunPos);
	virtual DirectX::XMFLOAT3 GetGunPosition(int gunIndex);
};


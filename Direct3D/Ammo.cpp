
#include "IncludePCH.h"
#include "Ammo.h"

using namespace DirectX;
Ammo::Ammo()
{
}

Ammo::Ammo(DirectX::XMVECTOR pos, DirectX::XMVECTOR vel,float speed)
	:speed(speed)
{
	XMStoreFloat3(&position, pos);
	XMStoreFloat3(&velocity, vel);
}

bool Ammo::Update(float dt)
{
	timer += dt;
	XMVECTOR p = XMLoadFloat3(&position) + XMLoadFloat3(&velocity) * speed * dt;
	XMStoreFloat3(&position, p);
	return (timer < timeToDie);
}

DirectX::XMFLOAT3 Ammo::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Ammo::GetVeclocity()
{
	return velocity;
}

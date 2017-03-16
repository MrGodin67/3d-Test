#pragma once

class Ammo
{
public:
	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 velocity = { 0.0f,0.0f,0.0f };
	float speed;
	float timeToDie = 2.0f;
	float timer = 0.0f;
public:
	Ammo();
	Ammo(DirectX::XMVECTOR pos, DirectX::XMVECTOR vel,float speed);
	bool Update(float dt);
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetVeclocity();

};
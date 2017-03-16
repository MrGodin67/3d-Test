#include "IncludePCH.h"
#include "Terrain.h"
#include "Player.h"

Player::Player(CFirstPersonCamera& cam)
	:
	m_cam(cam),
	m_gun(0.25f,0.5f)
{
}

void Player::Update(float dt,Terrain* terrain)
{
	m_gun.Update(dt, m_cam.GetViewMatrix(),isFiring);
	/*DirectX::XMFLOAT3 cPos = m_cam.GetEyePt2();
	float height;
	if (terrain->GetHeight(m_cam.GetEyePt(), cPos.y))
	{
		cPos.y += m_heightOffTerrain;
		m_cam.SetEye(cPos.x, cPos.y, cPos.z);
	};*/
}

void Player::IsFiring(bool val)
{
	isFiring = val;
}

bool Player::FireGun(const DirectX::XMVECTOR & targetPos, Ammo& outAmmo)
{
	XMVECTOR p;
	XMVECTOR v = m_gun.Fire(targetPos, p);
	if (XMVector3Equal(v, { 0.0f,0.0f,0.0f,1.0f }))
	{
		return false;
	}
	outAmmo = Ammo(p, v, m_ammoVelocity);
	return true;
}

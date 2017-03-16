#include "IncludePCH.h"
#include "GunBase.h"
using namespace DirectX;

GunBase::GunBase()
{
}


GunBase::~GunBase()
{
}

bool GunBase::Loaded()
{
	return m_loaded;
}



void GunBase::Update(DirectX::XMVECTOR &position,float dt)
{
	
	if ((m_reloadTimer += dt) >= m_rateOfFire)
	{
		m_loaded = true;

	}
	XMStoreFloat3(&pos,position);
}

void GunBase::SetRateOfFire(const float & val)
{
	m_rateOfFire = val;
}

DirectX::XMVECTOR GunBase::Fire(const DirectX::XMVECTOR& targetPos)
{
	if (m_loaded)
	{
		m_loaded = false;
		m_reloadTimer = 0.0f;
		DirectX::XMVECTOR velocity = targetPos - XMLoadFloat3(&pos);
		return XMVector3Normalize(velocity);

	}
	return XMVectorSet(0.0f,0.0f,0.0f,1.0f);
}

#pragma once
#include "Events.h"
class GunBase
{
public:
	
protected:
	DirectX::XMFLOAT3 pos;
	float m_rateOfFire = 0.25f;
	float m_reloadTimer = 0.0f;
	bool m_loaded = true;
	enum GunType
	{
		light,
		medium,
		heavy
	};
public:
	GunBase();
	virtual ~GunBase();
	virtual bool Loaded();
    virtual void Update(DirectX::XMVECTOR &position,float dt);
	virtual void SetRateOfFire(const float& val);
	DirectX::XMFLOAT3 GetPosition() { return pos; }
	virtual DirectX::XMVECTOR Fire(const DirectX::XMVECTOR& targetPos);
};


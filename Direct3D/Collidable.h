#pragma once
#include "IncludePCH.h"
#include "Utilities.h"
class Collidable
{
public:
	Collidable() {};
	virtual Utilities::BoundingBox GetAABB() = 0;
	virtual DirectX::XMVECTOR GetPosition() = 0;
	virtual DirectX::XMVECTOR GetVelocity() = 0;

};
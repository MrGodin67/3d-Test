#pragma once
#include "Object3D.h"
#include "WorldGrid.h"
#include"Camera3D_2.h"
#include "DualGun.h"
#include "Ammo.h"
class Player 
{
	DualGun m_gun;
	CFirstPersonCamera& m_cam;
	float m_heightOffTerrain = 0.25f;
	float m_ammoVelocity = 18.0f;
	bool isFiring = false;
public:
	Player(CFirstPersonCamera& cam);
	WorldGridCell* pCurrentCell;
	void Update(float dt,class Terrain* terrain);
	void IsFiring(bool val);
	bool IsFiring() { return isFiring; }
	bool FireGun(const DirectX::XMVECTOR & targetPos,Ammo& outAmmo);
};
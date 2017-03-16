#include "IncludePCH.h"
#include "Object3D.h"
#include "WorldGrid.h"
#include "Graphics.h"
using namespace DirectX;

Object3D::Object3D()
{
	pCurrentCell = nullptr;
}


Object3D::~Object3D()
{
}

void Object3D::SetModel(WavefrontModel * model)
{
	m_pModel = model;
}

DirectX::XMVECTOR Object3D::GetPosition()
{
	return XMLoadFloat3(&m_position);
}

void Object3D::SetPosition(DirectX::XMVECTOR pos)
{
	XMStoreFloat3(&m_position, pos);
	
}
/////////////////////////////////////////////////
// Drawable
/////////////////////////////////////////////////
HRESULT Object3D::Drawable::Draw(Graphics & gfx)
{
	return S_OK;
}

//////////////////////////////////////////////////
// Collision
//////////////////////////////////////////////////

Utilities::BoundingBox Object3D::Collidable::GetAABB()
{
	XMFLOAT3 minPt = parent.m_pModel->BoundingBox().minPt;
	XMFLOAT3 maxPt = parent.m_pModel->BoundingBox().maxPt;
	minPt.x = parent.m_position.x + minPt.x;
	minPt.y = parent.m_position.y + minPt.y;
	minPt.z = parent.m_position.z + minPt.z;
	maxPt.x = parent.m_position.x + maxPt.x;
	maxPt.y = parent.m_position.y + maxPt.y;
	maxPt.z = parent.m_position.z + maxPt.z;

	return Utilities::BoundingBox(parent.m_position,minPt,maxPt);
}

DirectX::XMVECTOR Object3D::Collidable::GetPosition()
{
	return XMLoadFloat3(&parent.m_position);
}

DirectX::XMVECTOR Object3D::Collidable::GetVelocity()
{
	return XMLoadFloat3(&parent.m_velocity);
}

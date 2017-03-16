#pragma once
#include "Collidable.h"
#include "drawable.h"
#include "Model.h"
class Object3D
{
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_velocity;
	WavefrontModel* m_pModel;
public:
	class Drawable : public ::Drawable
	{
		Object3D& parent;
	public:
		Drawable(Object3D& owner)
			:parent(owner)
		{}
		virtual HRESULT Draw(class Graphics& gfx)override;
	};
	class Collidable : public ::Collidable
	{
		Object3D& parent;
	public:
		Collidable(Object3D& owner)
			:parent(owner)
		{}
		virtual Utilities::BoundingBox GetAABB()override;
		virtual DirectX::XMVECTOR GetPosition()override;
		virtual DirectX::XMVECTOR GetVelocity()override;
	};
public:
	Object3D();
	virtual ~Object3D();
	void SetModel(WavefrontModel* model);
	DirectX::XMVECTOR GetPosition();
	void SetPosition(DirectX::XMVECTOR pos);
	struct  WorldGridCell* pCurrentCell = nullptr;
	Collidable Collision() { return Collidable(*this); }
	Drawable Renderable() { return Drawable(*this); }
};


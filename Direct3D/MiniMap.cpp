#include "IncludePCH.h"
#include "MiniMap.h"
#include "Object3D.h"
#include "Graphics.h"
#include "Utilities.h"
using namespace DirectX;
MiniMap::MiniMap()
{
}


MiniMap::MiniMap(D2D1_RECT_F frame,const XMFLOAT2& WorldAxisOffset, const float scale )
	:frame(frame),worldAxis(WorldAxisOffset),scale(scale)
{
	rRect.radiusX = 12.0f;
	rRect.radiusY = 12.0f;
	rRect.rect = frame;
}

MiniMap::~MiniMap()
{
}



void MiniMap::AddEntity(const MiniMapEntityType entityType, const DirectX::XMVECTOR& pos)
{
	DirectX::XMVECTOR vec = playerData.pos - pos;
	
	float lenSq = XMVectorGetX(XMVector3LengthSq(vec));
	if (lenSq < sqr<float>(playerData.range))// * playerData.range)
	{
		XMFLOAT2 newPos;
		newPos.y = frame.top + ((worldAxis.y + XMVectorGetZ(pos)) * scale);
		newPos.x = frame.left + ((worldAxis.x + XMVectorGetX(pos)) * scale);
		m_positions[entityType].push_back(newPos);
	}
}

void MiniMap::Update(const PlayerData& in_playerData)
{
	m_positions.clear();
	XMFLOAT2 newPos;
	playerData = in_playerData;
	
	newPos.y = frame.top + ((worldAxis.y + XMVectorGetZ(playerData.pos)) *scale);
	newPos.x = frame.left + (worldAxis.x + (XMVectorGetX(playerData.pos) )* scale);
	m_positions[Player].push_back(newPos);
	for (int c = 0; c < 3; c++)
	{
		TrianglePts[c].x = newPos.x + arrayTriangle[0][c];
		TrianglePts[c].y = newPos.y + arrayTriangle[1][c];
	}
	
	newPos.y = ( newPos.y) + (XMVectorGetZ(playerData.vel)) * playerData.range;
	newPos.x = ( newPos.x) + XMVectorGetX(playerData.vel) * playerData.range;

	if (newPos.x < frame.left + 10.0f)
		newPos.x = frame.left + 10.0f;
	if (newPos.x > frame.right - 10.0f)
		newPos.x = frame.right - 10.0f;

	if (newPos.y < frame.top + 10.0f)
		newPos.y = frame.top + 10.0f;
	if (newPos.y > frame.bottom - 10.0f)
		newPos.y = frame.bottom - 10.0f;

	m_positions[RangeRetical].push_back(newPos);
	playerData.rotAngle = Utilities::GetAngleBetweenPoints2D(
		XMFLOAT2(m_positions[Player][0].x, m_positions[Player][0].y),
		XMFLOAT2(m_positions[RangeRetical][0].x, m_positions[RangeRetical][0].y)
	);
}

void MiniMap::Render(Graphics & gfx)
{
	gfx.D2D_DrawFilledRoundedRectangle(nullptr, rRect, { 0.0f,0.0f,0.0f,0.55f });
		
	for (auto& it : m_positions)
	{
		switch (it.first)
		{
		case RangeRetical:
		{
			gfx.D2D_DrawEllipse({ it.second[0].x,it.second[0].y }, { 2.0f,2.0f }, { 0.0f,1.0f,1.0f,1.0f });
		}
		break;
		case Player:
		{
			gfx.D2D1_DrawTriangle(
				&D2D1::Matrix3x2F::Rotation(playerData.rotAngle, { it.second[0].x ,it.second[0].y}),
				TrianglePts, { 0.0f,1.0f,0.0f,1.0f });
		}
		break;
		case Enemy:
		{
			D2D1_RECT_F rect;
			for (int c = 0; c < it.second.size(); c++)
			{
				float size = 2.0f;
				std::function< D2D1_RECT_F(float, float) >getRect = [&size](float x, float y)->D2D1_RECT_F
				{
					return D2D1::RectF(x - size, y - size, x + size, y + size);
				};
				rect.left = it.second[c].x - 2.0f;
				rect.top = it.second[c].y - 2.0;
				rect.bottom = it.second[c].y + 2.0f;
				rect.right = it.second[c].x + 2.0f;
				//getRect(it.second[c].x, it.second[c].y)
				gfx.D2D_DrawRectangle(nullptr, rect, { 1.0f,1.0f,1.0f,1.0f });
			}
			
			
		};
		break;
		}
	}
}

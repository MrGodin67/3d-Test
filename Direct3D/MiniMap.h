#pragma once

#include "WorldGrid.h"
static const int MMEntType_Player = 0;
static const int MM_Enemy = 1;
static const int MM_Goal = 2;
static const int MM_Misc = 3;
class MiniMap
{
	
public:
	enum MiniMapEntityType
	{
		Player,
		Enemy,
		Goal,
		Misc,
		RangeRetical,
		Invalid

	};
	struct alignas(16) PlayerData
	{
		DirectX::XMVECTOR pos;
		DirectX::XMVECTOR vel;
		float range = 0.0f;
		float rotAngle = 0.0f;
		PlayerData() {}
		PlayerData(DirectX::XMVECTOR pos, DirectX::XMVECTOR vel, float range)
			:pos(pos), vel(vel), range(range)
		{}

	};
public:
	
	MiniMap();
	MiniMap(D2D1_RECT_F frame,const DirectX::XMFLOAT2 &WorlsAxisOffset,const float scale = 1.0f);
	~MiniMap();
	void AddEntity(const MiniMapEntityType entityType, const DirectX::XMVECTOR& pos);
	void Update(const PlayerData& in_playerData);
	void Render(class Graphics& gfx);
private:
	const float arrayTriangle[2][3] = {
		{ 0.0f,6.0f,-6.0f },
		{ -6.0f,6.0f,6.0f }
	};
	D2D1_POINT_2F TrianglePts[3];
	D2D1_RECT_F frame;
	DirectX::XMFLOAT2 worldAxis;
	D2D1_ROUNDED_RECT rRect;
	float scale;
	PlayerData playerData;
	std::unordered_map<MiniMapEntityType, std::vector<DirectX::XMFLOAT2>> m_positions;

};


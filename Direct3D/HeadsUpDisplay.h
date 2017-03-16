#pragma once
#include "FontFactory.h"
#include "MiniMap.h"
#include"Events.h"
#include "Table.h"
#include "InfoPanel.h"
class Graphics;
class HUD_Object
{
public:
	HUD_Object() {}
	virtual ~HUD_Object() {}
	virtual void Draw(Graphics& gfx) = 0;
	
protected:
	D2D1_COLOR_F color;
	
};
class HeadsUpDisplay
{
	
	std::vector<HUD_Object> m_items;
	std::unordered_map<std::string,FontFactory::FontDesc> m_text;
	std::unique_ptr<MiniMap> minimap;
	std::unique_ptr<InfoPanel> infopanel;
	DirectX::XMFLOAT3 m_camPos;
	float vpWidth;
	float vpHeight;
	D2D1_RECT_F vpDimensions;
	DirectX::XMFLOAT2 vpCenter;
	
	
public:
	HeadsUpDisplay(D3D11_VIEWPORT& viewPort );
	HeadsUpDisplay(DirectX::XMFLOAT3& camPos);
	~HeadsUpDisplay();

	
	void UpdateObjectPosition(MiniMap::MiniMapEntityType type, DirectX::XMVECTOR& pos);
	void AddText(std::string name,FontFactory::FontDesc desc);
	void Update(const UpdateEventArgs& args, const MiniMap::PlayerData& player);
	void UpdateMouse(const int x, const int y);
	FontFactory::FontDesc& GetText(std::string name);
	void Render(Graphics& gfx);
};


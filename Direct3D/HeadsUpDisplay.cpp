#include "IncludePCH.h"
#include "HeadsUpDisplay.h"
#include "Graphics.h"
#include "StringHandler.h"
using namespace DirectX;
HeadsUpDisplay::HeadsUpDisplay(D3D11_VIEWPORT& viewPort)
{
	
	vpWidth = viewPort.Width;
	vpHeight = viewPort.Height;
	vpDimensions.left = viewPort.TopLeftX;
	vpDimensions.top = viewPort.TopLeftY;
	vpDimensions.right = viewPort.TopLeftX + vpWidth;
	vpDimensions.bottom = viewPort.TopLeftY + vpHeight;
	vpCenter.x = (vpDimensions.right - vpDimensions.left) * 0.5f;
	vpCenter.y = (vpDimensions.bottom - vpDimensions.top) * 0.5f;
	D2D1_RECT_F infoFrame;
	infoFrame.left = 10.0f;
	infoFrame.bottom = vpDimensions.bottom - 10.0f;
	infoFrame.top = vpDimensions.bottom - (10.0f + 96.0f);
	infoFrame.right = vpDimensions.right - (10.0f + 202.0f );
	D2D1_RECT_F rect;
	rect.left = vpDimensions.right - 202.0f;
	rect.right = vpDimensions.right - 10.0f;
	rect.top = vpDimensions.bottom - 202.0f;
	rect.bottom = vpDimensions.bottom - 10.0f;
	minimap = std::make_unique<MiniMap>(rect, DirectX::XMFLOAT2( 0.0f,0.0f ), 3.2f);
	infopanel = std::make_unique<InfoPanel>(infoFrame);
	
	
}

HeadsUpDisplay::HeadsUpDisplay(DirectX::XMFLOAT3& camPos)
{
}
HeadsUpDisplay::~HeadsUpDisplay()
{
}


void HeadsUpDisplay::UpdateObjectPosition(MiniMap::MiniMapEntityType type, DirectX::XMVECTOR & pos)
{
	minimap->AddEntity(type, pos);
}

void HeadsUpDisplay::AddText(std::string name, FontFactory::FontDesc desc)
{
	m_text[name] = desc;
}

void HeadsUpDisplay::Update(const UpdateEventArgs& args, const MiniMap::PlayerData& player)
{
	minimap->Update(player);
	std::wstring x, y, z;
	DXStringHandler::DXFloatPercisionWString(x, XMVectorGetX(player.pos), 2);
	DXStringHandler::DXFloatPercisionWString(y, XMVectorGetY(player.pos), 2);
	DXStringHandler::DXFloatPercisionWString(z, XMVectorGetZ(player.pos), 2);
	m_text["ordinates"].text = L"Ordinates X[" + x + L"] Y[" +
		y + L"] Z[" + z + L"]";
	infopanel->Update(args);

}

void HeadsUpDisplay::UpdateMouse(const int x, const int y)
{
	m_text["mouse"].text = L"Mouse X[" + std::to_wstring(x) + L"] Y[" +
		std::to_wstring(y) + L"]";

}

FontFactory::FontDesc & HeadsUpDisplay::GetText(std::string name)
{
	return m_text[name];
}

void HeadsUpDisplay::Render(Graphics & gfx)
{
	infopanel->Draw(gfx);
	for (auto& text : m_text)
		gfx.D2D_RenderText(text.second);

	minimap->Render(gfx);
}

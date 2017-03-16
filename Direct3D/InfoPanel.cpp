#include "IncludePCH.h"
#include "InfoPanel.h"


InfoPanel::InfoPanel()
{}

InfoPanel::InfoPanel(const D2D1_RECT_F& frame)
	:frame(frame)
{
	int height = (int)(frame.bottom - frame.top);
	int width = (int)(frame.right - frame.left);
	mGrid.resize((int)(height / 18.0f), (int)(width / 120.0f));
	float startX;
	float startY = frame.top;
	for (int r = 0; r < (int)(height / 18.0f); r++)
	{
		startX = frame.left + 2.0f;
		for (int c = 0; c < (int)(width / 120.0f); c++)
		{
			mGrid(r, c) = D2D1::Point2F(startX, startY);
			startX += 120.0f;

		}
		startY += 18.0f;
	}
	D2D1_RECT_F f = { mGrid(0,1).x,mGrid(0,1).y+4.0f,mGrid(0,1).x + 100.0f,mGrid(0,1).y + 16.0f };
	
	D2D1_COLOR_F c = { 1.0f,1.0f,1.0f,1.0f };
	m_meters["shield"] = std::make_unique<Meter>(f,100.0f, c, false);
	f = { mGrid(1,1).x,mGrid(1,1).y+4.0f,mGrid(1,1).x + 100.0f,mGrid(1,1).y + 16.0f };
	m_meters["power"] = std::make_unique<Meter>(f, 100.0f, c, false);

	
	rRect.radiusX = 12.0f;
	rRect.radiusY = 12.0f;
	rRect.rect = frame;
}


InfoPanel::~InfoPanel()
{}

void InfoPanel::Draw(Graphics & gfx)
{
	
	gfx.D2D_DrawFilledRoundedRectangle(nullptr, rRect, { 0.0f,0.0f,0.0f,0.55f });

	for (auto& it : m_meters)
		it.second->Draw(gfx);
}

void InfoPanel::Update(const UpdateEventArgs & e)
{
	static float p = 0.0f;
	p += e.ElapsedTime * 2.0f;
	m_meters["shield"]->Update(p);
}

int InfoPanel::OnMouseMove(MouseMotionEventArgs & e)
{
	return 0;
}

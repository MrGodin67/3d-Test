#include "IncludePCH.h"
#include "IntroMenu.h"
#include "Graphics.h"



IntroMenu::IntroMenu()
{
	frame = { viewportCenter.x - 256.0f,
	viewportCenter.y - 256.0f,
	viewportCenter.x + 256.0f,
	viewportCenter.y + 256.0f };

	b1.left = frame.left + (1.0f * 128.0f);
	b1.top = frame.top + (4.0f * 128.0f);
	b1.right = button1.mframe.left + 128.0f;
	b1.bottom = frame.bottom - 128.0f;
	button1.imageIndex = 0;
	button1.returnVal = 1;

	button2.mframe = { frame.left + (3.0f * 128.0f), frame.top + (3.0f * 128.0f), frame.left + (4.0f * 128.0f), frame.top + (4.0f * 128.0f) };
	button2.imageIndex = 1;
	button2.returnVal = -1;
}


IntroMenu::~IntroMenu()
{
}

HRESULT IntroMenu::Render(Graphics & gfx, D2DImage * imageData)
{
	D2D1_ROUNDED_RECT r;
	r.rect = frame;
	r.radiusX = 12.0f;
	r.radiusY = 12.0f;
	gfx.D2D_DrawFilledRoundedRectangle(nullptr, r, fillColor);
	gfx.D2D_DrawRectangle(nullptr, button1.mframe, { 1.0f,1.0f,1.0f,1.0f });
	gfx.DrawSprite(nullptr, button1.mframe, imageData->image, &imageData->GetClipRect(button1.imageIndex));
	
	gfx.DrawSprite(nullptr, button2.mframe, imageData->image, &imageData->GetClipRect(button2.imageIndex));
	return S_OK;
}

UserEventArgs IntroMenu::OnMouseClick( MouseButtonEventArgs & mouse)
{
	
	if (button1.pointInFrame((float)mouse.X, (float)mouse.Y))
	{
		MenuReturnType rt = rt_play;
		return UserEventArgs(button1.returnVal, &rt, nullptr);
	}
	if (button2.pointInFrame((float)mouse.X, (float)mouse.Y))
	{
		MenuReturnType rt = rt_exit;
		return UserEventArgs(button2.returnVal, &rt, nullptr);
	}
	
	return UserEventArgs(0, nullptr, nullptr);
}

UserEventArgs IntroMenu::OnMouseMove(const MouseMotionEventArgs & mouse)
{
	return UserEventArgs(0,0,0);
}

void IntroMenu::OnUpdate(const UpdateEventArgs & args)
{
}

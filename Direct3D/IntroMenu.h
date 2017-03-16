#pragma once
#include "MenuBase.h"
#include "TextureFactory.h"
class IntroMenu :
	public MenuBase
{
	MenuBase::button button1;
	MenuBase::button button2;
	D2DImage* image;
	D2D1_RECT_F b1;
public:
	IntroMenu();
	~IntroMenu();

	// Inherited via MenuBase
	virtual HRESULT Render(Graphics & gfx, D2DImage * imageData) override;
	virtual UserEventArgs OnMouseClick( MouseButtonEventArgs & mouse) override;
	virtual UserEventArgs OnMouseMove(const MouseMotionEventArgs& mouse)override;
	virtual void OnUpdate(const UpdateEventArgs & args) override;
};


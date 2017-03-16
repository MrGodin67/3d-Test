#pragma once
#include "Events.h"
#include "TextureFactory.h"

class MenuBase
{
protected:
	static D2D1_COLOR_F frameColor;
	static D2D1_COLOR_F fillColor;
	static D2D1_POINT_2F viewportCenter;
	D2D1_RECT_F frame;
public:
	struct button
	{
		D2D1_RECT_F mframe;
		D2DImage    image;
		UINT        imageIndex;
		int         returnVal;
		button() {}
		bool pointInFrame(float x, float y)
		{
			return (x > mframe.left && x < mframe.right &&
				y > mframe.top && y < mframe.bottom);
		}
	};
public:
	enum MenuReturnType
	{
		rt_intro,
		rt_map,
		rt_tech,
		rt_play,
		rt_exit,
		rt_resume,
		rt_Invalid
	};
	enum MenuType
	{
		intro,
		map,
		tech,
		numMenuTypes
	};
	static void SetViewPortCenter(float x, float y) { viewportCenter.x = x; viewportCenter.y = y; }
public:
	MenuBase();
	virtual ~MenuBase();
	virtual HRESULT Render(class Graphics& gfx,D2DImage * imageData) = 0;
	virtual UserEventArgs OnMouseClick( MouseButtonEventArgs& mouse) = 0;
	virtual UserEventArgs OnMouseMove(const MouseMotionEventArgs& mouse) = 0;

	virtual void OnUpdate(const UpdateEventArgs& args) = 0;

protected:

};
typedef std::unordered_map<MenuBase::MenuType, std::unique_ptr<MenuBase>> MenuMap;
typedef MenuBase::MenuType MenuTypes;
typedef MenuBase::button Button;

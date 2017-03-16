#pragma once
#include "Events.h"
#include "StringHandler.h"
#include "Graphics.h"
#include "Table.h"
class Meter
{
	D2D1_RECT_F frame;
	D2D1_RECT_F slider;
	float m_width;
	float m_max;
	float m_current;
	D2D1_COLOR_F frameColor;
	D2D1_COLOR_F sliderColor;
	bool drawFrame = false;
	
public:
	
	Meter(D2D1_RECT_F frame,float max, D2D1_COLOR_F frmColor, bool drawframe = true)
		:frame(frame),m_max(max),frameColor(frmColor),drawFrame(drawframe)
	{
		slider.left = frame.left;
		slider.top = frame.top + 2.0f;
		slider.right = slider.left;
		slider.bottom = frame.bottom - 2.0f;
		m_width = frame.right - frame.left;
		m_current = 0.0f;
	};
	void Update(const float& position)
	{
		
		
		m_current >=1.0f ? m_current = 1.0f : m_current = position / m_max;
		slider.right = slider.left +  (m_width * m_current);
		if (m_current < 0.25f)
			sliderColor = D2D1::ColorF(1.0f, 0.0f, 0.0f, 0.55f);
		else if( m_current >= 0.25f && m_current < 0.75f)
			sliderColor = D2D1::ColorF(0.0f, 1.0f, 1.0f, 0.55f);
		else
			sliderColor = D2D1::ColorF(0.0f, 1.0f, 0.0f, 0.55f);

	};
	std::wstring PercentAsString() 
	{
		std::wstring str;
		DXStringHandler::DXFloatPercisionWString(str, m_current, 2);
		return str;
	}
	void Draw(Graphics& gfx)
	{
		if (drawFrame)
			gfx.D2D_DrawRectangle(nullptr, frame, frameColor);
		if (m_current > 0.0f)
		{
			gfx.D2D_DrawFilledRectangle(nullptr, slider, sliderColor);
		}

	}
};

class InfoPanel
{
	Table<D2D1_POINT_2F> mGrid;
	D2D1_RECT_F frame;
	std::unordered_map<std::string,std::unique_ptr<Meter>> m_meters;
	D2D1_ROUNDED_RECT rRect;
public:
	InfoPanel();
	InfoPanel(const D2D1_RECT_F& frame);
	~InfoPanel();
	void Draw(class Graphics& gfx);
	void Update(const UpdateEventArgs& e);
	int OnMouseMove(MouseMotionEventArgs& e);
};


#pragma once
#include "IncludePCH.h"

class  Drawable
{
public:
	Drawable() {}
	virtual HRESULT Draw(class Graphics& gfx) = 0;
};
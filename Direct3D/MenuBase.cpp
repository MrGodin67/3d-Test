#include "IncludePCH.h"
#include "MenuBase.h"

D2D1_COLOR_F MenuBase::frameColor = { 1.0f,1.0f,1.0f,1.0f };
D2D1_COLOR_F MenuBase::fillColor = { 0.0f,0.0f,0.2f,0.5f };
D2D1_POINT_2F MenuBase::viewportCenter = { 0.0f,0.0f};
MenuBase::MenuBase()
{
}


MenuBase::~MenuBase()
{
}

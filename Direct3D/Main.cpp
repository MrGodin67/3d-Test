
#include "IncludePCH.h"

#include "Direct3DWindow.h"
#include "Meteor.h"
#include "SimpleTimer.h"
const static bool FULL_SCREEN = false;
const static bool V_SYNC = true;
const static int SCREEN_WIDTH = 1024;
const static int SCREEN_HEIGHT = 768;
const static WCHAR* APPLICATION_NAME = L"Meteor Shower";
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Direct3DWindow D3D11Window(SCREEN_WIDTH, SCREEN_HEIGHT, APPLICATION_NAME, V_SYNC,FULL_SCREEN);
	Meteor app(D3D11Window);
	while (D3D11Window.ProcessMessage())
	{};
	D3D11Window.Shutdown();
	return 0;
}
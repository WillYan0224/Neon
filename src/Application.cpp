#include "Application.h"

Application::Application()
	:
	wnd( 960, 540, "NeonDirectX11")
{
}

int Application::Run()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
		DoFrame();
	}
}


void Application::DoFrame()
{
	wnd.Gfx().EndFrame();
}

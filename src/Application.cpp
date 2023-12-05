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
	
	const float color = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(1.0f, 0.5f, color);
	wnd.Gfx().DrawTestTriangle(
		-timer.Peek(),
		0.0f,
		0.0f
		);
	wnd.Gfx().DrawTestTriangle(
		timer.Peek(),
		(wnd.mouse.GetPosX() / 480.0f) - 1.0f,
		(-wnd.mouse.GetPosY() / 270.0f) + 1.0f
	);
	wnd.Gfx().EndFrame();
}

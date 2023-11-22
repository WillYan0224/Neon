#include "Application.h"
#include <sstream>

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
	std::ostringstream oss;
	const float t = timer.Peek();
	oss << "elapsed : " << std::setprecision(2) << std::fixed << t << 's';
	wnd.SetTitle(oss.str());
}

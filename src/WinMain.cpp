#include "Window.h"
#include <sstream>

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	try {
		Window wnd( 960, 540, "NeonDirectX" );

		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage( &msg, nullptr, 0, 0 ) ) > 0)
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );

			// test code
			while (!wnd.mouse.IsEmpty())
			{
				const auto e = wnd.mouse.Read();
				switch (e.GetType())
				{
				case Mouse::Event::Type::Leave:
					wnd.SetTitle("Gone!");
					break;
				case Mouse::Event::Type::Move:
				{
					std::ostringstream oss;
					oss << "Mouse moved to (" << e.GetPosX() << "," << e.GetPosY() << ")";
					wnd.SetTitle(oss.str());
				}
				break;
				}
			}

		}

		if (gResult == -1)
		{
			return -1;
		}

		return static_cast<int>(msg.wParam);
	}
	catch (const NeonException& exception)
	{
		MessageBox(nullptr, exception.what(), exception.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (std::exception& exception)
	{
		MessageBox(nullptr, exception.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No detail", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}
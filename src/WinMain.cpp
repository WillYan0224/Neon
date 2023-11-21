#include "Window.h"


int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	try {
		Window wnd( 1920, 1080, "NeonDirectX" );

		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage( &msg, nullptr, 0, 0 ) ) > 0)
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
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
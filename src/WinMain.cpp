#include "Window.h"
#include <sstream>
#include "Application.h"

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	try {
		return Application{}.Run();
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
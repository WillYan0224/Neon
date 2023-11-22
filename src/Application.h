#pragma once
#include "Window.h"
#include "NeonTimer.h"

class Application
{
public:
	Application();

	int Run();

private:
	void DoFrame();

private:
	Window wnd;
	NeonTimer timer;
};
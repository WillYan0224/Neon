#pragma once
#include "Window.h"
#include "NeonTimer.h"

class Application
{
public:
	Application();

	int Run();
	~Application();
private:
	void DoFrame();

private:
	Window wnd;
	NeonTimer timer;
	std::vector < std::unique_ptr<class Cube>> cubes;
};
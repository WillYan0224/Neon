#pragma once
#include "Window.h"
#include "NeonTimer.h"
#include "ImguiManager.h"
class Application
{
public:
	Application();

	int Run();
	~Application();
private:
	void DoFrame();

private:
	ImguiManager imgui;
	Window wnd;
	NeonTimer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawables = 180;
};
#pragma once
#include "Window.h"
#include "NeonTimer.h"
#include "ImguiManager.h"
#include "Imgui/imgui.h"
#include "Camera.h"
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
	Camera cam;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawables = 180;
	float speed_factor = 1.0f;
};
#pragma once
#include "Window.h"
#include "NeonTimer.h"
#include "ImguiManager.h"
#include "Imgui/imgui.h"
#include "Camera.h"
#include "PointLight.h"
#include <set>

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
	PointLight light;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	std::vector<class Box*> boxes;
	std::optional<int> comboBoxIndex;
	std::set<int> boxControlIds;
	static constexpr size_t nDrawables = 240;
	float speed_factor = 1.0f;

};
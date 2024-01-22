#pragma once
#include "Window.h"
#include "NeonTimer.h"
#include "ImguiManager.h"
#include "Imgui/imgui.h"
#include "Camera.h"
#include "PointLight.h"
#include "Model.h"
#include <set>

class Application
{
public:
	Application();

	int Run();
	~Application();
private:
	void DoFrame();
	void ShowModelWindow();
private:
	ImguiManager imgui;
	Window wnd;
	NeonTimer timer;
	Camera cam;
	PointLight light;
	float speed_factor = 1.0f;
	Model nano{ wnd.Gfx(),"src\\Models\\nanosuit.obj" };
	struct
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} pos;
};
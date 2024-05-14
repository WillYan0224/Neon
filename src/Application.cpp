#include "Application.h"

#include <memory>
#include <algorithm>
#include "NeonMath.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "Vertex.h"

#include "AssimpTest.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>


#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"

namespace dx = DirectX;

GDIPlusManager gdipm;

Application::Application()
	:
	wnd(1600, 900, "DX11 Showcase"),
	light(wnd.Gfx())
{
	
//	wnd.Gfx().SetProjection( dx::XMMatrixPerspectiveLH( 1.0f, 9.0 / 16.0,0.5f,40.0f ) );
	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveFovLH(90.0f * (NEON::PI / 180), 16 / 9.0f, 1.0f, 40.0f));
}

void Application::DoFrame()
{	
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
	ImGuiIO io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::Begin("DockSpace Demo", nullptr, window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	const auto dt = timer.Mark() * speed_factor;
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx(), cam.GetMatrix());
	const auto transform = dx::XMMatrixRotationRollPitchYaw(pos.roll, pos.pitch, pos.yaw) *
		dx::XMMatrixTranslation(pos.x, pos.y, pos.z);
	
	wnd.Gfx().SetViewport(1280.0f, 760.0f, 0.0f, 0.0f);
	// Render to texture first
	wnd.Gfx().RenderToTexture();
	wnd.Gfx().SetRenderTarget();
	wnd.Gfx().ClearBuffer(0.07f, 0.4f, 0.12f);
	nano.Draw(wnd.Gfx(), transform);
	light.Draw(wnd.Gfx());
	
	// swap back buffer
	wnd.Gfx().SetBackBufferRenderTarget();
	wnd.Gfx().ClearBackBuffer(0.12f, 0.12f, 0.12f);
	wnd.Gfx().RenderViewports();
	ImGui::End();
	// imgui window to control camera
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	ShowModelWindow();

	// imgui window to control simulation speed
	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 6.0f, "%.4f", 3.2f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold spacebar to pause)");
	}

	wnd.Gfx().EndFrame();
}

Application::~Application()
{}


int Application::Run()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}

void Application::ShowModelWindow()
{
	if (ImGui::Begin("Model"))
	{
		using namespace std::string_literals;

		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &pos.roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pos.pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &pos.yaw, -180.0f, 180.0f);

		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -20.0f, 20.0f);
		ImGui::SliderFloat("Y", &pos.y, -20.0f, 20.0f);
		ImGui::SliderFloat("Z", &pos.z, -20.0f, 20.0f);
	}
	ImGui::End();
}
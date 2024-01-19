#include "Application.h"
#include "FlatSphere.h"
#include "Pyramid.h"
#include "Cylinder.h"
#include "Box.h"
#include "SkinnedBox.h"
#include "Sheet.h"
#include "SkinnedBox.h"
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
	wnd(1280, 760, "DX11 Showcase"),
	light(wnd.Gfx())
{
	class Factory
	{
	public:
		Factory( Graphics& gfx )
			:
			gfx( gfx )
		{}
		std::unique_ptr<Drawable> operator()()
		{
			const DirectX::XMFLOAT3 mat = { cdist( rng ),cdist( rng ),cdist( rng ) };

			switch( sdist( rng ) )
			{
			case 0:
				return std::make_unique<Box>(
					gfx,rng,adist,ddist,
					odist,rdist,bdist,mat
				);
			case 1:
				return std::make_unique<Cylinder>(
					gfx,rng,adist,ddist,odist,
					rdist,bdist,tdist
				);
			case 2:
				return std::make_unique<Pyramid>(
					gfx,rng,adist,ddist,odist,
					rdist,tdist
				);
			case 3:
				return std::make_unique<AssTest>(
					gfx, rng, adist, ddist,
					odist, rdist, mat, 1.5f
				);
			default:
				assert( false && "impossible drawable option in factory" );
				return {};
			}
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_int_distribution<int> sdist{ 0,3 };
		std::uniform_real_distribution<float> adist{ 0.0f,NEON::PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,NEON::PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,NEON::PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_real_distribution<float> cdist{ 0.0f,1.0f };
		std::uniform_int_distribution<int> tdist{ 3,30 };
	};

	drawables.reserve( nDrawables );
	std::generate_n( std::back_inserter( drawables ),nDrawables,Factory{ wnd.Gfx() } );

	// init box pointers for editing instance parameters
	for( auto& pd : drawables )
	{
		if( auto pb = dynamic_cast<Box*>(pd.get()) )
		{
			boxes.push_back( pb );
		}
	}

	wnd.Gfx().SetProjection( dx::XMMatrixPerspectiveLH( 1.0f, 9.0 / 16.0,0.5f,40.0f ) );
}

void Application::DoFrame()
{	
	const auto dt = timer.Mark() * speed_factor;
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx(), cam.GetMatrix());
	for( auto& d : drawables )
		{
			d->Update( wnd.kbd.KeyIsPressed( VK_SPACE ) ? 0.0f : dt );
			d->Draw( wnd.Gfx() );
		}
	light.Draw(wnd.Gfx());

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
	ImGuiIO io = ImGui::GetIO();
	ImGui::Begin("DockSpace Demo", nullptr, window_flags);
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	ImGui::End();
	// imgui window to control camera
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	
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
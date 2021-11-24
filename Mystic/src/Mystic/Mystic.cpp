#include <iostream>
#include <chrono>
#include "entt/entity/registry.hpp"
#include "GFX/Renderer3D.h"
#include "GFX/primitives.h"

#include "GFX/Shader.h"

unsigned int SCREEN_WIDTH = 800;
unsigned int SCREEN_HEIGHT = 600;


void RenderLoop()
{
	float dt = 0.0f;
	int fpsCounts = 0;
	while (!Mystic::Renderer3D::ShouldCloseWindow())
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		
		Mystic::Renderer3D::ClearScreen();

		if (Mystic::Renderer3D::GetKeyState(MYSTIC_KEY_ESCAPE))
		{
			Mystic::Renderer3D::SetWindowShouldClose(true);
		}

		if (Mystic::Renderer3D::GetKeyState(MYSTIC_KEY_W))
		{
			//world->GetPlayer()->ProcessKeyBoard(CameraDirection::Forward, 50, dt);
		}
		if (Mystic::Renderer3D::GetKeyState(MYSTIC_KEY_S))
		{
			//world->GetPlayer()->ProcessKeyBoard(CameraDirection::Backward, 50, dt);
		}
		if (Mystic::Renderer3D::GetKeyState(MYSTIC_KEY_A))
		{
			//world->GetPlayer()->ProcessKeyBoard(CameraDirection::Left, 50, dt);
		}
		if (Mystic::Renderer3D::GetKeyState(MYSTIC_KEY_D))
		{
			//world->GetPlayer()->ProcessKeyBoard(CameraDirection::Right, 50, dt);
		}
		if (Mystic::Renderer3D::GetMouseButtonState(MYSTIC_MOUSE_BUTTON_LEFT))
		{
			//world->GetPlayer()->ProcessLeftMouseClick();
		}
		if (Mystic::Renderer3D::GetMouseButtonState(MYSTIC_MOUSE_BUTTON_RIGHT))
		{
			//world->GetPlayer()->ProcessRightMouseClick();
		}

		//GlobalEventManager::ProcessEvents();
		
		//world->Update(dt);

		Mystic::Renderer3D::SwapBuffers();
		Mystic::Renderer3D::PollEvents();

		auto stopTime = std::chrono::high_resolution_clock::now();

		dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();

		fpsCounts++;
		if (fpsCounts > 6)
		{
			std::cout << dt * 1000 << std::endl;
			
			fpsCounts = 0;
		}
		
	}
}

int main()
{
    std::cout << "Hello World!\n";

	Mystic::Renderer3D::Init();
	Mystic::Renderer3D::OpenScene(SCREEN_WIDTH, SCREEN_HEIGHT, "MysticEngine");

	RenderLoop();
}
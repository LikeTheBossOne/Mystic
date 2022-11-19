#pragma once
#include <functional>

#include "Application.h"
#include "../ECS/ComponentRegistry.h"
#include "../ECS/Components/Renderable.h"
//#include "OnRender/Renderer3D.h"
//#include "OnRender/Window.h"
//#include "Platform/OpenGL/OpenGLWindow.h"
#include "../Scene/ProjectScene.h"

#ifdef MYST_PLATFORM_WINDOWS

extern Mystic::Application* Mystic::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	auto app = Mystic::CreateApplication({ argc, argv });

	app->Run();

	delete app;

	return 0;
}



#endif
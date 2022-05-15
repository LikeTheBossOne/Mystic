#include <algorithm>
#include <iostream>
#include <Mystic.h>
#include "Mystic/Core/EntryPoint.h"
#include <vector>

#include "Mystic/Render/Renderer.h"
#include "SandboxLayer.h"
#include "Mystic/Scene/ProjectScene.h"

class Sandbox : public Mystic::Application
{
public:
	Sandbox(Mystic::ApplicationCommandLineArgs args) : Application("Sandbox", args)
	{
		PushLayer(new SandboxLayer());
	}

	~Sandbox()
	{
		
	}
	
};

Mystic::Application* Mystic::CreateApplication(ApplicationCommandLineArgs args)
{
	return new Sandbox(args);
}
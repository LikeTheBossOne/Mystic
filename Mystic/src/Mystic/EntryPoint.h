#pragma once
#include "Application.h"

#ifdef MYST_PLATFORM_WINDOWS

extern Mystic::Application* Mystic::CreateApplication();

int main(int argc, char* argv[])
{
	auto app = Mystic::CreateApplication();
	app->Run();
	delete app;
}

#endif
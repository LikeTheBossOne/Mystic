#include "GameLayer.h"
#include "Mystic/Core/EntryPoint.h"

class GameApp : public Mystic::Application
{
public:
	GameApp(Mystic::ApplicationCommandLineArgs args) : Application("Game", args)
	{
		PushLayer(new Game::GameLayer());
	}

	~GameApp()
	{

	}
};

Mystic::Application* Mystic::CreateApplication(Mystic::ApplicationCommandLineArgs args)
{
	return new GameApp(args);
}

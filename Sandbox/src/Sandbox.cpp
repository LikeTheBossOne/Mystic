#include <Mystic.h>

#include "Mystic/Editor/Editor.h"
#include "Mystic/GFX/Renderer2D.h"

class Sandbox : public Mystic::Application
{
public:
	Sandbox()
	{
		
	}

	~Sandbox()
	{
		
	}

	inline void Run() override
	{
		/*Mystic::Renderer2D::Init();
		Mystic::Renderer2D::OpenScene(1000, 1000, "SFML WINDOW");
		Mystic::Renderer2D::SetFramerateLimit(60);
		
		int rotation = 0;
		while (true)
		{
			Mystic::Renderer2D::ClearScreen();
			Mystic::Renderer2D::RenderQuad(glm::vec2(100, 500), glm::vec2(80, 40), rotation % 360, glm::vec4(255, 0, 0, 255));
			Mystic::Renderer2D::RenderQuad(glm::vec2(200, 500), glm::vec2(40, 40), rotation % 360, glm::vec4(255, 0, 0, 255));
			Mystic::Renderer2D::RenderQuad(glm::vec2(300, 500), glm::vec2(40, 40), rotation % 360, glm::vec4(255, 0, 0, 255));
			Mystic::Renderer2D::RenderQuad(glm::vec2(400, 500), glm::vec2(40, 40), rotation % 360, glm::vec4(255, 0, 0, 255));
			Mystic::Renderer2D::RenderQuad(glm::vec2(500, 500), glm::vec2(40, 40), rotation % 360, glm::vec4(255, 0, 0, 255));
			Mystic::Renderer2D::RenderQuad(glm::vec2(600, 500), glm::vec2(40, 40), rotation % 360, glm::vec4(255, 0, 0, 255));
			Mystic::Renderer2D::RenderQuad(glm::vec2(700, 500), glm::vec2(40, 40), rotation % 360, glm::vec4(255, 0, 0, 255));
			Mystic::Renderer2D::SwapBuffers();

			rotation++;
		}*/

		Mystic::Editor::Init(1000, 1000, "Editor Window");
		Mystic::Editor::Start();
	}
};

Mystic::Application* Mystic::CreateApplication()
{
	return new Sandbox();
}
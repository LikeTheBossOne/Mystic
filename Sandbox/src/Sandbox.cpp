#include <algorithm>
#include <Mystic.h>
#include <vector>

#include "Mystic/Editor/Editor.h"
#include "Mystic/GFX/Renderer2D.h"
//#include "entt.hpp"

class Sandbox : public Mystic::Application
{
public:
	Sandbox()
	{
		
	}

	~Sandbox()
	{
		
	}

	inline void Start() override
	{

	}

	inline void Update() override
	{
		
	}

	inline void Render() override
	{
		Mystic::Renderer2D::RenderTriangle();
	}
};

Mystic::Application* Mystic::CreateApplication()
{
	return new Sandbox();
}
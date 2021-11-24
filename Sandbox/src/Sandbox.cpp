#include <Mystic.h>

class Sandbox : public Mystic::Application
{
public:
	Sandbox()
	{
		
	}

	~Sandbox()
	{
		
	}
};

Mystic::Application* Mystic::CreateApplication()
{
	return new Sandbox();
}
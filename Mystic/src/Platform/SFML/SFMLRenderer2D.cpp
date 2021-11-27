#include "SFMLRenderer2D.h"

namespace Mystic
{
	SFMLRenderer2D::SFMLRenderer2D()
	{
		_window = nullptr;
	
	}

	bool SFMLRenderer2D::CreateWindow(uint32_t width, uint32_t height, std::string title)
	{
		_window = new sf::RenderWindow(sf::VideoMode(width, height), title);
		return true;
	}

	void SFMLRenderer2D::RenderQuad(glm::vec2 location, glm::vec2 size, float rotationDegs, glm::vec4 color)
	{
		float originOffsetX = size.x / 2;
		float originOffsetY = size.y / 2;
		sf::RectangleShape rect(sf::Vector2f(size.x, size.y));
		rect.setOrigin(originOffsetX, originOffsetY);
		rect.setPosition(location.x + originOffsetX, location.y + originOffsetY);
		rect.setRotation(rotationDegs);
		rect.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
		_window->draw(rect);
	}

	void SFMLRenderer2D::ClearScreen()
	{
		_window->clear(sf::Color::Black);
	}

	bool SFMLRenderer2D::GetKeyState(int16_t key)
	{
		return false;
	}

	bool SFMLRenderer2D::GetMouseButtonState(int8_t button)
	{
		return false;
	}

	void SFMLRenderer2D::SwapBuffers()
	{
		_window->display();
	}

	void SFMLRenderer2D::PollEvents()
	{
		sf::Event e;
		_window->pollEvent(e);
	}

	void SFMLRenderer2D::SetFramerateLimit(uint32_t limit)
	{
		_window->setFramerateLimit(limit);
	}
}

#include "SFMLRenderer2D.h"

namespace Mystic
{
	SFMLRenderer2D::SFMLRenderer2D()
	{
		_renderTarget = nullptr;
	}

	SFMLRenderer2D::~SFMLRenderer2D()
	{
	}
	
	bool SFMLRenderer2D::CreateWindow(uint32_t width, uint32_t height, std::string title)
	{
		sf::RenderTexture* tex = new sf::RenderTexture();
		tex->create(width, height, sf::ContextSettings{0, 0, 0, 1, 1, sf::ContextSettings::Default, false});
		_renderTarget = std::unique_ptr<sf::RenderTarget>(tex);
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
		_renderTarget->draw(rect);
	}

	void SFMLRenderer2D::ClearScreen()
	{
		//_renderTarget->clear(sf::Color::Black);
	}

	bool SFMLRenderer2D::GetKeyState(int16_t key) const
	{
		return false;
	}

	bool SFMLRenderer2D::GetMouseButtonState(int8_t button) const
	{
		return false;
	}

	void SFMLRenderer2D::SwapBuffers()
	{
		if (sf::RenderTexture* tex = static_cast<sf::RenderTexture*>(_renderTarget.get()))
		{
			tex->display();
		}
	}

	void SFMLRenderer2D::PollEvents()
	{
		//sf::Event e;
		//_renderTarget->pollEvent(e);
	}

	void SFMLRenderer2D::SetFramerateLimit(uint32_t limit)
	{
		//_renderTarget->setFramerateLimit(limit);
	}

	uint32_t SFMLRenderer2D::GetTextureHandle() const
	{
		assert(_renderTarget, "RenderTexture target is null");

		if (sf::RenderTexture* tex = static_cast<sf::RenderTexture*>(_renderTarget.get()))
		{
			return tex->getTexture().getNativeHandle();
		}
		else
		{
			assert(false, "RenderTarget was not a texture");
			return 100;
		}
		
	}
}

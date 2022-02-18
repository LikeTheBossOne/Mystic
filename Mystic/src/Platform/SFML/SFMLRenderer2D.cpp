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

	void SFMLRenderer2D::RenderTriangle()
	{
		
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

	void SFMLRenderer2D::HandleJobs()
	{
		
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

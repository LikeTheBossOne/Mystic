#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include "../../Mystic/GFX/Renderer2D.h"

#include "SFML/Graphics.hpp"

namespace Mystic
{
	/**
	 * This is not super performant because you construct a shape or sprite/texture every time.
	 * In the future, this should be optimized to reuse shapes/textures or should just switch to 2D OpenGL
	 */
	class SFMLRenderer2D : public InstRenderer2D
	{
		public:
		SFMLRenderer2D();
	
		bool CreateWindow(uint32_t width, uint32_t height, std::string title) override;
		void RenderQuad(glm::vec2 location, glm::vec2 size, float rotationDegs, glm::vec4 color) override;
		void ClearScreen() override;
		bool GetKeyState(int16_t key) override;
		bool GetMouseButtonState(int8_t button) override;
		void SwapBuffers() override;
		void PollEvents() override;
		void SetFramerateLimit(uint32_t limit) override;

		private:
		sf::RenderWindow* _window;
	};
}

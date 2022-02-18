#pragma once
#include <glm/vec2.hpp>

class Window
{
public:
	Window();
	virtual ~Window();


	virtual void SwapBuffer();
	virtual glm::uvec2 GetSize();
	virtual void Resize(uint32_t width, uint32_t height);
	virtual void Resize(glm::uvec2 size);
};

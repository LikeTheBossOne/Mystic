#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../Mystic/GFX/Window.h"

namespace Mystic
{
    class MYSTIC_API OpenGLWindow :
        public Window
    {
    public:
        OpenGLWindow();

        void OpenWindow(int width, int height) override;
        void Clear(float r, float g, float b, float a) override;
        void SwapBuffer() override;
        bool ShouldClose() override;
        void PollEvents() override;

    private:
        GLFWwindow* _window;
    };
}

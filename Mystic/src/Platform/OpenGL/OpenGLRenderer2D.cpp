#include "OpenGLRenderer2D.h"
#include "../../Mystic/GFX/Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include "../../Mystic/GFX/Camera.h"

namespace Mystic
{
	Mystic::OpenGLRenderer2D::OpenGLRenderer2D()
	{
		
	}

	Mystic::OpenGLRenderer2D::~OpenGLRenderer2D()
	{
        glDeleteVertexArrays(1, &_VAO);
        glDeleteBuffers(1, &_VBO);
        glDeleteProgram(_shaderProgram);
	}

    void glfw_onError(int error, const char* description)
    {
        // print message in Windows popup dialog box
        std::cout << description << std::endl;
    }
    
    void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

	bool Mystic::OpenGLRenderer2D::CreateWindow(uint32_t width, uint32_t height, std::string title)
	{
        const unsigned int SCR_WIDTH = 800;
        const unsigned int SCR_HEIGHT = 600;

        const char* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        const char* fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\n\0";

        // build and compile our shader program
        // vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // link shaders
        _shaderProgram = glCreateProgram();
        glAttachShader(_shaderProgram, vertexShader);
        glAttachShader(_shaderProgram, fragmentShader);
        glLinkProgram(_shaderProgram);
        // check for linking errors
        glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(_shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left  
             0.5f, -0.5f, 0.0f, // right 
             0.0f,  0.5f, 0.0f  // top   
        };
        
        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);


		return true;
	}

	void Mystic::OpenGLRenderer2D::RenderTriangle()
	{
        glUseProgram(_shaderProgram);
        glBindVertexArray(_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void Mystic::OpenGLRenderer2D::ClearScreen()
	{
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
	}

	bool Mystic::OpenGLRenderer2D::GetKeyState(int16_t key) const
	{
		return false;
	}

	bool Mystic::OpenGLRenderer2D::GetMouseButtonState(int8_t button) const
	{
		return false;
	}

	void Mystic::OpenGLRenderer2D::SwapBuffers()
	{
	}

	void Mystic::OpenGLRenderer2D::PollEvents()
	{
		glfwPollEvents();
	}

	void Mystic::OpenGLRenderer2D::SetFramerateLimit(uint32_t limit)
	{
	}

	uint32_t Mystic::OpenGLRenderer2D::GetTextureHandle() const
	{
		return NULL;
	}
}
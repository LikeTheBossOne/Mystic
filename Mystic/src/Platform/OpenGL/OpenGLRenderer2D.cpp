#include "OpenGLRenderer2D.h"
#include "../../Mystic/GFX/Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"
#include "RenderJob.h"
#include "../../Mystic/GFX/Camera.h"

namespace Mystic
{
	Mystic::OpenGLRenderer2D::OpenGLRenderer2D()
	{
        _jobs = std::vector<Ref<RenderJob>>();
        _meshes = std::unordered_map<std::string, Ref<Mesh>>();
	}

	Mystic::OpenGLRenderer2D::~OpenGLRenderer2D()
	{
        for (std::pair<std::string, Ref<Mesh>> pair : _meshes)
        {
            glDeleteVertexArrays(1, &pair.second->VAO);
            glDeleteBuffers(1, &pair.second->VBO);
        }
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
        _shader = new Shader("..\\Mystic\\assets\\shaders\\vertex1.vs", "..\\Mystic\\assets\\shaders\\fragment1.fs");

        std::string key = "triangle";
        Ref<Mesh> mesh = CreateMesh(key);
        if (mesh != NULL)
        {
            _meshes["triangle"] = mesh;
        }


		return true;
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

	void OpenGLRenderer2D::UseShaderProgram()
	{
        _shader->Use();
	}

	void OpenGLRenderer2D::SetProjectionMatrix(glm::mat4& projectionMatrix)
	{
        glUniformMatrix4fv(glGetUniformLocation(_shader->id, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}

	void OpenGLRenderer2D::SetViewMatrix(glm::mat4& viewMatrix)
	{
        glUniformMatrix4fv(glGetUniformLocation(_shader->id, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	}

	void OpenGLRenderer2D::RenderEnt(std::string& meshKey, glm::mat4& modelMat)
	{
        _jobs.emplace_back(std::make_shared<RenderJob>(meshKey, modelMat));
	}

	void OpenGLRenderer2D::HandleJobs()
	{
        for (Ref<RenderJob> job : _jobs)
        {
            if (_meshes.find(job->MeshKey) == _meshes.end())
            {
                assert(true, "Couldn't find mesh for " + job->MeshKey);
            }

			//TODO: Pass view (transform) and projection (camera) matrix into shaders
            glUniformMatrix4fv(glGetUniformLocation(_shader->id, "model"), 1, GL_FALSE, glm::value_ptr(*job->ModelMat.get()));

            glBindVertexArray(_meshes[job->MeshKey]->VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        _jobs.clear();
	}

	uint32_t Mystic::OpenGLRenderer2D::GetTextureHandle() const
	{
		return NULL;
	}

    //TODO: Update this to use another parameter for vertices, or load vertices from a file.
	Ref<Mesh> OpenGLRenderer2D::CreateMesh(std::string& key)
	{
        const Ref<Mesh> mesh = std::make_shared<Mesh>(key);

        glGenVertexArrays(1, &mesh->VAO);
        glGenBuffers(1, &mesh->VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(mesh->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);


        if (key == "triangle")
        {
            const float vertices[] = {
                -0.5f, -0.5f, 0.0f, // left  
                 0.5f, -0.5f, 0.0f, // right 
                 0.0f,  0.5f, 0.0f  // top   
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        }
        else
        {
            return NULL;
        }


        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        return mesh;
	}
}

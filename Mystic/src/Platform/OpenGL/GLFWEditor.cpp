#include "GLFWEditor.h"

#include "../../../ImGui/imgui.h"
#include "../../../ImGui/imgui_impl_glfw.h"
#include "../../../ImGui/imgui_impl_opengl3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <iostream>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "../../Mystic/GFX/Renderer2D.h"
#include "../../Mystic/Application.h"
#include "../../Mystic/Game.h"
#include "../../Mystic/Scene/SceneSerializer.h"
#include "../../Mystic/Scene/ProjectScene.h"

#include "../../Mystic/ECS/Components/Transform.h"
#include "../../Mystic/ECS/Components/Renderable.h"

namespace Mystic
{
	GLFWEditor::GLFWEditor()
	{
        _window = nullptr;
		_width = 800;
		_height = 600;
		_appRunning = false;

		_projectScene = std::make_shared<ProjectScene>();

		/*SceneSerializer serializer(_projectScene);
		std::string scenePath = "Scenes\\Scene1.yaml";
		serializer.DeserializeScene(scenePath);*/

		// Instead let's do some sample project generation
		Ref<entt::registry> reg = _projectScene->GetRegistry();

		const std::string key = "triangle";
		std::srand(std::time(nullptr));
		for (int i = 0; i < 10; i++)
		{
			Entity entInst = _projectScene->CreateEntity();
			const entt::entity ent = entInst.EntId;
			Transform transform =
			{
				glm::vec3((std::rand() % 20) - 10.f, (std::rand() % 20) - 10.f, (std::rand() % 20) - 30.f),
				glm::quat(glm::vec3((std::rand() % 180) - 90.f, (std::rand() % 360), (std::rand() % 360))),
				glm::vec3((std::rand() % 5), (std::rand() % 5), (std::rand() % 5))
			};
			reg->emplace<Transform>(ent, transform);
			Renderable renderable = { key };
			reg->emplace<Renderable>(ent, renderable);
		}

	}

	GLFWEditor::~GLFWEditor()
	{
	}
	void GLFWEditor::Init(int windowWidth, int windowHeight, std::string windowTitle, std::function<Application* ()> applicationCreater)
	{
		_width = windowWidth;
		_height = windowHeight;
		_appCreater = applicationCreater;
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}

	void GLFWEditor::OnStart()
	{
		// glfw: initialize and configure
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		// glfw window creation
		_window = glfwCreateWindow(_width, _height, "LearnOpenGL", NULL, NULL);
		if (_window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(_window);
		glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(_window, true);
		ImGui_ImplOpenGL3_Init("#version 330");


        Renderer2D::OpenScene(1, 1, "");

		// Setup FBO for ImGui::Image rendering
		glGenFramebuffers(1, &_FBO);
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer was not valid");
		glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

		glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 400, 400, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);



        _show_demo_window = false;
        _show_another_window = false;
        _clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        return;
	}

	void GLFWEditor::OnEnd()
	{
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glDeleteBuffers(1, &_FBO);
		glDeleteBuffers(1, &_texture);

		glfwDestroyWindow(_window);
		glfwTerminate();
	}

	void GLFWEditor::PreRender()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void GLFWEditor::Render()
	{
		glClearColor(_clear_color.x * _clear_color.w, _clear_color.y * _clear_color.w, _clear_color.z * _clear_color.w, _clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (_show_demo_window)
			ImGui::ShowDemoWindow(&_show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &_show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &_show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&_clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			
			ImGui::End();
		}

		{
			ImGui::Begin("Game");

			if (ImGui::Button("Start/Stop", ImVec2(60, 20)))
			{
				if (!_appRunning)
				{
					StartGame();
				}
				else
				{
					StopGame();
				}
				
			}
			BindFramebuffer();
			Renderer2D::ClearScreen();
			glViewport(0, 0, 400, 400);
			Renderer2D::HandleJobs();
			UnbindFramebuffer();

			ImGui::Image((void*)_texture, ImVec2(800, 800), ImVec2(1, 1), ImVec2(0, 0));
			
			ImGui::End();
		}

		// 3. Show another simple window.
		if (_show_another_window)
		{
			ImGui::Begin("Another Window", &_show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				_show_another_window = false;
			ImGui::End();
		}

		// Rendering
		ImGui::Render();

		int display_w, display_h;
		glfwGetFramebufferSize(_window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		//Mystic::Renderer2D::RenderTriangle();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(_window);
	}

	void GLFWEditor::PostRender()
	{
		if (_appRunning && _app && _game)
		{
			_game->Update();
		}
		else
		{
			_projectScene->RenderScene();
		}
	}

	void GLFWEditor::ShowGameWindow()
	{

	}

	void GLFWEditor::BindFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	}

	void GLFWEditor::UnbindFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	bool GLFWEditor::ShouldClose()
	{
		return glfwWindowShouldClose(_window);
	}

	bool GLFWEditor::PollEvents()
	{
		glfwPollEvents();
		return true;
	}

	void GLFWEditor::StartGame()
	{
		_app = Ref<Application>(_appCreater());

		Ref<RuntimeScene> runtimeScene = _projectScene->CreateRuntimeScene();

		_game = std::make_shared<Game>(_app, runtimeScene);
		_app->Start(_game);


		_appRunning = true;
	}

	void GLFWEditor::StopGame()
	{
		SceneSerializer serializer(_projectScene);
		std::string scenePath = "Scenes\\Scene1.yaml";
		serializer.SerializeScene(scenePath);

		_app->Close();
		_app = nullptr;
		_game = nullptr;

		_appRunning = false;
	}
}



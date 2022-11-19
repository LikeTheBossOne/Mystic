#include "Application.h"

#include "Window.h"
#include "GLFW/glfw3.h"
#include "Mystic/Render/Renderer.h"
#include "Mystic/ImGui/ImGuiLayer.h"
#include "Mystic/Core/LayerStack.h"
#include "Mystic/ECS/ComponentRegistry.h"
#include "Mystic/ECS/SystemRegistry.h"
#include "Mystic/ECS/Components/CameraComponent.h"
#include "Mystic/ECS/Components/CharacterComponent.h"
#include "Mystic/ECS/Components/MeshRendererComponent.h"
#include "Mystic/ECS/Components/Renderable.h"
#include "Mystic/ECS/Components/RigidBodyComponent.h"
#include "Mystic/ECS/Components/SpriteRendererComponent.h"
#include "Mystic/ECS/Components/TagComponent.h"
#include "Mystic/ECS/Components/ThrusterComponent.h"
#include "Mystic/ECS/Components/TransformComponent.h"
#include "Mystic/ECS/Components/VelocityComponent.h"
#include "Mystic/ECS/Systems/AccelerationSystem.h"
#include "Mystic/ECS/Systems/CharacterMovementSystem.h"
#include "Mystic/ECS/Systems/ThrusterMovementSystem.h"
#include "Mystic/ECS/Systems/VelocitySystem.h"
#include "Mystic/Events/ApplicationEvent.h"
#include "singleton.h"

namespace Mystic
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name, ApplicationCommandLineArgs args) : _running(true), _minimized(false), _lastFrameTime(0.0f), _commandLineArgs(args)
	{
		assert(!s_Instance, "Application already exists");

		s_Instance = this;

		_window = Window::Create();
		_window->SetEventCallback(MYST_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		_imGuiLayer = new ImGuiLayer();
		PushOverlay(_imGuiLayer);

		ComponentRegistry::AddComponent<GUIDComponent>();
		ComponentRegistry::AddComponent<TagComponent>();
		ComponentRegistry::AddComponent<TransformComponent>();
		ComponentRegistry::AddComponent<VelocityComponent>();
		ComponentRegistry::AddComponent<SpriteRendererComponent>();
		ComponentRegistry::AddComponent<Renderable>();
		ComponentRegistry::AddComponent<CameraComponent>();
		ComponentRegistry::AddComponent<CharacterComponent>();
		ComponentRegistry::AddComponent<RigidBodyComponent>();
		ComponentRegistry::AddComponent<ThrusterComponent>();
		ComponentRegistry::AddComponent<MeshRendererComponent>();

		SystemRegistry::AddSystem<CharacterMovementSystem>("CharacterMovementSystem");
		SystemRegistry::AddSystem<VelocitySystem>("VelocitySystem");
		SystemRegistry::AddSystem<AccelerationSystem>("AccelerationSystem");
		SystemRegistry::AddSystem<ThrusterMovementSystem>("ThrusterMovementSystem");
	}

	Application::~Application()
	{
		Renderer::Shutdown();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(MYST_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(MYST_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = _layerStack.rbegin(); it != _layerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		_layerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run()
	{
		while (_running)
		{
			float time = (float)glfwGetTime();
			float timestep = time - _lastFrameTime;
			_lastFrameTime = time;

			if (!_minimized)
			{
				for (Layer* layer : _layerStack)
					layer->OnUpdate(timestep);

				_imGuiLayer->Begin();
				{
					for (Layer* layer : _layerStack)
						layer->OnImGuiRender();
				}
				_imGuiLayer->End();
			}

			_window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		_running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			_minimized = true;
			return false;
		}

		_minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	void Application::Close()
	{
		_running = false;
	}
}

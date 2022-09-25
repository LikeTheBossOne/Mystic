#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <vector>

#include "Core.h"
#include "LayerStack.h"
#include "MMacros.h"

int main(int argc, char** argv);

namespace Mystic
{
	class Window;
	class WindowResizeEvent;
	class WindowCloseEvent;
	class Layer;
	class ImGuiLayer;
	class Event;
	class Game;

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			assert(index < Count);
			return Args[index];
		}
	};

	class MYSTIC_API Application
	{
	public:
		Application(const std::string& name = "Mystic Game", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return *_window; }
		ImGuiLayer* GetImGuiLayer() { return _imGuiLayer; }

		void Close();

		ApplicationCommandLineArgs GetCommandLineArgs() const { return _commandLineArgs; }

	private:
		void Run();

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		bool _running;
		float _lastFrameTime;
		bool _minimized;

		ImGuiLayer* _imGuiLayer;
		LayerStack _layerStack;
		Unique<Window> _window;
		ApplicationCommandLineArgs _commandLineArgs;

	private:
		friend int ::main(int argc, char** argv);
	};

	// To be defined in client
	Mystic::Application* CreateApplication(ApplicationCommandLineArgs args);
}


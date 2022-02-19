#pragma once
#include <functional>
#include <memory>
#include <string>
#include "../Core.h"
#include "../../Mystic/Layer.h"

namespace Mystic
{
	class Application;

	class EditorLayer : public Layer
	{
	public:
		virtual ~EditorLayer() = default;
		
		virtual void Init(int windowWidth, int windowHeight, std::string windowTitle, std::function<Application*()> applicationCreater) = 0;

		virtual void OnStart() = 0;
		virtual void OnEnd() = 0;
		
		virtual void PreRender() = 0;
		virtual void Render() = 0;
		virtual void PostRender() = 0;

		virtual void ShowGameWindow() = 0;
		virtual void BindFramebuffer() = 0;
		virtual void UnbindFramebuffer() = 0;

		virtual bool ShouldClose() = 0;
		virtual bool PollEvents() = 0;

		virtual void OnEvent(Event& e) override = 0;
	};
	
	class MYSTIC_API Editor
	{
	public:
		static void Init(int windowWidth, int windowHeight, std::string title, std::function<Application*()> applicationCreater);
		static void Start();
		static bool ShouldClose();
		static void Update();

		static void ShowGameWindow();
		static void BindFramebuffer();
		static void UnbindFrameBuffer();

	private:
		static void PreRender();
		static void Render();
		static void PostRender();

	private:
		static std::unique_ptr<EditorLayer> s_editor;
	};
}

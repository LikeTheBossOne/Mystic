//#pragma once
//#include "Application.h"
//
//struct ImGuiContext;
//
//namespace Mystic
//{
//	class ServiceLocator
//	{
//	public:
//		Application& GetApplication()
//		{
//			Log::Assert(!!_application, "Application is null");
//			return *_application;
//		}
//
//#ifndef NO_IMGUI
//		ImGuiContext& GetImGuiContext()
//		{
//			Log::Assert(!!_imguiContext, "ImGuiContext is null");
//			return *_imguiContext;
//		}
//
//		void SetImGuiContext(ImGuiContext* imguiContext)
//		{
//			Log::Assert(!_imguiContext, "An ImGuiContext already exists in this runtime");
//			_imguiContext = imguiContext;
//		}
//#endif
//
//	private:
//		void SetApplication(Application* application)
//		{
//			Log::Assert(!_application, "An application already exists in this runtime");
//			_application = application;
//		}
//
//	private:
//		Application* _application = nullptr;
//
//#ifndef NO_IMGUI
//		ImGuiContext* _imguiContext = nullptr;
//#endif
//
//		friend Application;
//	};
//}

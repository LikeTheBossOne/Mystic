#include "EditorLayer.h"
#include "imgui.h"
#include "singleton.h"
#include "Mystic/Core/Application.h"

#include "Mystic/Core/EntryPoint.h"

namespace Mystic
{
	class MysticEditorApp : public Application
	{
	public:
		MysticEditorApp(ApplicationCommandLineArgs args) : Application("Mystic Editor", args)
		{
			PushLayer(new EditorLayer());
		}

		~MysticEditorApp()
		{
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new MysticEditorApp(args);
	}
}

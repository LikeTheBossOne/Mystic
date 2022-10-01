#include "GameCodeSystem.h"

#include <complex.h>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace Mystic
{
	static void DeleteComponentStub(entt::registry&) {}
	static void InitComponentsStub(entt::registry&) {}
	static void InitImGuiStub(ImGuiContext*) {}
	//static void SaveScriptStub(json&) {}
	//static void LoadScriptStub(json&, Entity) {}
	static void ImGuiStub(entt::registry&,entt::entity) {}
	static void UpdateComponentsStub(entt::registry&, float) {}
	//static void EditorUpdateScriptStub(float, Scene*) { Log::Info("STUB UPDATE;"); }
	static void AddComponentFromStringStub(entt::registry&, std::string, entt::entity, Scene*) {}

	static FARPROC __stdcall TryLoadFunction(HMODULE module, const char* functionName)
	{
		auto func = GetProcAddress(module, functionName);
		if (func == NULL)
		{
			//Log::Warning("Could not load dll function '%s'", functionName);
			std::cout << "Could not load dll function: \'" << functionName << '\'' << std::endl;
		}

		return func;
	}
	

	void GameCodeSystem::Start(entt::registry& registry)
	{
		Reload(registry);
	}

	static bool FileExists(const std::string& name) {
		std::ifstream f(name.c_str());
		return f.good();
	}

	void GameCodeSystem::Reload(entt::registry& registry)
	{
		if (s_isLoaded)
		{
			if (!FreeGameCodeLibrary(registry)) return;
		}
		
		WCHAR pBuf[256];
		size_t len = sizeof(pBuf);
		int bytes = GetModuleFileName(NULL, pBuf, len);
		std::wstring ws(pBuf);
		std::string path(ws.begin(), ws.end());
		path = path.substr(0, path.find_last_of("\\/")) + "\\GameCode.dll";
		if (!FileExists(path))
		{
			std::cout << "Reloading GameCode -- Could not find DLL at path: " << path;
			return;
		}

		s_module = LoadLibraryA(path.c_str());

		if (!s_module)
		{
			std::cout << "Failed to load module: " << path;
			return;
		}

		s_initImGuiFn = (InitImGuiFn)TryLoadFunction(s_module, "InitImGui");
		s_imGuiFn = (ImGuiFn)TryLoadFunction(s_module, "ImGui");

		s_initComponentsFn = (InitComponentsFn)TryLoadFunction(s_module, "InitComponents");
		s_updateComponentsFn = (UpdateComponentsFn)TryLoadFunction(s_module, "UpdateComponents");
		s_addComponentFromStringFn = (AddComponentFromStringFn)TryLoadFunction(s_module, "AddComponent");
		s_deleteComponentFn = (DeleteComponentFn)TryLoadFunction(s_module, "DeleteComponent");

		if (s_initComponentsFn)
		{
			s_initComponentsFn(registry);
		}
	}

	void GameCodeSystem::Update(float dt, Scene* scene)
	{
		if (s_updateComponentsFn)
		{
			s_updateComponentsFn(scene->_registry, dt);
		}
	}

	void GameCodeSystem::ImGui(entt::registry& registry, entt::entity entity)
	{
		if (s_imGuiFn)
		{
			s_imGuiFn(registry, entity);
		}
	}

	void GameCodeSystem::AddComponentFromString(std::string className, entt::entity entity, entt::registry& registry, Scene* scene)
	{
		if (s_addComponentFromStringFn)
		{
			s_addComponentFromStringFn(registry, className, entity, scene);
		}
	}

	bool GameCodeSystem::FreeGameCodeLibrary(entt::registry& registry)
	{
		if (s_deleteComponentFn)
		{
			s_deleteComponentFn(registry);
		}
		
		s_deleteComponentFn = DeleteComponentStub;
		s_updateComponentsFn = UpdateComponentsStub;
		s_addComponentFromStringFn = AddComponentFromStringStub;
		s_initComponentsFn = InitComponentsStub;
		s_initImGuiFn = InitImGuiStub;
		s_imGuiFn = ImGuiStub;

		if (!FreeLibrary(s_module))
		{
			DWORD errorCode = GetLastError();
			std::cout << "Could not free script dll. Error Code: " << errorCode;
			return false;
		}

		s_module = nullptr;
		s_isLoaded = false;
		return true;
	}
}

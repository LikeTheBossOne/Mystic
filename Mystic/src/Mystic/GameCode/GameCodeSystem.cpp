#include "GameCodeSystem.h"

#include <complex.h>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "Mystic/Logging/Log.h"

namespace Mystic
{
	static void DeleteComponentStub(entt::registry&) {}
	static void InitComponentsStub(Scene*) {}
	static void InitImGuiStub(ImGuiContext*) {}
	static void SerializeEntityStub(entt::registry&, YAML::Emitter&, entt::entity) {}
	static void DeserializeEntityStub(entt::registry&, YAML::detail::iterator_value&, entt::entity, Scene*) {}
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
			Log::MError("Could not load dll gunction: \'%s\'", functionName);
		}

		return func;
	}
	

	void GameCodeSystem::Start(Scene* scene)
	{
		if (!s_isLoaded)
			Reload(scene);
	}

	static bool FileExists(const std::string& name) {
		std::ifstream f(name.c_str());
		return f.good();
	}

	void GameCodeSystem::Reload(Scene* scene)
	{
		if (s_isLoaded)
		{
			if (!FreeGameCodeLibrary(scene->_registry)) return;
		}
		
		WCHAR pBuf[256];
		DWORD len = sizeof(pBuf);
		int bytes = GetModuleFileName(NULL, pBuf, len);
		std::wstring ws(pBuf);
		std::string path(ws.begin(), ws.end());

		path = path.substr(0, path.find_last_of("\\/")) + "\\GameCode.dll";
		if (!FileExists(path))
		{
			Log::MError("Reloading GameCode -- Could not find DLL at path: %s", path.c_str());
			return;
		}

		s_module = LoadLibraryA(path.c_str());

		if (!s_module)
		{
			Log::MError("Failed to load module: %s", path.c_str());
			return;
		}

		s_initImGuiFn = (InitImGuiFn)TryLoadFunction(s_module, "InitImGui");
		s_imGuiFn = (ImGuiFn)TryLoadFunction(s_module, "ImGui");

		s_initComponentsFn = (InitComponentsFn)TryLoadFunction(s_module, "InitComponents");
		s_updateComponentsFn = (UpdateComponentsFn)TryLoadFunction(s_module, "UpdateComponents");
		s_addComponentFromStringFn = (AddComponentFromStringFn)TryLoadFunction(s_module, "AddComponent");
		s_deleteComponentsFn = (DeleteComponentsFn)TryLoadFunction(s_module, "DeleteComponents");
		s_serializeEntityFn = (SerializeEntityFn)TryLoadFunction(s_module, "SerializeEntity");
		s_deserializeEntityFn = (DeserializeEntityFn)TryLoadFunction(s_module, "DeserializeEntity");

		if (s_initComponentsFn)
		{
			s_initComponentsFn(scene);
		}
	}

	void GameCodeSystem::Update(float dt, Scene* scene)
	{
		if (s_updateComponentsFn)
		{
			s_updateComponentsFn(scene->_registry, dt);
		}
	}

	void GameCodeSystem::SerializeEntity(entt::registry& registryRef, YAML::Emitter& outEmitter, entt::entity entity)
	{
		if (s_serializeEntityFn)
		{
			s_serializeEntityFn(registryRef, outEmitter, entity);
		}
	}

	void GameCodeSystem::DeserializeEntity(entt::registry& registryRef, YAML::detail::iterator_value& entityNode,
	                                       entt::entity entity, Scene* scene)
	{
		if (s_deserializeEntityFn)
		{
			s_deserializeEntityFn(registryRef, entityNode, entity, scene);
		}
	}

	void GameCodeSystem::InitImGui(ImGuiContext* context)
	{
		if (s_initImGuiFn)
		{
			s_initImGuiFn(context);
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
		if (s_deleteComponentsFn)
		{
			s_deleteComponentsFn(registry);
		}
		
		s_deleteComponentsFn = DeleteComponentStub;
		s_updateComponentsFn = UpdateComponentsStub;
		s_addComponentFromStringFn = AddComponentFromStringStub;
		s_initComponentsFn = InitComponentsStub;
		s_serializeEntityFn = SerializeEntityStub;
		s_deserializeEntityFn = DeserializeEntityStub;
		s_initImGuiFn = InitImGuiStub;
		s_imGuiFn = ImGuiStub;

		// POOLS BREAK HERE. All memory from DLLs are invalidated when they are freed.S
		if (!FreeLibrary(s_module))
		{
			DWORD errorCode = GetLastError();
			Log::MError("Could not free script dll. Error Code: %lu", errorCode);
			return false;
		}

		s_module = nullptr;
		s_isLoaded = false;
		return true;
	}
}

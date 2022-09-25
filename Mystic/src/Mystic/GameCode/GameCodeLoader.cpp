#include "GameCodeLoader.h"

#include <complex.h>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace Mystic
{
	static void DeleteComponentStub() {}
	static void InitComponentsStub() {}
	static void InitImGuiStub(ImGuiContext*) {}
	//static void SaveScriptStub(json&) {}
	//static void LoadScriptStub(json&, Entity) {}
	static void ImGuiStub(entt::entity) {}
	static void UpdateComponentsStub(float, Scene*) {}
	//static void EditorUpdateScriptStub(float, Scene*) { Log::Info("STUB UPDATE;"); }
	static void AddComponentFromStringStub(std::string, entt::entity, entt::registry&) {}

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

	GameCodeLoader::~GameCodeLoader()
	{
		FreeGameCodeLibrary();
	}

	void GameCodeLoader::Start()
	{
		Reload();
	}

	static bool FileExists(const std::string& name) {
		std::ifstream f(name.c_str());
		return f.good();
	}

	void GameCodeLoader::Reload()
	{
		if (_isLoaded)
		{
			if (!FreeGameCodeLibrary()) return;
		}

		std::string pathToDLL = std::filesystem::current_path().string() + "GameCode.dll";
		if (!FileExists(pathToDLL))
		{
			std::cout << "Reloading GameCode -- Could not find DLL at path: " << pathToDLL;
			return;
		}

		_module = LoadLibraryA(pathToDLL.c_str());

		if (!_module)
		{
			std::cout << "Failed to load module: " << pathToDLL;
			return;
		}

		_initImGuiFn = (InitImGuiFn)TryLoadFunction(_module, "InitImGui");
		_imGuiFn = (ImGuiFn)TryLoadFunction(_module, "ImGui");

		_initComponentsFn = (InitComponentsFn)TryLoadFunction(_module, "InitComponents");
		_updateComponentsFn = (UpdateComponentsFn)TryLoadFunction(_module, "UpdateComponents");
		_addComponentFromStringFn = (AddComponentFromStringFn)TryLoadFunction(_module, "AddComponent");
		_deleteComponentFn = (DeleteComponentFn)TryLoadFunction(_module, "DeleteComponent");

		if (_initComponentsFn)
		{
			_initComponentsFn();
		}
	}

	bool GameCodeLoader::FreeGameCodeLibrary()
	{
		if (_deleteComponentFn)
		{
			_deleteComponentFn();
		}
		
		_deleteComponentFn = DeleteComponentStub;
		_updateComponentsFn = UpdateComponentsStub;
		_addComponentFromStringFn = AddComponentFromStringStub;
		_initComponentsFn = InitComponentsStub;
		_initImGuiFn = InitImGuiStub;
		_imGuiFn = ImGuiStub;

		if (!FreeLibrary(_module))
		{
			DWORD errorCode = GetLastError();
			std::cout << "Could not free script dll. Error Code: " << errorCode;
			return false;
		}

		_module = nullptr;
		_isLoaded = false;
		return true;
	}
}

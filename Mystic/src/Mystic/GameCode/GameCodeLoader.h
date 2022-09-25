#pragma once
#include "imgui_internal.h"
#include "entt.hpp"
#include "Mystic/Scene/Scene.h"

namespace Mystic
{
    typedef void (*DeleteComponentFn)();
    typedef void (*InitComponentsFn)();
    typedef void (*InitImGuiFn)(ImGuiContext*);
    //typedef void (*SaveScriptFn)(json&);
    //typedef void (*LoadScriptFn)(json&, Entity);
    typedef void (*ImGuiFn)(entt::entity);
    typedef void (*UpdateComponentsFn)(float, Scene*);
    //typedef void (*EditorUpdateScriptFn)(float, Scene*);
    typedef void (*AddComponentFromStringFn)(std::string, entt::entity, entt::registry&);

	class GameCodeLoader
	{
	public:
        ~GameCodeLoader();

        void Start();
        void Reload();

	private:
        bool FreeGameCodeLibrary();


        InitImGuiFn _initImGuiFn = nullptr;
        ImGuiFn _imGuiFn = nullptr;

        InitComponentsFn _initComponentsFn = nullptr;
        UpdateComponentsFn _updateComponentsFn = nullptr;
        AddComponentFromStringFn _addComponentFromStringFn = nullptr;
        DeleteComponentFn _deleteComponentFn = nullptr;

        bool _isLoaded = false;
        HMODULE _module;
	};
}

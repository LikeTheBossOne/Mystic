#pragma once
#include "imgui_internal.h"
#include "entt.hpp"
#include "Mystic/Scene/Scene.h"

namespace Mystic
{
    typedef void (*DeleteComponentFn)(entt::registry&);
    typedef void (*InitComponentsFn)(entt::registry&);
    typedef void (*InitImGuiFn)(ImGuiContext*);
    //typedef void (*SaveScriptFn)(json&);
    //typedef void (*LoadScriptFn)(json&, Entity);
    typedef void (*ImGuiFn)(entt::registry&, entt::entity);
    typedef void (*UpdateComponentsFn)(entt::registry& ,float);
    //typedef void (*EditorUpdateScriptFn)(float, Scene*);
    typedef void (*AddComponentFromStringFn)(entt::registry&, std::string, entt::entity, Scene*);

	class MYSTIC_API GameCodeSystem
	{
	public:
        static void Start(entt::registry& registry);
        static void Reload(entt::registry& registry);
        
		static void Update(float dt, Scene* scene);
        static void ImGui(entt::registry& registry, entt::entity entity);
        
        static void AddComponentFromString(std::string className, entt::entity entity, entt::registry& registry, Scene* scene);

	private:
        static bool FreeGameCodeLibrary(entt::registry& registry);
         

        inline static InitImGuiFn s_initImGuiFn = nullptr;
        inline static ImGuiFn s_imGuiFn = nullptr;

        inline static InitComponentsFn s_initComponentsFn = nullptr;
        inline static UpdateComponentsFn s_updateComponentsFn = nullptr;
        inline static AddComponentFromStringFn s_addComponentFromStringFn = nullptr;
        inline static DeleteComponentFn s_deleteComponentFn = nullptr;

        inline static bool s_isLoaded = false;
        inline static HMODULE s_module;
	};
}

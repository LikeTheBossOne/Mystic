#pragma once
#include <entt.hpp>
#include "LaserComponent.Gen.h"
#include "Mystic/Scene/Scene.h"

extern "C" namespace Mystic
{
	extern "C" namespace Init
	{
		extern "C" __declspec(dllexport) void AddComponent(entt::registry& registryRef, std::string className, entt::entity entity, Scene* scene)
		{
			assert(registryRef.valid(entity), "attempted to add a component to an entity that did not exist");

			for (auto strClass : ReflectLaserComponent::stringToMap)
			{
				if (strClass.first == className)
				{
					ReflectLaserComponent::AddComponent(className, entity, scene, registryRef);
					return;
				}
			}
		}

		extern "C" __declspec(dllexport) void UpdateComponents(entt::registry& registryRef, float dt)
		{
			{
				auto view = registryRef.view<LaserComponent>();
				for (auto [entity, comp] : view.each())
				{
					comp.Update();
				}
			}
		}

		extern "C" __declspec(dllexport) void InitComponents(entt::registry& registryRef)
		{
			//log
			ReflectLaserComponent::Init(registryRef);
		}

		extern "C" __declspec(dllexport) void InitImGui(ImGuiContext* ctx)
		{
			ImGui::SetCurrentContext(ctx);
		}

		extern "C" __declspec(dllexport) void ImGui(entt::registry& registryRef, entt::entity entity)
		{
			ReflectLaserComponent::ImGui(entity, registryRef);
		}

		extern "C" __declspec(dllexport) void DeleteComponents(entt::registry& registryRef)
		{
			ReflectLaserComponent::DeleteComponents(registryRef);
		}

	}
}

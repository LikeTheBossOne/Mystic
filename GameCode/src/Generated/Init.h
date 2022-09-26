#pragma once
#include <entt.hpp>
#include "LaserComponent.Gen.h"
#include "Mystic/Scene/Scene.h"

extern "C" namespace Mystic
{
	extern "C" namespace Init
	{
		inline entt::registry registry = entt::registry();

		extern "C" __declspec(dllexport) void AddComponent(std::string className, entt::entity entity)
		{
			if (!registry.valid(entity))
			{
				registry.create(entity);
			}

			for (auto strClass : ReflectLaserComponent::stringToMap)
			{
				if (strClass.first == className)
				{
					ReflectLaserComponent::AddComponent(className, entity, registry);
					return;
				}
			}
		}

		extern "C" __declspec(dllexport) void UpdateComponents(float dt, Scene* scene)
		{
			{
				auto view = registry.view<LaserComponent>();
				for (auto [entity, comp] : view.each())
				{
					//comp.Update()
				}
			}
		}

		extern "C" __declspec(dllexport) void InitComponents()
		{
			//log
			ReflectLaserComponent::Init();
		}

		extern "C" __declspec(dllexport) void InitImGui(ImGuiContext* ctx)
		{
			ImGui::SetCurrentContext(ctx);
		}

		extern "C" __declspec(dllexport) void ImGui(entt::entity entity)
		{
			ReflectLaserComponent::ImGui(entity, registry);
		}

		extern "C" __declspec(dllexport) void DeleteComponents()
		{
			ReflectLaserComponent::DeleteComponents(registry);
		}

	}
}

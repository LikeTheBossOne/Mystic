#pragma once

#define ENTT_STANDARD_CPP
#include "entt.hpp"
#include "imgui/imgui.h"

#include "../Components/LaserComponent.h"
#include "Mystic/ECS/Entity.h"

namespace Mystic
{
	namespace ReflectLaserComponent
	{
		bool initialized = false;

		std::vector<entt::id_type> ids =
		{
			entt::hashed_string{"LaserComponent::Damage"},
		};

		std::map<entt::id_type, const char*> debugNames =
		{
			{ entt::type_id<LaserComponent>().hash(), "LaserComponent"},
			{ ids[0], "Damage" }
		};

		std::map<std::string, entt::id_type> stringToMap =
		{
			{ "LaserComponent", entt::type_id<LaserComponent>().hash()}
		};

		void Init(entt::registry& registryRef)
		{
			if (initialized) return;
			initialized = true;

			auto laserComponentFactory = entt::meta<LaserComponent>()
				.data<&LaserComponent::Damage, entt::as_ref_t>(ids[0])
				.type();
		}

		void AddComponent(std::string className, entt::entity entity, Scene* scene, entt::registry& registryRef)
		{
			if (className == "LaserComponent")
			{
				registryRef.emplace<LaserComponent>(entity, entity, scene);
			}
		}

		void ImGuiAny(entt::meta_any any, entt::meta_handle handle)
		{
			auto typeData = entt::resolve(any.type().id());
			auto typeName = debugNames.find(any.type().id())->second;

			for (auto data : typeData.data())
			{
				auto name = debugNames.find(data.id());
				auto type = debugNames.find(data.type().id());
				if (name == debugNames.end() && type == debugNames.end())
					continue;

				if (data.type().is_arithmetic())
				{
					if (data.type().info().name() == "float")
					{
						float& val = any.cast<float&>();
						ImGui::DragFloat(name->second, &val);
					}
				}
			}
		}

		void ImGui(entt::entity entity, entt::registry& registryRef)
		{
			if (!registryRef.valid(entity))
				return;
			if (registryRef.all_of<LaserComponent>(entity))
			{
				LaserComponent& comp = registryRef.get<LaserComponent>(entity);
				ImGuiAny({ comp }, comp);
			}
		}

		void DeleteComponents(entt::registry& registryRef)
		{
			{
				registryRef.clear<LaserComponent>();
			}
		}
	}
}

#pragma once
#include <entt.hpp>
#include <yaml-cpp/node/iterator.h>
#include "Mystic/Scene/Scene.h"
#include "Mystic/Logging/Log.h"
#include "yaml-cpp/emitter.h"
#include "ReflectAsteroidComponent.generated.h"
#include "ReflectLaserComponent.generated.h"

extern "C" namespace Mystic
{
    extern "C" namespace Init
    {

        extern "C" __declspec(dllexport) void AddComponent(entt::registry& registryRef, std::string className, entt::entity entity, Scene* scene)
        {
            Log::Assert(registryRef.valid(entity), "attempted to add a component to an entity %lu that did not exist", entity);

            for (auto strClass : ReflectAsteroidComponent::stringToMap)
            {
                if (strClass.first == className)
                {
                    ReflectAsteroidComponent::AddComponent(className, entity, scene, registryRef);
                    return;
                }
            }

            for (auto strClass : ReflectLaserComponent::stringToMap)
            {
                if (strClass.first == className)
                {
                    ReflectLaserComponent::AddComponent(className, entity, scene, registryRef);
                    return;
                }
            }
        }

        extern "C" __declspec(dllexport) void DeleteComponents(entt::registry& registryRef)
        {
            ReflectAsteroidComponent::DeleteComponents(registryRef);
            ReflectLaserComponent::DeleteComponents(registryRef);
        }

        extern "C" __declspec(dllexport) void InitComponents(Scene* scene)
        {
            scene->RegisterComponentType<AsteroidComponent>();
            scene->RegisterComponentType<LaserComponent>();
            ReflectAsteroidComponent::Init();
            ReflectLaserComponent::Init();
        }

        extern "C" __declspec(dllexport) void UpdateComponents(entt::registry& registryRef, float dt)
        {

            {
                auto view = registryRef.view<AsteroidComponent>();
                for (auto [entity, comp] : view.each())
                {
                    comp.Update(dt);
                }
            }

            {
                auto view = registryRef.view<LaserComponent>();
                for (auto [entity, comp] : view.each())
                {
                    comp.Update(dt);
                }
            }
        }

        extern "C" __declspec(dllexport) void SerializeEntity(entt::registry& registryRef, YAML::Emitter& out, entt::entity entity)
        {
            if (registryRef.any_of<AsteroidComponent, LaserComponent>(entity))
            {
                ReflectAsteroidComponent::SerializeEntity(registryRef, out, entity);
                ReflectLaserComponent::SerializeEntity(registryRef, out, entity);

            }
        }

        extern "C" __declspec(dllexport) void DeserializeEntity(entt::registry& registryRef, YAML::detail::iterator_value& entityNode, entt::entity entity, Scene* scene)
        {
            ReflectAsteroidComponent::DeserializeEntity(registryRef, entityNode, entity, scene);
            ReflectLaserComponent::DeserializeEntity(registryRef, entityNode, entity, scene);
        }

        extern "C" __declspec(dllexport) void InitImGui(ImGuiContext* ctx)
        {
            ImGui::SetCurrentContext(ctx);
        }

        extern "C" __declspec(dllexport) void ImGui(entt::registry & registryRef, entt::entity entity)
        {
            ReflectAsteroidComponent::ImGui(entity, registryRef);
            ReflectLaserComponent::ImGui(entity, registryRef);
        }

    }
}

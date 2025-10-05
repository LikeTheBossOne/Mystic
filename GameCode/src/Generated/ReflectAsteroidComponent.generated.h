
#pragma once

#define ENTT_STANDARD_CPP
#include "entt.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "Components/AsteroidComponent.h"
#include "yaml-cpp/node/iterator.h"
#include "yaml-cpp/node/convert.h"

namespace Mystic::ReflectAsteroidComponent
{

    inline bool initialized = false;

    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    std::vector<entt::id_type> ids =
	{

        entt::hashed_string{"AsteroidComponent::SplitsLeft"},

    };

    std::map<entt::id_type, const char*> debugNames =
	{

        { entt::type_id<AsteroidComponent>().hash(), "AsteroidComponent"},
        { ids[0], "SplitsLeft" },

    };

    std::map<std::string, entt::id_type> stringToMap =
    {
        { "AsteroidComponent", entt::type_id<AsteroidComponent>().hash()}
    };


    inline void Init()
    {
        if (initialized) return;
        initialized = true;

        auto factory = entt::meta<AsteroidComponent>()

            .data<&AsteroidComponent::SplitsLeft, entt::as_ref_t>(ids[0])

            .type();
    }


    inline void AddComponent(std::string className, entt::entity entity, Scene* scene, entt::registry& registryRef)
    {
        if (className == "AsteroidComponent")
        {
            registryRef.emplace<AsteroidComponent>(entity, entity, scene);
        }
    }


    inline void DeleteComponents(entt::registry& registryRef)
    {
        
    }


    inline void SerializeEntity(entt::registry& registryRef, YAML::Emitter& out, entt::entity entity)
    {
        AsteroidComponent* component = registryRef.try_get<AsteroidComponent>(entity);
        if (component)
        {
            out << YAML::Key << "AsteroidComponent" << YAML::Value << YAML::BeginMap;

            out << YAML::Key << "SplitsLeft" << YAML::Value << component->SplitsLeft;

            out << YAML::EndMap;
        }
    }


    inline void DeserializeEntity(entt::registry& registryRef, YAML::detail::iterator_value& entityNode, entt::entity entity, Scene* scene)
    {
        auto componentNode = entityNode["AsteroidComponent"];
        if (componentNode)
        {
            AsteroidComponent component(entity, scene);


            if (componentNode["SplitsLeft"])
            {
                component.SplitsLeft = componentNode["SplitsLeft"].as<uint8_t>();
            }

            registryRef.emplace<AsteroidComponent>(entity, component);
        }
    }


    inline void ImGuiAny(entt::meta_any any, AsteroidComponent& handle)
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
                    float* val = (float*)data.get(handle).data();
                    ImGui::DragFloat(name->second, val);
                }

                if (data.type().info().name() == "int")
                {
                    int* val = (int*)data.get(handle).data();
                    ImGui::DragInt(name->second, val);
                }
            }
        }
    }

    inline void ImGui(entt::entity entity, entt::registry& registryRef)
    {
        if (!registryRef.valid(entity))
            return;
        if (registryRef.all_of<AsteroidComponent>(entity))
        {
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(AsteroidComponent).hash_code(), treeNodeFlags, "Asteroid Component");
            ImGui::PopStyleVar();

            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
            {
                ImGui::OpenPopup("ComponentSettings");
            }

            bool shouldRemoveComponent = false;
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove component"))
                    shouldRemoveComponent = true;

                ImGui::EndPopup();
            }

            if (open)
            {
                AsteroidComponent& comp = registryRef.get<AsteroidComponent>(entity);
                ImGuiAny({ comp }, comp);
                ImGui::TreePop();
            }

            if (shouldRemoveComponent)
                registryRef.remove<AsteroidComponent>(entity);
        }
    }
}

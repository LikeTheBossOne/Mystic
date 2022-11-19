
#pragma once

#define ENTT_STANDARD_CPP
#include "entt.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "Components/LaserComponent.h"
#include "yaml-cpp/node/iterator.h"

namespace Mystic::ReflectLaserComponent
{

    inline bool initialized = false;

    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    std::vector<entt::id_type> ids =
	{

        entt::hashed_string{"LaserComponent::Damage"},
        entt::hashed_string{"LaserComponent::Range"},
        entt::hashed_string{"LaserComponent::TestInt"},

    };

    std::map<entt::id_type, const char*> debugNames =
	{

        { entt::type_id<LaserComponent>().hash(), "LaserComponent"},
        { ids[0], "Damage" },
        { ids[1], "Range" },
        { ids[2], "TestInt" },

    };

    std::map<std::string, entt::id_type> stringToMap =
    {
        { "LaserComponent", entt::type_id<LaserComponent>().hash()}
    };


    inline void Init()
    {
        if (initialized) return;
        initialized = true;

        auto factory = entt::meta<LaserComponent>()

            .data<&LaserComponent::Damage, entt::as_ref_t>(ids[0])
            .data<&LaserComponent::Range, entt::as_ref_t>(ids[1])
            .data<&LaserComponent::TestInt, entt::as_ref_t>(ids[2])

            .type();
    }


    inline void AddComponent(std::string className, entt::entity entity, Scene* scene, entt::registry& registryRef)
    {
        if (className == "LaserComponent")
        {
            registryRef.emplace<LaserComponent>(entity, entity, scene);
        }
    }


    inline void DeleteComponents(entt::registry& registryRef)
    {
        
    }


    inline void SerializeEntity(entt::registry& registryRef, YAML::Emitter& out, entt::entity entity)
    {
        LaserComponent* component = registryRef.try_get<LaserComponent>(entity);
        if (component)
        {
            out << YAML::Key << "LaserComponent" << YAML::Value << YAML::BeginMap;

            out << YAML::Key << "Damage" << YAML::Value << component->Damage;
            out << YAML::Key << "Range" << YAML::Value << component->Range;
            out << YAML::Key << "TestInt" << YAML::Value << component->TestInt;

            out << YAML::EndMap;
        }
    }


    inline void DeserializeEntity(entt::registry& registryRef, YAML::detail::iterator_value& entityNode, entt::entity entity, Scene* scene)
    {
        auto componentNode = entityNode["LaserComponent"];
        if (componentNode)
        {
            LaserComponent component(entity, scene);


            if (componentNode["Damage"])
            {
                component.Damage = componentNode["Damage"].as<float>();
            }

            if (componentNode["Range"])
            {
                component.Range = componentNode["Range"].as<float>();
            }

            if (componentNode["TestInt"])
            {
                component.TestInt = componentNode["TestInt"].as<int32_t>();
            }

            registryRef.emplace<LaserComponent>(entity, component);
        }
    }


    inline void ImGuiAny(entt::meta_any any, LaserComponent& handle)
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
        if (registryRef.all_of<LaserComponent>(entity))
        {
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(LaserComponent).hash_code(), treeNodeFlags, "Laser Component");
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
                LaserComponent& comp = registryRef.get<LaserComponent>(entity);
                ImGuiAny({ comp }, comp);
                ImGui::TreePop();
            }

            if (shouldRemoveComponent)
                registryRef.remove<LaserComponent>(entity);
        }
    }
}


#pragma once

#define ENTT_STANDARD_CPP
#include "entt.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "Components/DefaultComponent.h"
#include "yaml-cpp/node/iterator.h"

namespace Mystic::ReflectDefaultComponent
{

    inline bool initialized = false;

    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    std::vector<entt::id_type> ids =
	{

        entt::hashed_string{"DefaultComponent::Char"},
        entt::hashed_string{"DefaultComponent::Short"},
        entt::hashed_string{"DefaultComponent::Int"},
        entt::hashed_string{"DefaultComponent::LongLong"},
        entt::hashed_string{"DefaultComponent::UChar"},
        entt::hashed_string{"DefaultComponent::UShort"},
        entt::hashed_string{"DefaultComponent::UInt"},
        entt::hashed_string{"DefaultComponent::ULongLong"},

    };

    std::map<entt::id_type, const char*> debugNames =
	{

        { entt::type_id<DefaultComponent>().hash(), "DefaultComponent"},
        { ids[0], "Char" },
        { ids[1], "Short" },
        { ids[2], "Int" },
        { ids[3], "LongLong" },
        { ids[4], "UChar" },
        { ids[5], "UShort" },
        { ids[6], "UInt" },
        { ids[7], "ULongLong" },

    };

    std::map<std::string, entt::id_type> stringToMap =
    {
        { "DefaultComponent", entt::type_id<DefaultComponent>().hash()}
    };


    inline void Init()
    {
        if (initialized) return;
        initialized = true;

        auto factory = entt::meta<DefaultComponent>()

            .data<&DefaultComponent::Char, entt::as_ref_t>(ids[0])
            .data<&DefaultComponent::Short, entt::as_ref_t>(ids[1])
            .data<&DefaultComponent::Int, entt::as_ref_t>(ids[2])
            .data<&DefaultComponent::LongLong, entt::as_ref_t>(ids[3])
            .data<&DefaultComponent::UChar, entt::as_ref_t>(ids[4])
            .data<&DefaultComponent::UShort, entt::as_ref_t>(ids[5])
            .data<&DefaultComponent::UInt, entt::as_ref_t>(ids[6])
            .data<&DefaultComponent::ULongLong, entt::as_ref_t>(ids[7])

            .type();
    }


    inline void AddComponent(std::string className, entt::entity entity, Scene* scene, entt::registry& registryRef)
    {
        if (className == "DefaultComponent")
        {
            registryRef.emplace<DefaultComponent>(entity, entity, scene);
        }
    }


    inline void DeleteComponents(entt::registry& registryRef)
    {
        
    }


    inline void SerializeEntity(entt::registry& registryRef, YAML::Emitter& out, entt::entity entity)
    {
        DefaultComponent* component = registryRef.try_get<DefaultComponent>(entity);
        if (component)
        {
            out << YAML::Key << "DefaultComponent" << YAML::Value << YAML::BeginMap;

            out << YAML::Key << "Char" << YAML::Value << component->Char;
            out << YAML::Key << "Short" << YAML::Value << component->Short;
            out << YAML::Key << "Int" << YAML::Value << component->Int;
            out << YAML::Key << "LongLong" << YAML::Value << component->LongLong;
            out << YAML::Key << "UChar" << YAML::Value << component->UChar;
            out << YAML::Key << "UShort" << YAML::Value << component->UShort;
            out << YAML::Key << "UInt" << YAML::Value << component->UInt;
            out << YAML::Key << "ULongLong" << YAML::Value << component->ULongLong;

            out << YAML::EndMap;
        }
    }


    inline void DeserializeEntity(entt::registry& registryRef, YAML::detail::iterator_value& entityNode, entt::entity entity, Scene* scene)
    {
        auto componentNode = entityNode["DefaultComponent"];
        if (componentNode)
        {
            DefaultComponent component(entity, scene);


            if (componentNode["Char"])
            {
                component.Char = componentNode["Char"].as<int8_t>();
            }

            if (componentNode["Short"])
            {
                component.Short = componentNode["Short"].as<int16_t>();
            }

            if (componentNode["Int"])
            {
                component.Int = componentNode["Int"].as<int32_t>();
            }

            if (componentNode["LongLong"])
            {
                component.LongLong = componentNode["LongLong"].as<int64_t>();
            }

            if (componentNode["UChar"])
            {
                component.UChar = componentNode["UChar"].as<uint8_t>();
            }

            if (componentNode["UShort"])
            {
                component.UShort = componentNode["UShort"].as<uint16_t>();
            }

            if (componentNode["UInt"])
            {
                component.UInt = componentNode["UInt"].as<uint32_t>();
            }

            if (componentNode["ULongLong"])
            {
                component.ULongLong = componentNode["ULongLong"].as<uint64_t>();
            }

            registryRef.emplace<DefaultComponent>(entity, component);
        }
    }


    inline void ImGuiAny(entt::meta_any any, DefaultComponent& handle)
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
        if (registryRef.all_of<DefaultComponent>(entity))
        {
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(DefaultComponent).hash_code(), treeNodeFlags, "Default Component");
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
                DefaultComponent& comp = registryRef.get<DefaultComponent>(entity);
                ImGuiAny({ comp }, comp);
                ImGui::TreePop();
            }

            if (shouldRemoveComponent)
                registryRef.remove<DefaultComponent>(entity);
        }
    }
}

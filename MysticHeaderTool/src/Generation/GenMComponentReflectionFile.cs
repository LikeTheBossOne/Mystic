using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MysticHeaderTool.Parsing;

namespace MysticHeaderTool.Generation
{

    public class GenMComponentReflectionFile : GeneratedFile
    {
        private MComponent ReflectedComponent { get; }

        public GenMComponentReflectionFile(string genPath, MComponent component) : base(genPath)
        {
            ReflectedComponent = component;
        }

        public override bool Generate()
        {
            try
            {
                Writer = new StreamWriter(GenPath, false);
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return false;
            }

            WriteIncludes();
            Writer.WriteLine($"namespace Mystic::Reflect{ReflectedComponent.Name}\n{{");
            WriteGlobals();

            WriteInit();

            WriteAddComponent();

            WriteDeleteComponents();

            WriteSerializeComponent();
            
            WriteDeserializeComponent();

            WriteImGuiFuncs();

            Writer.WriteLine("}");

            Writer.Close();

            return true;
        }

        private void WriteIncludes()
        {
            string includes = @$"
#pragma once

#define ENTT_STANDARD_CPP
#include ""entt.hpp""
#include ""imgui/imgui.h""
#include ""imgui/imgui_internal.h""

#include ""Components/{ReflectedComponent.Name}.h""
#include ""yaml-cpp/node/iterator.h""
";
            Writer.WriteLine(includes);
        }

        private void WriteGlobals()
        {
            StringBuilder globals = new StringBuilder();
            
            globals.AppendLine(@"
    inline bool initialized = false;

    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    std::vector<entt::id_type> ids =
	{"
            );

            globals.AppendLine();
            foreach (MProperty prop in ReflectedComponent.Properties)
            {
                globals.AppendLine($"        entt::hashed_string{{\"{ReflectedComponent.Name}::{prop.Name}\"}},");
            }
            globals.AppendLine(@"
    };

    std::map<entt::id_type, const char*> debugNames =
	{"
            );

            globals.AppendLine();
            globals.AppendLine(
                $"        {{ entt::type_id<{ReflectedComponent.Name}>().hash(), \"{ReflectedComponent.Name}\"}},");
            uint idx = 0;
            foreach (MProperty prop in ReflectedComponent.Properties)
            {
                globals.AppendLine($"        {{ ids[{idx}], \"{prop.Name}\" }},");
                idx++;
            }

            globals.AppendLine(@$"
    }};

    std::map<std::string, entt::id_type> stringToMap =
    {{
        {{ ""{ReflectedComponent.Name}"", entt::type_id<{ReflectedComponent.Name}>().hash()}}
    }};
"
            );

            Writer.Write(globals.ToString());
        }
        

        private void WriteInit()
        {
            StringBuilder init = new StringBuilder();
            init.AppendLine(@$"
    inline void Init()
    {{
        if (initialized) return;
        initialized = true;

        auto factory = entt::meta<{ReflectedComponent.Name}>()
"
            );

            uint idx = 0;
            foreach (MProperty prop in ReflectedComponent.Properties)
            {
                init.AppendLine(
                    $"            .data<&{ReflectedComponent.Name}::{prop.Name}, entt::as_ref_t>(ids[{idx}])");
                idx++;
            }

            init.AppendLine(@$"
            .type();
    }}
"
            );

            Writer.Write(init.ToString());
        }

        private void WriteAddComponent()
        {
            string add = $@"
    inline void AddComponent(std::string className, entt::entity entity, Scene* scene, entt::registry& registryRef)
    {{
        if (className == ""{ReflectedComponent.Name}"")
        {{
            registryRef.emplace<{ReflectedComponent.Name}>(entity, entity, scene);
        }}
    }}
";
            Writer.WriteLine(add);
        }

        private void WriteDeleteComponents()
        {
            string delete = $@"
    inline void DeleteComponents(entt::registry& registryRef)
    {{
        
    }}
";
            Writer.WriteLine(delete);
        }

        private void WriteSerializeComponent()
        {
            StringBuilder serialize = new StringBuilder();
            serialize.AppendLine($@"
    inline void SerializeEntity(entt::registry& registryRef, YAML::Emitter& out, entt::entity entity)
    {{
        {ReflectedComponent.Name}* component = registryRef.try_get<{ReflectedComponent.Name}>(entity);
        if (component)
        {{
            out << YAML::Key << ""{ReflectedComponent.Name}"" << YAML::Value << YAML::BeginMap;
"
            );

            foreach (MProperty prop in ReflectedComponent.Properties)
            {
                serialize.AppendLine($"            out << YAML::Key << \"{prop.Name}\" << YAML::Value << component->{prop.Name};");
            }

            serialize.AppendLine(@$"
            out << YAML::EndMap;
        }}
    }}
"
            );

            Writer.Write(serialize.ToString());
        }

        private void WriteDeserializeComponent()
        {
            StringBuilder deserialize = new StringBuilder();
            deserialize.AppendLine($@"
    inline void DeserializeEntity(entt::registry& registryRef, YAML::detail::iterator_value& entityNode, entt::entity entity, Scene* scene)
    {{
        auto componentNode = entityNode[""{ReflectedComponent.Name}""];
        if (componentNode)
        {{
            {ReflectedComponent.Name} component(entity, scene);
"
            );

            foreach (MProperty prop in ReflectedComponent.Properties)
            {
                deserialize.Append(@$"
            if (componentNode[""{prop.Name}""])
            {{
                component.{prop.Name} = componentNode[""{ prop.Name}""].as<{prop.Type.GetTypeDef()}>();
            }}
"
                );
            }
            
            deserialize.AppendLine(@$"
            registryRef.emplace<{ReflectedComponent.Name}>(entity, component);
        }}
    }}
"
            );

            Writer.Write(deserialize.ToString());
        }

        private void WriteImGuiFuncs()
        {
            string imguiAny = $@"
    inline void ImGuiAny(entt::meta_any any, {ReflectedComponent.Name}& handle)
    {{
        auto typeData = entt::resolve(any.type().id());
        auto typeName = debugNames.find(any.type().id())->second;

        for (auto data : typeData.data())
        {{
            auto name = debugNames.find(data.id());
            auto type = debugNames.find(data.type().id());
            if (name == debugNames.end() && type == debugNames.end())
                continue;

            if (data.type().is_arithmetic())
            {{
                if (data.type().info().name() == ""float"")
                {{
                    float* val = (float*)data.get(handle).data();
                    ImGui::DragFloat(name->second, val);
                }}

                if (data.type().info().name() == ""int"")
                {{
                    int* val = (int*)data.get(handle).data();
                    ImGui::DragInt(name->second, val);
                }}
            }}
        }}
    }}
";
            Writer.Write(imguiAny);

            string imgui = $@"
    inline void ImGui(entt::entity entity, entt::registry& registryRef)
    {{
        if (!registryRef.valid(entity))
            return;
        if (registryRef.all_of<{ReflectedComponent.Name}>(entity))
        {{
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{{ 4, 4 }});
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid({ReflectedComponent.Name}).hash_code(), treeNodeFlags, ""{ReflectedComponent.ImGuiName}"");
            ImGui::PopStyleVar();

            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button(""+"", ImVec2{{ lineHeight, lineHeight }}))
            {{
                ImGui::OpenPopup(""ComponentSettings"");
            }}

            bool shouldRemoveComponent = false;
            if (ImGui::BeginPopup(""ComponentSettings""))
            {{
                if (ImGui::MenuItem(""Remove component""))
                    shouldRemoveComponent = true;

                ImGui::EndPopup();
            }}

            if (open)
            {{
                {ReflectedComponent.Name}& comp = registryRef.get<{ReflectedComponent.Name}>(entity);
                ImGuiAny({{ comp }}, comp);
                ImGui::TreePop();
            }}

            if (shouldRemoveComponent)
                registryRef.remove<{ReflectedComponent.Name}>(entity);
        }}
    }}
";
            Writer.Write(imgui);
        }

    }
}

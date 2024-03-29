﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MysticHeaderTool.Parsing;

namespace MysticHeaderTool.Generation
{
    public class GenGameCodeExternFile : GeneratedFile
    {
        private List<KeyValuePair<string, MComponent>> GenFileNameToComponents { get; }

        public GenGameCodeExternFile(string genPath, List<KeyValuePair<string, MComponent>> genFileNameToComponents) : base(genPath)
        {
            GenFileNameToComponents = genFileNameToComponents;
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

            Writer.WriteLine("#pragma once");
            WriteIncludes();

            Writer.WriteLine(@"
extern ""C"" namespace Mystic
{
    extern ""C"" namespace Init
    {");

            WriteAddComponent();
            WriteDeleteComponents();

            WriteInitComponents();
            WriteUpdateComponents();

            WriteSerializeEntity();
            WriteDeserializeEntity();

            WriteInitImGui();
            WriteImGui();
            

            Writer.Write(@"
    }
}
");

            Writer.Close();

            return true;
        }

        private void WriteIncludes()
        {
            StringBuilder includes = new StringBuilder();
            includes.AppendLine("#include <entt.hpp>");
            includes.AppendLine(@"#include <yaml-cpp/node/iterator.h>");
            includes.AppendLine(@"#include ""Mystic/Scene/Scene.h""");
            includes.AppendLine(@"#include ""Mystic/Logging/Log.h""");
            includes.AppendLine(@"#include ""yaml-cpp/emitter.h""");
            foreach (var pair in GenFileNameToComponents)
            {
                includes.AppendLine($"#include \"{pair.Key}\"");
            }

            Writer.Write(includes.ToString());
        }


        private void WriteAddComponent()
        {
            StringBuilder addComponent = new StringBuilder();
            addComponent.Append(@"
        extern ""C"" __declspec(dllexport) void AddComponent(entt::registry& registryRef, std::string className, entt::entity entity, Scene* scene)
        {
            Log::Assert(registryRef.valid(entity), ""attempted to add a component to an entity %lu that did not exist"", entity);
");
            foreach (var pair in GenFileNameToComponents)
            {
                addComponent.Append($@"
            for (auto strClass : Reflect{pair.Value.Name}::stringToMap)
            {{
                if (strClass.first == className)
                {{
                    Reflect{pair.Value.Name}::AddComponent(className, entity, scene, registryRef);
                    return;
                }}
            }}
");
            }

            addComponent.Append("        }");
            
            Writer.WriteLine(addComponent.ToString());
        }

        private void WriteDeleteComponents()
        {
            StringBuilder deleteComponents = new StringBuilder();
            deleteComponents.Append(@"
        extern ""C"" __declspec(dllexport) void DeleteComponents(entt::registry& registryRef)
        {
");
            foreach (var pair in GenFileNameToComponents)
            {
                deleteComponents.AppendLine($"            Reflect{pair.Value.Name}::DeleteComponents(registryRef);");
            }

            deleteComponents.Append("        }");
            Writer.WriteLine(deleteComponents.ToString());
        }

        private void WriteInitComponents()
        {
            StringBuilder initComponents = new StringBuilder();
            initComponents.Append(@"
        extern ""C"" __declspec(dllexport) void InitComponents(Scene* scene)
        {
");

            foreach (var pair in GenFileNameToComponents)
            {
                initComponents.AppendLine($"            scene->RegisterComponentType<{pair.Value.Name}>();");
            }

            foreach (var pair in GenFileNameToComponents)
            {
                initComponents.AppendLine($"            Reflect{pair.Value.Name}::Init();");
            }

            initComponents.Append("        }");
            Writer.WriteLine(initComponents.ToString());
        }

        private void WriteUpdateComponents()
        {
            StringBuilder updateComponents = new StringBuilder();
            updateComponents.Append(@"
        extern ""C"" __declspec(dllexport) void UpdateComponents(entt::registry& registryRef, float dt)
        {
");
            foreach (var pair in GenFileNameToComponents)
            {
                updateComponents.Append($@"
            {{
                auto view = registryRef.view<{pair.Value.Name}>();
                for (auto [entity, comp] : view.each())
                {{
                    comp.Update();
                }}
            }}
");
            }

            updateComponents.Append("        }");
            Writer.WriteLine(updateComponents.ToString());
        }

        private void WriteSerializeEntity()
        {
            StringBuilder serializeEntity = new StringBuilder();
            serializeEntity.Append(@"
        extern ""C"" __declspec(dllexport) void SerializeEntity(entt::registry& registryRef, YAML::Emitter& out, entt::entity entity)
        {
            if (registryRef.any_of<");

            if (GenFileNameToComponents.Count > 0)
            {
                serializeEntity.Append(GenFileNameToComponents[0].Value.Name);
            }
            
            for (int i = 1; i < GenFileNameToComponents.Count; i++)
            {
                serializeEntity.Append($", {GenFileNameToComponents[i].Value.Name}");
            }

            serializeEntity.Append(@">(entity))
            {
");
            foreach (var pair in GenFileNameToComponents)
            {
                serializeEntity.AppendLine($"                Reflect{pair.Value.Name}::SerializeEntity(registryRef, out, entity);");
            }

            serializeEntity.Append(@"
            }
        }");
            Writer.WriteLine(serializeEntity.ToString());
        }

        private void WriteDeserializeEntity()
        {
            StringBuilder deserializeEntity = new StringBuilder();
            deserializeEntity.Append(@"
        extern ""C"" __declspec(dllexport) void DeserializeEntity(entt::registry& registryRef, YAML::detail::iterator_value& entityNode, entt::entity entity, Scene* scene)
        {
");
            foreach (var pair in GenFileNameToComponents)
            {
                deserializeEntity.AppendLine($"            Reflect{pair.Value.Name}::DeserializeEntity(registryRef, entityNode, entity, scene);");
            }

            deserializeEntity.Append("        }");
            Writer.WriteLine(deserializeEntity.ToString());
        }

        private void WriteInitImGui()
        {
            string initImGui = @"
        extern ""C"" __declspec(dllexport) void InitImGui(ImGuiContext* ctx)
        {
            ImGui::SetCurrentContext(ctx);
        }
";
            Writer.Write(initImGui);
        }

        private void WriteImGui()
        {
            StringBuilder imgui = new StringBuilder();
            imgui.Append(@"
        extern ""C"" __declspec(dllexport) void ImGui(entt::registry & registryRef, entt::entity entity)
        {
");
            foreach (var pair in GenFileNameToComponents)
            {
                imgui.AppendLine($"            Reflect{pair.Value.Name}::ImGui(entity, registryRef);");
            }

            imgui.Append("        }");
            Writer.WriteLine(imgui.ToString());
        }
    }
}

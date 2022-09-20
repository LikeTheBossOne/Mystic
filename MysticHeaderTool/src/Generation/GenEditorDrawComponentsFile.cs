using System;
using System.ComponentModel.Design;
using System.IO;
using MysticHeaderTool.Parsing;
using MysticHeaderTool.Reflection;

namespace MysticHeaderTool.Generation
{
    public class GenEditorDrawComponentsFile
    {

        private StreamWriter Writer { get; set; }

        public MReflectionContext Context { get; }
        public string GenPath { get; }

        public GenEditorDrawComponentsFile(MReflectionContext context, string genPath)
        {
            Context = context;
            GenPath = genPath;
        }

        // Generate code that takes in a type and directs it to the correct type-specific widget. (below)
        // For each mstruct in ReflectionContext, generate code for editor widgets for that type
        public bool Generate()
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
            WritePreClassCode();

            foreach (MStruct mstruct in Context.MStructDictionary.Values)
            {
                WriteMStructCode(mstruct);
                Writer.WriteLine();
            }

            WritePostClassCode();


            Writer.Close();

            return true;
        }

        private void WriteIncludes()
        {
            Writer.WriteLine("#include \"../Utility/EditorStructControls.h\"");
            Writer.WriteLine("#include \"Mystic/ECS/Entity.h\"");
        }

        private void WritePreClassCode()
        {
            Writer.WriteLine("namespace Mystic");
            Writer.WriteLine("{");
            Writer.WriteLine("    class DrawGeneratedComponents");
            Writer.WriteLine("    {");
            Writer.WriteLine("    public:");
        }

        private void WritePostClassCode()
        {
            Writer.WriteLine("    };");
            Writer.WriteLine("}");
        }

        private void WriteMStructCode(MStruct mstruct)
        {
            Writer.WriteLine("        static void DrawGeneratedComponents(Entity entity)");
            Writer.WriteLine("        {");
            Writer.WriteLine($"        DrawComponent<{mstruct.Name}>(\"{mstruct.Name}\", entity, []({mstruct.Name}& component)" + Environment.NewLine +
                          "            {");
            foreach (MProperty mprop in mstruct.Properties)
            {
                WriteControl(mprop);
            }
            Writer.WriteLine("        });");
            Writer.WriteLine("    }");
        }

        private void WriteAddCode(MStruct mstruct)
        {

        }

        private void WriteControl(MProperty mprop)
        {
            switch (mprop.Type)
            {
                case MPropertyType.Float:
                    WriteFloatControl(mprop);
                    break;


                case MPropertyType.Vec3:
                    WriteVec3Control(mprop);
                    break;
                default:
                    break;
            }
        }

        #region PropertyControls

        private void WriteFloatControl(MProperty mprop)
        {
            string code = $"            DrawFloatControl(\"{mprop.Name}\", &component.{mprop.Name});";

            Writer.WriteLine(code);
        }

        private void WriteVec3Control(MProperty mprop)
        {
            string code = $"            DrawVec3Control(\"{mprop.Name}\", &component.{mprop.Name});";

            Writer.WriteLine(code);
        }

        #endregion

    }
}

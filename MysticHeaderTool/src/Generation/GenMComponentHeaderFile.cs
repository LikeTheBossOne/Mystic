using System;
using System.IO;
using System.Text;
using MysticHeaderTool.Parsing;

namespace MysticHeaderTool.Generation
{
    public class GenMComponentHeaderFile : GeneratedFile
    {
        private MComponent ReflectedComponent { get; }

        public GenMComponentHeaderFile(string genPath, MComponent component) : base(genPath)
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
            
            WritePreMacros();
            
            WriteMComponentMacros();
            Writer.WriteLine();

            Writer.Close();

            return true;
        }

        private void WritePreMacros()
        {
            StringBuilder sb = new StringBuilder(Path.GetFileName(ReflectedComponent.SourceFilePath));
            sb.Replace('/', '_');
            sb.Replace('\\', '_');
            sb.Replace('.', '_');

            Writer.WriteLine("#undef CURRENT_FILE_PATH");
            Writer.WriteLine("#define CURRENT_FILE_PATH " + sb);
        }

        private void WriteMComponentMacros()
        {
            StringBuilder sb = new StringBuilder(Path.GetFileName(ReflectedComponent.SourceFilePath));
            sb.Replace('/', '_');
            sb.Replace('\\', '_');
            sb.Replace('.', '_');

            Writer.WriteLine($"#define {sb}_{ReflectedComponent.LineNumber}_MGENERATED_INFO \\");
            WriteGeneratedBody();
        }

        private void WriteGeneratedBody()
        {
            string bodyDef = 
                @$"public:                                                           \
    {ReflectedComponent.Name}(entt::entity e, Mystic::Scene* scene) \
    {{                                                                \
        _ent = e;                                                     \
        _owningScene = scene;                                         \
    }}                                                                \
";
            Writer.WriteLine(bodyDef);
        }
    }
}

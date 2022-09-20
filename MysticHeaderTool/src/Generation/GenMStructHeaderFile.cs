using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MysticHeaderTool.Parsing;
using MysticHeaderTool.Reflection;

namespace MysticHeaderTool.Generation
{
    public class GenMStructHeaderFile
    {
        private StreamWriter Writer { get; set; }

        public MStruct ReflectedStruct { get; }
        public string GenPath { get; }

        public GenMStructHeaderFile(MStruct reflectedStruct, string genPath)
        {
            ReflectedStruct = reflectedStruct;
            GenPath = genPath;
        }

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
            WritePreMacros();
            
            WriteMStructMacros();
            Writer.WriteLine();

            Writer.Close();

            return true;
        }

        private void WriteIncludes()
        {

        }

        private void WritePreMacros()
        {
            StringBuilder sb = new StringBuilder(Path.GetFileName(ReflectedStruct.SourceFilePath));
            sb.Replace('/', '_');
            sb.Replace('\\', '_');
            sb.Replace('.', '_');

            Writer.WriteLine("#undef CURRENT_FILE_PATH");
            Writer.WriteLine("#define CURRENT_FILE_PATH " + sb);
        }

        private void WriteMStructMacros()
        {
            StringBuilder sb = new StringBuilder(Path.GetFileName(ReflectedStruct.SourceFilePath));
            sb.Replace('/', '_');
            sb.Replace('\\', '_');
            sb.Replace('.', '_');

            Writer.WriteLine($"#define {sb}_{ReflectedStruct.LineNumber}_MGENERATED_INFO \\");
            Writer.WriteLine('\\');

            foreach (MProperty prop in ReflectedStruct.Properties)
            {
                WriteMPropsToMacro(prop);
                Writer.WriteLine('\\');
            }
        }

        private void WriteMPropsToMacro(MProperty property)
        {
            Writer.WriteLine("public:");
            Writer.WriteLine($"{property.GetPropertyTypeDef()} _MPropGet_{property.Name}(){{ return {property.Name}; }} \\");
            Writer.WriteLine($"void _MPropSet_{property.Name}({property.GetPropertyTypeDef()} value){{ {property.Name} = value; }} \\");
        }
    }
}

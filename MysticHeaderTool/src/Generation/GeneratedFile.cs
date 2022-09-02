using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MysticHeaderTool.Parsing;

namespace MysticHeaderTool.Generation
{
    public class GeneratedFile
    {
        private StreamWriter Writer { get; set; }
        public string FilePath { get; }

        public GeneratedFile(string filePath)
        {
            FilePath = filePath;
        }

        public bool Generate(MStruct structToReflect)
        {
            try
            {
                Writer = new StreamWriter(FilePath);
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return false;
            }

            WriteIncludes();
            WritePreMacros();
            
            WriteMStructMacros(structToReflect);
            Writer.WriteLine();

            return true;
        }

        private void WriteIncludes()
        {

        }

        private void WritePreMacros()
        {
            StringBuilder sb = new StringBuilder(FilePath);
            sb.Replace('/', '_');
            sb.Replace('.', '_');

            Writer.WriteLine("#undef CURRENT_FILE_PATH");
            Writer.WriteLine("#define CURRENT_FILE_PATH " + sb);
        }

        private void WriteMStructMacros(MStruct mStruct)
        {
            StringBuilder sb = new StringBuilder(FilePath);
            sb.Replace('/', '_');
            sb.Replace('.', '_');

            Writer.WriteLine("#define " + sb + "_" + mStruct.LineNumber + " \\");

            foreach (MProperty prop in mStruct.Properties)
            {
                WriteMPropsToMacro(prop);
            }
        }

        private void WriteMPropsToMacro(MProperty property)
        {
            Writer.WriteLine($"{property.Type.GetName()} _MPropGet_{property.Name}(){{ return {property.Name} }} \\");
            Writer.WriteLine($"void _MPropSet_{property.Name}({property.Type} value){{ {property.Name} = value }} \\");
        }
    }
}

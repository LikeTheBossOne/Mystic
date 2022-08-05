using System;
using System.Collections.Generic;
using System.IO;

namespace MysticHeaderTool.Parsing
{
    internal class HeaderParser
    {
        private Dictionary<string, MProperty> _properties;

        public HeaderParser(HeaderParserSettings settings)
        {

        }

        public void Parse(string headerPath)
        {
            string[] lines;
            try
            {
                lines = File.ReadAllLines(headerPath);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return;
            }
            
            
            for (uint i = 0; i < lines.Length; i++)
            {
                string trimmedLine = lines[i].Trim();

                if (trimmedLine.StartsWith("MSTRUCT("))
                {
                    ParseMStruct(in lines, ref i);
                }
                else if (trimmedLine.StartsWith("MCLASS("))
                {
                    ParseMClass(in lines, ref i);
                }
            }
        }

        private MStruct ParseMStruct(in string[] lines, ref uint currIdx)
        {
            if (currIdx + 1 >= lines.Length)
            {
                return null;
            }

            string firstLine = lines[currIdx + 1];

            string[] tokens = firstLine.Split(' ');
            if (tokens.Length < 2 || tokens[0] != "struct")
            {
                return null;
            }

            string typeName = tokens[2].TrimEnd('{');



            return null;
        }

        private MClass ParseMClass(in string[] lines, ref uint currIdx)
        {
            return null;
        }

        private MProperty ParseMProperty(in string metaLine, in string propLine)
        {
            return null;
        }
    }
}

using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;

namespace MysticHeaderTool.Parsing
{
    internal class HeaderParser
    {
        private Dictionary<string, MPropertyType> s_PropDict = new Dictionary<string, MPropertyType>()
        {
            {"float", MPropertyType.Float},
            {"double", MPropertyType.Double},
            {"uint8_t", MPropertyType.UInt8},
            {"uint16_t", MPropertyType.UInt16},
            {"uint32_t", MPropertyType.UInt32},
            {"uint", MPropertyType.UInt32},
            {"uint64_t", MPropertyType.UInt64},
            {"int8_t", MPropertyType.Int8},
            {"int16_t", MPropertyType.UInt16},
            {"int32_t", MPropertyType.UInt32},
            {"int", MPropertyType.Int32},
            {"int64_t", MPropertyType.UInt64},
            {"bool", MPropertyType.Bool},
            {"std::string", MPropertyType.String}
        };
        private Dictionary<string, MProperty> _properties;

        public HeaderParser(HeaderParserSettings settings)
        {
            
        }

        public void Parse(string headerPath)
        {
            Regex r = new Regex("");
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
                    //ParseMClass(in lines, ref i);
                }
            }
        }

        private MStruct ParseMStruct(in string[] lines, ref uint currIdx)
        {
            uint initialIdx = currIdx;

            currIdx++;
            if (currIdx >= lines.Length)
            {
                return null;
            }

            string firstLine = lines[currIdx];

            string[] tokens = firstLine.Split(' ');
            if (tokens.Length < 2 || tokens[0] != "struct")
            {
                return null;
            }

            string typeName = tokens[2].TrimEnd('{');
            var mStruct = new MStruct(typeName, initialIdx);

            while (currIdx < lines.Length)
            {
                string currLine = lines[currIdx];

                if (currLine.StartsWith("MPROPERTY(") && currIdx + 1 < lines.Length)
                {
                    MProperty mProp = ParseMProperty(in currLine, in lines[currIdx + 1]);
                    if (mProp != null)
                    {
                        mStruct.Properties.Add(mProp);

                        currIdx++;
                    }
                }

                currIdx++;
            }

            return mStruct;
        }

        private MProperty ParseMProperty(in string metaLine, in string propLine)
        {
            // Don't do anything with meta information yet.
            bool isConst = false;
            string name;
            MPropertyType type;

            string[] propTokens = propLine.Split(' ');

            uint idx = 0;
            if (idx < propTokens.Length && propTokens[idx] == "const")
            {
                isConst = true;
                idx++;
            }

            if (idx < propTokens.Length)
            {
                string token = propTokens[idx];

                type = s_PropDict[token];

                idx++;
            }

            if (idx < propTokens.Length)
            {
                name = propTokens[idx].TrimEnd(';');

                idx++;
            }



            return null;
        }
    }
}

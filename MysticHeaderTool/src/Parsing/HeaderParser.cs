using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;

namespace MysticHeaderTool.Parsing
{
    internal class HeaderParser
    {
        private static readonly Dictionary<string, MPropertyType> MPropDict = new()
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

        private static readonly Regex MStructClassifier = new(@"struct\s*(?'typename'\w*)\s*\{(?'contents'(?>\{(?<c>)|[^{}]+|\}(?<-c>))*(?(c)(?!)))\}");
        private static readonly Regex MClassClassifier = new(@"class\s*(?'typename'\w*)\s*\{(?'contents'(?>\{(?<c>)|[^{}]+|\}(?<-c>))*(?(c)(?!)))\}");
        private static readonly Regex MStructLineFinder = new(@"^\s*MSTRUCT\((.*?)\)\s*$");

        private static readonly Regex MPropertyClassifier = new(@"MPROPERTY\((?'meta'.*?)\)\s*(?'declaration'.*?);\s");
        private static readonly Regex MPropertyDeclarationParser =
            new(@"^\s*(?'isConst'const)?\s*(?'type'[\w\d:_*]+)\s*(?'name'[\w\d_*]+)\{?(?:.*?)\}?$");

        private Dictionary<string, MProperty> _properties;

        public HeaderParser(HeaderParserSettings settings)
        {
            
        }

        public void Parse(string headerPath)
        {
            Regex propertyAndVariable = new Regex(@"MPROPERTY\((.*)\)\s*(.*);\s");
            Regex propertyFinder = new Regex(@"MPROPERTY\((?:(.*),)*(.*)\)");

            string headerContents = File.ReadAllText(headerPath);

            // First find all MStructs
            List<MStruct> mstructs = FindMStructs(headerContents);

            // Find all MClasses

        }


        private List<MStruct> FindMStructs(string fileContents)
        {
            var mstructs = new List<MStruct>();

            // Get MSTRUCT line numbers and match them to declarations
            string[] lines = fileContents.Split('\n');
            var mstructLineNumbers = new Queue<uint>();
            for (uint i = 1; i <= lines.Length; i++)
            {
                if (MStructLineFinder.IsMatch(lines[i]))
                {
                    mstructLineNumbers.Enqueue(i);
                }
            }


            MatchCollection matches = MStructClassifier.Matches(fileContents);
            foreach (Match match in matches)
            {
                GroupCollection groupCollection = match.Groups;

                Group typenameGroup = groupCollection["typename"];
                Group contentsGroup = groupCollection["contents"];
                if (typenameGroup.Success && contentsGroup.Success)
                {
                    mstructs.Add(ParseMStruct(typenameGroup.Value, contentsGroup.Value, mstructLineNumbers.Dequeue()));
                }
                else
                {
                    throw new Exception("Bad regex group");
                }
            }

            return mstructs;
        }

        private MStruct ParseMStruct(string typeName, string structContents, uint lineNumber)
        {
            var mstruct = new MStruct(typeName, lineNumber)
            {
                Properties = FindMProperties(structContents),
            };

            return mstruct;
        }

        private List<MProperty> FindMProperties(string typeContents)
        {
            var mprops = new List<MProperty>();

            MatchCollection matches = MPropertyClassifier.Matches(typeContents);
            foreach (Match match in matches)
            {
                GroupCollection groupCollection = match.Groups;

                Group metaGroup = groupCollection["meta"];
                Group declarationGroup = groupCollection["declaration"];
                if (declarationGroup.Success)
                {
                    mprops.Add(metaGroup.Success
                        ? ParseMProperty(declarationGroup.Value, metaGroup.Value)
                        : ParseMProperty(declarationGroup.Value));
                }
                else
                {
                    throw new Exception("missing declaration from MPROPERTY");
                }
            }
            return mprops;
        }

        private MProperty ParseMProperty(string declaration, string meta = null)
        {
            Match declarationMatch = MPropertyDeclarationParser.Match(declaration);
            if (!declarationMatch.Success)
            {
                throw new Exception("Failed parsing MPROPERTY with declaration: " + declaration.Trim());
            }

            GroupCollection groups = declarationMatch.Groups;
            Group isConstGroup = groups["isConst"];
            Group typeGroup = groups["type"];
            Group nameGroup = groups["name"];

            if (!typeGroup.Success || !nameGroup.Success)
            {
                throw new Exception("Failed parsing MPROPERTY with declaration: " + declaration.Trim());
            }

            var mprop = new MProperty();

            return null;
        }
    }
}

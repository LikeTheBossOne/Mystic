using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using MysticHeaderTool.Parsing.Exceptions;
using MysticHeaderTool.Reflection;

namespace MysticHeaderTool.Parsing
{
    internal class HeaderParser
    {
        #region Fields

        private MReflectionContext MReflectionContext { get; }

        #endregion

        #region Properties



        #endregion

        #region Private Static Readonly
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

            {"std::string", MPropertyType.String},

            {"vec3_t", MPropertyType.Vec3},
            {"vec4_t", MPropertyType.Vec4},
            {"mat4_t", MPropertyType.Mat4}
        };

        private static readonly Regex MStructClassifier = new(@"\s*MSTRUCT\((?'meta'.*?)\)\s*struct\s*(?'typename'\w*)\s*\{(?'contents'(?>\{(?<c>)|[^{}]+|\}(?<-c>))*(?(c)(?!)))\}");
        private static readonly Regex MClassClassifier = new(@"class\s*(?'typename'\w*)\s*\{(?'contents'(?>\{(?<c>)|[^{}]+|\}(?<-c>))*(?(c)(?!)))\}");
        private static readonly Regex MGeneratedInfoLineFinder = new(@"^\s*MGENERATED_INFO\((.*?)\)\s*$");

        private static readonly Regex MPropertyClassifier = new(@"MPROPERTY\((?'meta'.*?)\)\s*(?'declaration'.*?);\s");
        private static readonly Regex MPropertyDeclarationParser =
            new(@"^\s*(?'isConst'const)?\s*(?'type'[\w\d:_*]+)\s*(?'name'[\w\d_*]+)\{?(?:.*?)\}?$");
        #endregion

        public HeaderParser(HeaderParserSettings settings, MReflectionContext mReflectionContext)
        {
            MReflectionContext = mReflectionContext;
        }

        public MStruct ParseHeader(string headerPath)
        {
            string headerContents = File.ReadAllText(headerPath);

            // First find all MStructs
            MStruct mstruct = FindMStruct(headerPath, headerContents);

            return mstruct;
        }


        private MStruct FindMStruct(string headerPath, string fileContents)
        {
            MatchCollection matches = MStructClassifier.Matches(fileContents);
            if (matches.Count > 1)
            {
                throw new Exception($"Too many structs in file: {headerPath}");
            }
            else if (matches.Count == 0)
            {
                return null;
            }
            
            GroupCollection groupCollection = matches[0].Groups;

            Group typenameGroup = groupCollection["typename"];
            Group contentsGroup = groupCollection["contents"];
            if (typenameGroup.Success && contentsGroup.Success)
            {
                // Get MGENERATED_BODY line number
                string[] lines = fileContents.Split('\n');
                uint line = 0;
                for (uint i = 0; i < lines.Length; i++)
                {
                    if (MGeneratedInfoLineFinder.IsMatch(lines[i]))
                    {
                        line = i + 1;
                        break;
                    }
                }

                if (line == 0)
                {
                    throw new Exception("Could not find MGENERATED_INFO()");
                }

                return ParseMStruct(typenameGroup.Value, contentsGroup.Value, headerPath, line);
            }
            else
            {
                throw new Exception("Bad regex group");
            }
        }

        private MStruct ParseMStruct(string typeName, string structContents, string headerPath, uint lineNumber)
        {
            var mstruct = new MStruct(typeName, headerPath, lineNumber)
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

            MProperty mProp;
            
            if (MPropDict.ContainsKey(typeGroup.Value))
            {
                mProp = new MProperty(nameGroup.Value, MPropDict[typeGroup.Value], isConstGroup.Success);
            }
            else if (MReflectionContext.MStructDictionary.ContainsKey(typeGroup.Value))
            {
                mProp = new MPropertyStructType(nameGroup.Value, typeGroup.Value, isConstGroup.Success);
            }
            else
            {
                throw new InvalidMPropertyTypeException("Invalid type: " + typeGroup.Value, typeGroup.Value);
            }

            return mProp;
        }
    }
}


using System;
using System.Collections.Generic;
using System.IO;
using MysticHeaderTool.Parsing;
using MysticHeaderTool.Reflection;

namespace MysticHeaderTool.Generation
{
    public class MCodeGenerator
    {
        #region Fields

        private MReflectionContext MReflectionContext { get; }

        #endregion

        public MCodeGenerator(MReflectionContext context)
        {
            MReflectionContext = context;
        }

        public void GenerateHeaderFiles(DirectoryInfo outputDir)
        {
            foreach (MStruct mStruct in MReflectionContext.MStructDictionary.Values)
            {
                string headerName = Path.GetFileName(mStruct.SourceFilePath);
                
                int hIndex = headerName.LastIndexOf(".h", StringComparison.Ordinal);

                string pathToGen;
                if (hIndex == -1)
                {
                    pathToGen = Path.Combine(outputDir.FullName, $"{headerName}.generated.h");
                }
                else
                {
                    pathToGen = Path.Combine(outputDir.FullName, headerName.Substring(0, hIndex) + ".generated.h");
                }

                var genFile = new GenMStructHeaderFile(mStruct, pathToGen);
                genFile.Generate();
            }
        }

        public void GenerateEditorFiles(DirectoryInfo outputDir)
        {
            var componentsFile = new GenEditorDrawComponentsFile(MReflectionContext, Path.Combine(outputDir.FullName, "DrawComponents.Generated.h"));
            componentsFile.Generate();
        }
    }
}

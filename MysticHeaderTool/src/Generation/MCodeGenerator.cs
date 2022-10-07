
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
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

        public void GenerateMComponentBodyHeaderFiles(DirectoryInfo outputDir)
        {
            foreach (MComponent component in MReflectionContext.MComponents)
            {
                string headerName = Path.GetFileName(component.SourceFilePath);
                
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

                var genFile = new GenMComponentHeaderFile(pathToGen, component);
                genFile.Generate();
            }
        }

        public void GenerateReflectionFiles(DirectoryInfo outputDir)
        {
            var fileNamesToComponents = new List<KeyValuePair<string,MComponent>>();
            foreach (MComponent component in MReflectionContext.MComponents)
            {
                StringBuilder fileNameBuilder = new StringBuilder();
                fileNameBuilder
                    .Append("Reflect")
                    .Append(component.Name)
                    .Append(".generated.h");
                string fileName = fileNameBuilder.ToString();

                fileNamesToComponents.Add(new KeyValuePair<string, MComponent>(fileName, component));

                var genComponentReflectionFile = new GenMComponentReflectionFile(Path.Combine(outputDir.FullName, fileName), component);
                genComponentReflectionFile.Generate();
            }

            string externFilePath = Path.Combine(outputDir.FullName, "Init.h");

            var genExternFile = new GenGameCodeExternFile(externFilePath, fileNamesToComponents);
            genExternFile.Generate();
        }

        public void GenerateNativeScriptFiles(DirectoryInfo outputDir)
        {
            string outputPath = Path.Combine(outputDir.FullName, "NativeScriptTemplates.h");

            var genTemplatesFile = new GenScriptTemplatesFile(outputPath, MReflectionContext.MComponents);
            genTemplatesFile.Generate();
        }
    }
}

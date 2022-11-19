using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Linq.Expressions;
using System.Runtime.CompilerServices;
using MysticHeaderTool.FileUtils;
using MysticHeaderTool.Generation;
using MysticHeaderTool.Parsing;
using MysticHeaderTool.Parsing.Exceptions;
using MysticHeaderTool.Reflection;

[assembly: InternalsVisibleTo("MysticHeaderToolTests")]

namespace MysticHeaderTool
{
    internal class MysticHeaderTool
    {
        private static readonly string GenReflectionHeaderPath = Path.Combine("Intermediate", "Reflection");
        private static readonly string GenHeaderPath = Path.Combine("Intermediate","Generated_H");
        private static readonly string GenEditorPath = Path.Combine("Intermediate", "Generated_Editor");

        /*
         * This should run when Mystic or MysticEditor BUILDS.
         *
         * OR it can be called manually to force code generation
         */
        static void Main(string[] args)
        {
            Console.Out.WriteLine("Starting Mystic Header Tool");
            Console.Out.WriteLine(args[0] + " " + args[1] + " " + args[2] + " " + args[3]);
            if (args.Length < 4)
            {
                throw new ArgumentException("usage: MysticHeaderTool.exe <path-to-parse> <path-to-root> <path-to-output> <last-build(YYYY-MM-DD:hh-mm-ss)>");
            }

            string pathToParse = args[0];
            string pathToRoot = args[1];
            string pathToOutput = args[2];
            string lastBuildStr = args[3];

            DirectoryInfo parseDir = new DirectoryInfo(pathToParse);
            DirectoryInfo outputDir = new DirectoryInfo(pathToOutput);
            string format = "dd/MM/yyyy-HH:mm:ss.ffffff";
            DateTime lastBuildTime = DateTime.ParseExact(lastBuildStr, format, CultureInfo.CurrentCulture);

            Console.Out.WriteLine("Got past start");

            // DO HEADER PARSING

            MReflectionContext context = new MReflectionContext();
            HeaderParser headerParser = new HeaderParser(new HeaderParserSettings(), context);

            List<FileInfo> filesToParse = FileFinder.GetFilesByModifiedTime(parseDir, lastBuildTime);
            foreach (FileInfo file in filesToParse)
            {
                MComponent component;
                try
                {
                    component = headerParser.ParseHeader(file.FullName);
                }
                catch (InvalidMPropertyTypeException e)
                {
                    continue;
                }

                if (component != null)
                {
                    //context.MClassDictionary.Add(mstruct.Name, mstruct);
                    context.MComponents.Add(component);
                }
            }

            // DO CODE GEN
            var generator = new MCodeGenerator(context);

            var headerGenDir = new DirectoryInfo(Path.Combine(pathToRoot, GenHeaderPath));
            if (!headerGenDir.Exists)
            {
                headerGenDir.Create();
            }

            generator.GenerateMComponentBodyHeaderFiles(headerGenDir);

            generator.GenerateReflectionFiles(outputDir);

            generator.GenerateNativeScriptFiles(outputDir);
        }
    }
}

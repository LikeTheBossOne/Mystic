using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Linq.Expressions;
using System.Runtime.CompilerServices;
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
            string binDir = Environment.CurrentDirectory;
            DirectoryInfo baseDir = Directory.GetParent(binDir)?.Parent?.Parent;
            if (baseDir == null || !baseDir.Exists)
            {
                throw new Exception("Could not find base directory");
            }

            DirectoryInfo srcDir = null;
            foreach (DirectoryInfo projectDir in baseDir.EnumerateDirectories())
            {
                if (projectDir.Name == "Sandbox")
                {
                    foreach (DirectoryInfo dir in projectDir.EnumerateDirectories())
                    {
                        if (dir.Name == "src")
                        {
                            srcDir = projectDir;
                            break;
                        }
                    }
                    break;
                }
            }

            if (srcDir == null)
            {
                throw new Exception("Could not find src directory for project");
            }


            // DO HEADER PARSING

            MReflectionContext context = new MReflectionContext();
            HeaderParser headerParser = new HeaderParser(new HeaderParserSettings(), context);

            // Enumerate srcDirs for all .h files and try to grab MStructs (inefficient solution)
            // TODO: Create solution that resolves via a dependency graph
            var filePaths = new Queue<string>(srcDir.EnumerateFiles("*.h", SearchOption.AllDirectories).ToList().ConvertAll((fileInfo) => fileInfo.FullName));
            
            while (filePaths.Count > 0)
            {
                string headerPath = filePaths.Dequeue();

                MStruct mstruct;
                try
                {
                    mstruct = headerParser.ParseHeader(headerPath);
                }
                catch (InvalidMPropertyTypeException e)
                {
                    // TODO: Use e.MPropertyTypeDef in dependency graph
                    filePaths.Enqueue(headerPath);
                    continue;
                }
                
                if (mstruct != null)
                {
                    context.MStructDictionary.Add(mstruct.Name, mstruct);
                }
            }


            // DO CODE GEN
            var generator = new MCodeGenerator(context);

            var headerGenDir = new DirectoryInfo(Path.Combine(baseDir.FullName, GenHeaderPath));
            if (!headerGenDir.Exists)
            {
                headerGenDir.Create();
            }

            generator.GenerateHeaderFiles(headerGenDir);


            //var editorGenDir = new DirectoryInfo(Path.Combine(baseDir.FullName, GenEditorPath));
            //if (!editorGenDir.Exists)
            //{
            //    editorGenDir.Create();
            //}

            //generator.GenerateEditorFiles(editorGenDir);
        }
    }
}

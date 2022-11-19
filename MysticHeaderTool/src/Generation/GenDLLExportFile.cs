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
    public class GenDLLExportFile : GeneratedFile
    {
        private MReflectionContext ReflectionContext { get; }

        public GenDLLExportFile(string genPath, MReflectionContext reflectionContext) : base(genPath)
        {
            ReflectionContext = reflectionContext;
        }

        public override bool Generate()
        {
            throw new NotImplementedException();
        }
    }
}

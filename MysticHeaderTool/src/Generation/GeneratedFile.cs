using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MysticHeaderTool.Parsing;

namespace MysticHeaderTool.Generation
{
    public abstract class GeneratedFile
    {
        protected StreamWriter Writer { get; set; }

        public string GenPath { get; }

        public GeneratedFile(string genPath)
        {
            GenPath = genPath;
        }

        public abstract bool Generate();
    }
}

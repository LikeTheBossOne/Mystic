using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MysticHeaderTool.Parsing;

namespace MysticHeaderTool.Reflection
{
    public class MReflectionContext
    {
        public Dictionary<string, MStruct> MStructDictionary { get; } // <MStruct.Name, MStruct>

        public MReflectionContext()
        {
            MStructDictionary = new Dictionary<string, MStruct>();
        }
    }
}

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
        public Dictionary<string, MClass> MClassDictionary { get; } // <MComponent.Name, MComponent>
        public List<MComponent> MComponents { get; }

        public MReflectionContext()
        {
            MClassDictionary = new Dictionary<string, MClass>();
            MComponents = new List<MComponent>();
        }
    }
}

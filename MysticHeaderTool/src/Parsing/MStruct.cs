using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MysticHeaderTool.Parsing
{
    public class MStruct
    {
        public MStruct(string name, uint lineNumber)
        {
            Properties = new List<MProperty>();
            LineNumber = lineNumber;
        }

        public string Name { get; set; }
        public List<MProperty> Properties { get; set; }
        public uint LineNumber { get; set; }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MysticHeaderTool.Parsing
{
    public class MClass
    {
        public MClass(string name)
        {
            Properties = new List<MProperty>();
        }

        public string Name { get; set; }
        public List<MProperty> Properties { get; set; }
    }
}

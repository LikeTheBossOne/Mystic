using System.Collections.Generic;

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

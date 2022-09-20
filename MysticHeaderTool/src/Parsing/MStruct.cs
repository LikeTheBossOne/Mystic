using System.Collections.Generic;
using System.Linq;

namespace MysticHeaderTool.Parsing
{
    public class MStruct
    {
        public MStruct(string name, string sourceFilePath, uint lineNumber)
        {
            Name = name;
            Properties = new List<MProperty>();
            SourceFilePath = sourceFilePath;
            LineNumber = lineNumber;
        }

        public string Name { get; set; }
        public List<MProperty> Properties { get; set; }
        public string SourceFilePath { get; set; }
        public uint LineNumber { get; set; }

        public override bool Equals(object obj)
        {
            return Equals(obj as MStruct);
        }

        public bool Equals(MStruct other)
        {
            return other != null &&
                   Name == other.Name && 
                   ((Properties == null && other.Properties == null) || Properties != null && other.Properties != null && Properties.SequenceEqual(other.Properties)) && 
                   LineNumber == other.LineNumber;
        }

        public override int GetHashCode()
        {
            unchecked
            {
                var hashCode = (Name != null ? Name.GetHashCode() : 0);
                hashCode = (hashCode * 397) ^ (Properties != null ? Properties.GetHashCode() : 0);
                hashCode = (hashCode * 397) ^ (int) LineNumber;
                return hashCode;
            }
        }
    }
}

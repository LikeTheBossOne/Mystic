using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace MysticHeaderTool.Parsing
{
    public class MComponent
    {
        private static readonly Regex ImGuiRegex = new Regex(@"
                (?<=[A-Z])(?=[A-Z][a-z]) |
                 (?<=[^A-Z])(?=[A-Z]) |
                 (?<=[A-Za-z])(?=[^A-Za-z])", RegexOptions.IgnorePatternWhitespace);

        public string Name { get; set; }
        public List<MProperty> Properties { get; set; }
        public string SourceFilePath { get; set; }
        public uint LineNumber { get; set; }
        public string ImGuiName { get; }

        public MComponent(string name, string sourceFilePath, uint lineNumber)
        {
            Name = name;
            Properties = new List<MProperty>();
            SourceFilePath = sourceFilePath;
            LineNumber = lineNumber;

            ImGuiName = ImGuiRegex.Replace(name, " ");
        }

        public override bool Equals(object obj)
        {
            return Equals(obj as MComponent);
        }

        public bool Equals(MComponent other)
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

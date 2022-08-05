#nullable enable
using System;
using System.Linq;

namespace MysticHeaderTool.Parsing
{
    public class MPropertyNameAttribute : Attribute
    {
        public string Name { get; private set; }

        public MPropertyNameAttribute(string name)
        {
            this.Name = name;
        }
    }

    public enum MPropertyType
    {
        [MPropertyName("Struct")]
        Struct,
        [MPropertyName("Float")]
        Float,
        [MPropertyName("Int")]
        Int,
        [MPropertyName("Bool")]
        Bool,
        [MPropertyName("String")]
        String
    }

    public static class MPropertyExtensions
    {
        public static string GetName(this MPropertyType value)
        {
            var enumType = value.GetType();
            var name = Enum.GetName(enumType, value);
            return enumType.GetField(name).GetCustomAttributes(false).OfType<MPropertyNameAttribute>().SingleOrDefault()?.Name ?? value.ToString();
        }
    }

    public class MProperty
    {
        public string Name { get; set; }
        public MPropertyType Type { get; set; }
    }
}

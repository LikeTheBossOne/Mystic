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
        [MPropertyName("Double")]
        Double,
        [MPropertyName("Int8")]
        Int8,
        [MPropertyName("Int16")]
        Int16,
        [MPropertyName("Int32")]
        Int32,
        [MPropertyName("Int64")]
        Int64,
        [MPropertyName("UInt8")]
        UInt8,
        [MPropertyName("UInt16")]
        UInt16,
        [MPropertyName("UInt32")]
        UInt32,
        [MPropertyName("UInt64")]
        UInt64,
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
        public MProperty(string name, MPropertyType type, bool isConst)
        {
            Name = name;
            Type = type;
            IsConst = isConst;
        }

        public string Name { get; set; }
        public MPropertyType Type { get; set; }
        public bool IsConst { get; set; }
    }
}

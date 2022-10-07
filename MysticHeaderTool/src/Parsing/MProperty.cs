#nullable enable
using System;
using System.Linq;

namespace MysticHeaderTool.Parsing
{
    public class MPropertyTypeDefAttribute : Attribute
    {
        public string Name { get; private set; }

        public MPropertyTypeDefAttribute(string name)
        {
            this.Name = name;
        }
    }

    public enum MPropertyType
    {
        [MPropertyTypeDef("struct")]
        Struct,

        [MPropertyTypeDef("float")]
        Float,
        [MPropertyTypeDef("double")]
        Double,

        [MPropertyTypeDef("int8_t")]
        Int8,
        [MPropertyTypeDef("int16_t")]
        Int16,
        [MPropertyTypeDef("int32_t")]
        Int32,
        [MPropertyTypeDef("int64_t")]
        Int64,

        [MPropertyTypeDef("uint8_t")]
        UInt8,
        [MPropertyTypeDef("uint16_t")]
        UInt16,
        [MPropertyTypeDef("uint32_t")]
        UInt32,
        [MPropertyTypeDef("uint64_t")]
        UInt64,

        [MPropertyTypeDef("bool")]
        Bool,

        [MPropertyTypeDef("std::string")]
        String,

        [MPropertyTypeDef("vec3_t")]
        Vec3,
        [MPropertyTypeDef("vec4_t")]
        Vec4,
        [MPropertyTypeDef("mat4_t")]
        Mat4,

        Invalid
    }

    public static class MPropertyExtensions
    {
        internal static string GetTypeDef(this MPropertyType value)
        {
            var enumType = value.GetType();
            var name = Enum.GetName(enumType, value);
            return enumType.GetField(name).GetCustomAttributes(false).OfType<MPropertyTypeDefAttribute>().SingleOrDefault()?.Name ?? value.ToString();
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

        public virtual string GetPropertyTypeDef()
        {
            return Type.GetTypeDef();
        }

        public override bool Equals(object obj)
        {
            return Equals(obj as MProperty);
        }

        public bool Equals(MProperty other)
        {
            return other != null && Name == other.Name && Type == other.Type && IsConst == other.IsConst;
        }

        public override int GetHashCode()
        {
            unchecked
            {
                var hashCode = Name.GetHashCode();
                hashCode = (hashCode * 397) ^ (int) Type;
                hashCode = (hashCode * 397) ^ IsConst.GetHashCode();
                return hashCode;
            }
        }
    }

    public class MPropertyClassType : MProperty
    {
        public MPropertyClassType(string name, string classTypeName, bool isConst) : base(name, MPropertyType.Struct, isConst)
        {
            ClassTypeName = classTypeName;
        }

        public string ClassTypeName { get; set; }

        public override string GetPropertyTypeDef()
        {
            return $"Ref<{ClassTypeName}>";
        }

        public override bool Equals(object obj)
        {
            return Equals(obj as MPropertyClassType);
        }

        protected bool Equals(MPropertyClassType other)
        {
            return other != null && ClassTypeName == other.ClassTypeName && base.Equals(other);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                return (base.GetHashCode() * 397) ^ ClassTypeName.GetHashCode();
            }
        }
    }
}

using System;

namespace MysticHeaderTool.Parsing.Exceptions
{
    public class InvalidMPropertyTypeException : Exception
    {
        public string MPropertyTypeDef { get; }

        public InvalidMPropertyTypeException(string message, string type) : base(message)
        {
            MPropertyTypeDef = type;
        }
    }
}

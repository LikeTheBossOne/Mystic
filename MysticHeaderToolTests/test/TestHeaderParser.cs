using System.Collections.Generic;
using MysticHeaderTool.Parsing;
using MysticHeaderTool.Reflection;

namespace MysticHeaderToolTests
{
    [TestClass]
    public class TestHeaderParser
    {
        [TestMethod]
        public void ParseHeader_SingleMStructTest()
        {
            // Expected
            var expected = new MComponent("TransformComponent", @"testfiles/ComponentTestValid.h.test", 9);
            expected.Properties.AddRange(new List<MProperty>
                {
                    new("Position", MPropertyType.Vec3, false),
                    new("Rotation", MPropertyType.Vec4, false),
                    new("Scale", MPropertyType.Vec3, false)
                }
            );


            // Actual
            var refContext = new MReflectionContext();
            var hp = new HeaderParser(new HeaderParserSettings(), refContext);
            MComponent actual = hp.ParseHeader(@"testfiles/ComponentTestValid.h.test");


            // Compare
            Assert.AreEqual(expected, actual);
        }
    }
}
using System.Collections.Generic;
using MysticHeaderTool.Parsing;
using MysticHeaderTool.Reflection;
using MysticHeaderTool.Generation;

namespace MysticHeaderToolTests
{
    [TestClass]
    public class TestGeneratedHeaderFile
    {
        [TestMethod]
        public void Generate_Test()
        {
            // Setup
            var outputPath = @"testfiles/output.generated.h";
            var testStruct = new MStruct("TransformComponent", @"testfiles/ComponentTestValid.h.test", 9);
            testStruct.Properties.AddRange(new List<MProperty>
                {
                    new("Position", MPropertyType.Vec3, false),
                    new("Rotation", MPropertyType.Vec4, false),
                    new("Scale", MPropertyType.Vec3, false)
                }
            );
            var generatedFile = new GenMStructHeaderFile(testStruct, outputPath);

            // Assert
            Assert.IsTrue(generatedFile.Generate());
        }
    }
}

using System.Collections.Generic;
using MysticHeaderTool.Parsing;
using MysticHeaderTool.Generation;

namespace MysticHeaderToolTests
{
    [TestClass]
    public class TestGenMComponentHeaderFile
    {
        [TestMethod]
        public void Generate_Test()
        {
            // Setup
            var outputPath = @"testfiles/output.generated.h";
            var testComponent = new MComponent("TransformComponent", @"testfiles/ComponentTestValid.h.test", 9);
            testComponent.Properties.AddRange(new List<MProperty>
                {
                    new("Position", MPropertyType.Vec3, false),
                    new("Rotation", MPropertyType.Vec4, false),
                    new("Scale", MPropertyType.Vec3, false)
                }
            );
            var generatedFile = new GenMComponentHeaderFile(outputPath, testComponent);

            // Assert
            Assert.IsTrue(generatedFile.Generate());
        }
    }
}

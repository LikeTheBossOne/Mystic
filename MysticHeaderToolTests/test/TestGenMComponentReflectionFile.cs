using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MysticHeaderTool.Generation;
using MysticHeaderTool.Parsing;

namespace MysticHeaderToolTests
{
    [TestClass]
    public class TestGenMComponentReflectionFile
    {
        [TestMethod]
        public void Generate_Test()
        {
            // Setup
            var outputPath = @"testfiles/ReflectOutput.h";
            var testComponent = new MComponent("TransformComponent", @"testfiles/ComponentTestValid.h.test", 9);
            testComponent.Properties.AddRange(new List<MProperty>
                {
                    new("Position", MPropertyType.Float, false),
                    new("Rotation", MPropertyType.Int32, false),
                    new("Scale", MPropertyType.Float, false)
                }
            );
            var generatedFile = new GenMComponentReflectionFile(outputPath, testComponent);

            // Assert
            Assert.IsTrue(generatedFile.Generate());
        }
    }
}

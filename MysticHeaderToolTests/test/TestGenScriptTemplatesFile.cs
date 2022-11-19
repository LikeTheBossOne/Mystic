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
    public class TestGenScriptTemplatesFile
    {
        [TestMethod]
        public void Generate_Test()
        {
            // Setup
            var outputPath = @"testfiles/GenNativeScriptsTemplate.h";
            var testComponent1 = new MComponent("TransformComponent", @"unimportant", 9);
            testComponent1.Properties.AddRange(new List<MProperty>
                {
                    new("Position", MPropertyType.Vec3, false),
                    new("Rotation", MPropertyType.Vec4, false),
                    new("Scale", MPropertyType.Vec3, false)
                }
            );
            var testComponent2 = new MComponent("LaserComponent", @"unimportant", 9);
            testComponent2.Properties.AddRange(new List<MProperty>
                {
                    new("Damage", MPropertyType.Float, false)
                }
            );
            var componentList = new List<MComponent>();
            componentList.Add(testComponent1);
            componentList.Add(testComponent2);

            var generatedFile = new GenScriptTemplatesFile(outputPath, componentList);

            // Assert
            Assert.IsTrue(generatedFile.Generate());
        }
    }
}

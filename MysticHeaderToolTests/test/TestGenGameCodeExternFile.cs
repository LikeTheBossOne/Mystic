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
    public class TestGenGameCodeExternFile
    {
        [TestMethod]
        public void Generate_Test()
        {
            // Setup
            var outputPath = @"testfiles/GenInitFile.h";
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
            var componentList = new List<KeyValuePair<string,MComponent>>();
            componentList.Add(new KeyValuePair<string, MComponent>("transform.generated.h", testComponent1));
            componentList.Add(new KeyValuePair<string, MComponent>("laser.generated.h", testComponent2));

            var generatedFile = new GenGameCodeExternFile(outputPath, componentList);

            // Assert
            Assert.IsTrue(generatedFile.Generate());
        }
    }
}

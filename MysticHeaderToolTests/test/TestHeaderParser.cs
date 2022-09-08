using MysticHeaderTool.Parsing;

namespace MysticHeaderToolTests
{
    [TestClass]
    public class TestHeaderParser
    {
        [TestMethod]
        public void TestMethod1()
        {
            HeaderParser hp = new HeaderParser(new HeaderParserSettings());

            hp.Parse(@"testfiles/ComponentTestValid.h.test");
        }
    }
}

namespace MysticHeaderTool.Generation
{
    public class CodeGenerator
    {
        // #define CURRENT_FILE_ID
        // 

        private static readonly string MPropertyTemplate =
            "public:" +
            "   MPropValType getProperty() { return _mPropVal; }" +
            "   void setProperty(MPropValType value) { _mPropVal = value; }";

        // each generated.h file should have a list of things to generate in it
    }
}

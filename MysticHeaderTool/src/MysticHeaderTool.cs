using System.Runtime.CompilerServices;
[assembly: InternalsVisibleTo("MysticHeaderToolTests")]

namespace MysticHeaderTool
{
    internal class MysticHeaderTool
    {
        /*
         * This should run when Mystic or MysticEditor BUILDS.
         *
         * OR it can be called manually to force code generation
         */
        static void Main(string[] args)
        {
            // Loop through header files that have .generated.h files attached to them.
            // // If file has MCOMPONENT, mark the class attached to it as a component needing code
            // // an MPROPERTY tags, ascertain the type of the component, and generate ImGui Editor code, and generate
            // 
            // // If file has MSTRUCT, add struct to dictionary as a type that can have ImGui code generated for it.

        }
    }
}

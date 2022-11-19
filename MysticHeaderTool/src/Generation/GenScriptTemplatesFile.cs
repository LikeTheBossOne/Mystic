using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using MysticHeaderTool.Parsing;

namespace MysticHeaderTool.Generation
{
    public class GenScriptTemplatesFile : GeneratedFile
    {
        private List<MComponent> Components;

        public GenScriptTemplatesFile(string genPath, List<MComponent> components) : base(genPath)
        {
            Components = components;
        }

        public override bool Generate()
        {
            try
            {
                Writer = new StreamWriter(GenPath, false);
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return false;
            }

            Writer.WriteLine("#pragma once");
            Writer.WriteLine();

            WriteGetComponentsFuncs();

            Writer.WriteLine();

            Writer.Close();

            return true;
        }

        private void WriteGetComponentsFuncs()
        {
            foreach (MComponent component in Components)
            {
                Writer.WriteLine($@"
template<> {component.Name}& Mystic::NativeScriptComponent::GetComponent<{component.Name}>() const
{{
	return _owningScene->EntityGetComponent<{component.Name}>(_ent);
}}");
            }
        }
    }
}

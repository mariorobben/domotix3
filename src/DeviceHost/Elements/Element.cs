using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceHost.Core
{
    public sealed class Element
    {
        public Element(String name, IElementReference reference)
        {
            Name = name;
            Reference = reference;
        }

        public String Name { get; private set; }
        public IElementReference Reference { get; private set; }
    }
}

using System;

namespace DeviceHost.Elements
{
    [AttributeUsage(AttributeTargets.Class)]
    public class ElementAttribute : Attribute
    {
        public ElementAttribute(String name)
        {
            Name = name;
        }

        public String Name { get; private set; }
    }
}

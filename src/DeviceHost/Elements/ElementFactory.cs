using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DeviceHost.Core;
using DeviceHost.ElementReferences;
using DeviceHost.Configuration;

namespace DeviceHost.Elements
{
    public static class ElementFactory
    {
        public static IElementReference CreateElementReference(IElementConfiguration elementConfiguration)
        {
            if (elementConfiguration is BinaryInputElementConfiguration binaryInputElementConfiguration)
            {
                return new BinaryInputElementReference(binaryInputElementConfiguration.Input);
            }
            else if (elementConfiguration is BinaryOutputElementConfiguration binaryOutputElementConfiguration)
            {
                return new BinaryOutputElementReference(binaryOutputElementConfiguration.Output);
            }
            else if (elementConfiguration is AnalogOutputElementConfiguration analogOutputElementConfiguration)
            {
                return new AnalogOutputElementReference(analogOutputElementConfiguration.Output);
            }
            else if (elementConfiguration is RgbElementConfiguration rgbElementConfiguration)
            {
                return new RgbElementReference(rgbElementConfiguration);
            }

            throw new ArgumentException("Unknown element configuration");
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using DeviceHost.Core;

namespace DeviceHost.ElementReferences
{
    public class AnalogOutputElementReference : IElementReference, IEquatable<AnalogOutputElementReference>
    {
        public AnalogOutputElementReference(Int32 output)
        {
            _output = output;
        }

        public Int32 Output { get { return _output; } }

        public override int GetHashCode()
        {
            return _output;
        }

        public override bool Equals(object obj)
        {
            return Equals(obj as AnalogOutputElementReference);
        }

        #region IEquatable<AnalogOutput_0_10V_ElementReference> implementation

        public bool Equals(AnalogOutputElementReference other)
        {
            return other != null && _output == other._output;
        }

        #endregion

        private readonly Int32 _output;
    }
}

using System;
using DeviceHost.Core;
using DeviceHost.Configuration;

namespace DeviceHost.ElementReferences
{
    public class BinaryOutputElementReference : IElementReference, IEquatable<BinaryOutputElementReference>
    {
        public BinaryOutputElementReference(Int32 output)
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
            return Equals(obj as BinaryOutputElementReference);
        }

        #region IEquatable<BinaryOutputElementReference> implementation

        public bool Equals(BinaryOutputElementReference other)
        {
            return other != null && _output == other._output;
        }

        #endregion

        private Int32 _output;
    }
}

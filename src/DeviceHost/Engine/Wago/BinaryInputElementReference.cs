using System;
using DeviceHost.Core;

namespace DeviceHost.ElementReferences
{
    public class BinaryInputElementReference : IElementReference, IEquatable<BinaryInputElementReference>
    {
        public BinaryInputElementReference(Int32 input)
        {
            _input = input;
        }

        public Int32 Input { get { return _input; } }

        public override int GetHashCode()
        {
            return _input;
        }

        public override bool Equals(object obj)
        {
            return Equals(obj as BinaryInputElementReference);
        }

        #region IEquatable<BinaryInputElementReference> implementation

        public bool Equals(BinaryInputElementReference other)
        {
            return other != null && _input == other._input;
        }

        #endregion

        private readonly Int32 _input;
    }
}

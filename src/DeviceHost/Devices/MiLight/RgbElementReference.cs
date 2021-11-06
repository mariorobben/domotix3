using System;
using DeviceHost.Core;
using DeviceHost.Configuration;

namespace DeviceHost.ElementReferences
{
    public class RgbElementReference : IElementReference, IEquatable<RgbElementReference>
    {
        public RgbElementReference(Int32 zone)
        {
            _zone = zone;
        }

        public RgbElementReference(RgbElementConfiguration rgbElementConfiguration)
        {
            _zone = rgbElementConfiguration.Zone;
        }

        public Int32 Zone { get { return _zone; } }

        public override int GetHashCode()
        {
            return _zone;
        }

        public override bool Equals(object obj)
        {
            return Equals(obj as RgbElementReference);
        }

        #region IEquatable<ZoneElementReference> implementation

        public bool Equals(RgbElementReference other)
        {
            return other != null && _zone == other._zone;
        }

        #endregion

        private readonly Int32 _zone;
    }
}

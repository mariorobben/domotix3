using System;

namespace Domotix3.Devices.MiLight
{
    public sealed class Brightness
    {
        public Brightness(byte value)
        {
            Value = value;
        }

        public byte Value { get; init; }
    }
}

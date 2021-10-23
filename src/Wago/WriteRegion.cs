using System;
using System.Linq;

namespace Domotix3.Devices.Wago
{
    public sealed class WriteRegion
    {
        public WriteRegion(int offset, Byte[] bytes)
        {
            Offset = offset;
            Bytes = bytes.ToArray();
        }

        public int Offset { get; init; }

        public Byte[] Bytes { get; init; }
    }
}

using System;
using System.Collections.Generic;

namespace Domotix3.Devices.Wago
{
    public interface IWago
    {
        Status GetStatus();
        IReadOnlyCollection<Terminal> GetTerminals();
        void WriteRegion(WriteRegion region);
        Byte[] ReadRegion(ReadRegion region);
    }
}

using System;
using System.Collections.Generic;

namespace Domotix3.Devices.Wago
{
    public interface IWago
    {
        Status GetStatus();
        IReadOnlyCollection<Terminal> GetTerminals();
        void WriteRegions(IReadOnlyCollection<WriteRegion> regions);
        IReadOnlyCollection<Byte[]> ReadRegions(IReadOnlyCollection<ReadRegion> regions);
    }
}

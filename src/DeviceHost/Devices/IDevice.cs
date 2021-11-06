using System;
using System.Collections.Generic;

namespace DeviceHost.Devices
{
    public interface IDevice
    {
        String DeviceName { get; }
    }

    public interface IElements
    {
        IEnumerable<object> Elements { get; }
    }
}

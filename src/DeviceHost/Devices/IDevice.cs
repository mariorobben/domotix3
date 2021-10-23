using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceHost.Devices
{
    public interface IDevice
    {
        String DeviceName { get; }
    }
}

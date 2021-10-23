using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceHost.Devices
{
    public abstract class AbstractDevice : IDevice
    {
        protected AbstractDevice(string deviceName)
        {
            DeviceName = deviceName;
        }

        public string DeviceName { get; init; }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DeviceHost.Configuration;
using DeviceHost.Devices.Wago;
using DeviceHost.Engine.MiLight;

namespace DeviceHost.Devices
{
    public static class DeviceFactory
    {
        public static IDevice CreateDevice(IDeviceConfiguration deviceConfiguration)
        {
            if (deviceConfiguration is WagoDeviceConfiguration wagoDeviceConfiguration)
            {
                return new WagoDevice(wagoDeviceConfiguration.Name, wagoDeviceConfiguration.Modules);
            }
            else if (deviceConfiguration is MiLightDeviceConfiguration miLightDeviceConfiguration)
            {
                return new MiLightDevice(miLightDeviceConfiguration.Name);
            }

            throw new InvalidOperationException("Invalid device configuration type.");
        }
    }
}

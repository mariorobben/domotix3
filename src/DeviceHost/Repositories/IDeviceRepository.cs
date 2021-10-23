using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DeviceHost.Devices;

namespace DeviceHost.DevicesConfiguration
{
    public interface IDeviceRepository
    {
        bool TryGetDevice<TDevice>(string deviceName, out TDevice device) where TDevice : class, IDevice;
    }

    public sealed class DeviceRepository : IDeviceRepository
    {
        public void RegisterDevice<TDevice>(TDevice device) where TDevice : class, IDevice
        {
            _devices.Add(device.DeviceName, device);
        }

        public bool TryGetDevice<TDevice>(string deviceName, out TDevice device) where TDevice : class, IDevice
        {
            _devices.TryGetValue(deviceName, out IDevice foundDevice);
            device = foundDevice as TDevice;
            return device is not null;
        }

        private IDictionary<string, IDevice> _devices = new Dictionary<string, IDevice>();
    }
}

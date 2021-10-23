using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using JsonSubTypes;
using Newtonsoft.Json;

namespace DeviceHost.DevicesConfiguration
{
    [JsonConverter(typeof(JsonSubtypes), "Type")]
    [JsonSubtypes.KnownSubType(typeof(WagoDeviceConfiguration), "wago")]
    [JsonSubtypes.KnownSubType(typeof(MilightDeviceConfiguration), "milight")]
    public interface IDeviceConfiguration
    {
        string Type { get; }
        string Name { get; }
    }

    public class WagoDeviceConfiguration : IDeviceConfiguration
    {
        public string Type { get; } = "wago";
        public string Name { get; set; }
        public List<string> Modules { get; set; }
    }

    public class MilightDeviceConfiguration : IDeviceConfiguration
    {
        public string Type { get; } = "milight";
        public string Name { get; set; }
    }

    public class DevicesConfiguration
    {
        public List<IDeviceConfiguration> Devices { get; set; }
    }
}

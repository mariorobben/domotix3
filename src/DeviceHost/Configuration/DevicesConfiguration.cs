using System.Collections.Generic;
using JsonSubTypes;
using Newtonsoft.Json;

namespace DeviceHost.Configuration
{
    [JsonConverter(typeof(JsonSubtypes), "Type")]
    [JsonSubtypes.KnownSubType(typeof(WagoDeviceConfiguration), "wago")]
    [JsonSubtypes.KnownSubType(typeof(MilightDeviceConfiguration), "milight")]
    public interface IDeviceConfiguration
    {
        string Type { get; }
        string Name { get; }
    }

    public sealed class WagoDeviceConfiguration : IDeviceConfiguration
    {
        public string Type { get; } = "wago";
        public string Name { get; set; }
        public List<string> Modules { get; set; }
    }

    public sealed class MilightDeviceConfiguration : IDeviceConfiguration
    {
        public string Type { get; } = "milight";
        public string Name { get; set; }
    }
}

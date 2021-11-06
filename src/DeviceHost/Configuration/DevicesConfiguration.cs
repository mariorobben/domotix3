using System.Collections.Generic;
using Domotix3.Devices.Wago;
using JsonSubTypes;
using Newtonsoft.Json;

namespace DeviceHost.Configuration
{
    [JsonConverter(typeof(JsonSubtypes), "Type")]
    [JsonSubtypes.KnownSubType(typeof(WagoDeviceConfiguration), WagoDeviceConfiguration.TypeName)]
    [JsonSubtypes.KnownSubType(typeof(MiLightDeviceConfiguration), MiLightDeviceConfiguration.TypeName)]
    public interface IDeviceConfiguration
    {
        string Type { get; }
        string Name { get; }
    }

    public sealed class WagoDeviceConfiguration : IDeviceConfiguration
    {
        public const string TypeName = "wago";

        public string Type { get; } = TypeName;
        public string Name { get; set; }
        public List<IWagoModule> Modules { get; set; }
    }

    [JsonConverter(typeof(JsonSubtypes), "Type")]
    [JsonSubtypes.KnownSubType(typeof(Wago_750_430_Module), Wago_750_430_Module.TypeName)]
    [JsonSubtypes.KnownSubType(typeof(Wago_750_530_Module), Wago_750_530_Module.TypeName)]
    [JsonSubtypes.KnownSubType(typeof(Wago_750_559_Module), Wago_750_559_Module.TypeName)]
    public interface IWagoModule
    {
        public string Type { get; }
    }

    public sealed class Wago_750_430_Module : IWagoModule
    {
        public const string TypeName = "750-430";

        public string Type { get; } = TypeName;
    }

    public sealed class Wago_750_530_Module : IWagoModule
    {
        public const string TypeName = "750-530";

        public string Type { get; } = TypeName;
    }

    public sealed class Wago_750_559_Module : IWagoModule
    {
        public const string TypeName = "750-559";

        public string Type { get; } = TypeName;
    }

    public sealed class MiLightDeviceConfiguration : IDeviceConfiguration
    {
        public const string TypeName = "milight";

        public string Type { get; } = TypeName;
        public string Name { get; set; }
    }
}

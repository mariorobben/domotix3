using System.Collections.Generic;
using JsonSubTypes;
using Newtonsoft.Json;

namespace DeviceHost.InfrastructureConfiguration
{
    public sealed class GroupConfiguration
    {
        public string Name { get; set; }
        public List<GroupConfiguration> Groups { get; set; }
        public List<IElementConfiguration> Elements { get; set; }
    }

    [JsonConverter(typeof(JsonSubtypes), "Type")]
    [JsonSubtypes.KnownSubType(typeof(BinaryInputElementConfiguration), "binaryInput")]
    [JsonSubtypes.KnownSubType(typeof(BinaryOutputElementConfiguration), "binaryOutput")]
    [JsonSubtypes.KnownSubType(typeof(AnalogOutputElementConfiguration), "analogOutput")]
    [JsonSubtypes.KnownSubType(typeof(RgbLedElementConfiguration), "rgbLed")]
    public interface IElementConfiguration
    {
        string Type { get; }
        string Name { get; }
    }

    public sealed class BinaryInputElementConfiguration : IElementConfiguration
    {
        public string Type { get; } = "binaryInput";
        public string Name { get; set; }
        public int Input { get; set; }
    }

    public sealed class BinaryOutputElementConfiguration : IElementConfiguration
    {
        public string Type { get; } = "binaryOutput";
        public string Name { get; set; }
        public int Output { get; set; }
    }

    public sealed class AnalogOutputElementConfiguration : IElementConfiguration
    {
        public string Type { get; } = "analogOutput";
        public string Name { get; set; }
        public int Output { get; set; }
    }

    public sealed class RgbLedElementConfiguration : IElementConfiguration
    {
        public string Type { get; } = "rgbLed";
        public string Name { get; set; }
        public int Zone { get; set; }
    }
}

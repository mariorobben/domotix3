using System.Collections.Generic;
using System.Runtime.Serialization;
using DeviceHost.Core;
using DeviceHost.Elements;
using JsonSubTypes;
using Newtonsoft.Json;

namespace DeviceHost.Configuration
{
    public sealed class InfrastructureConfiguration
    {
        public List<GroupConfiguration> Groups { get; set; }
    }

    public sealed class GroupConfiguration
    {
        public string Name { get; set; }
        public List<GroupConfiguration> Groups { get; set; }
        public List<IElementConfiguration> Elements { get; set; }
    }

    [JsonConverter(typeof(JsonSubtypes), "Type")]
    [JsonSubtypes.KnownSubType(typeof(BinaryInputElementConfiguration), BinaryInputElementConfiguration.TypeName)]
    [JsonSubtypes.KnownSubType(typeof(BinaryOutputElementConfiguration), BinaryOutputElementConfiguration.TypeName)]
    [JsonSubtypes.KnownSubType(typeof(AnalogOutputElementConfiguration), AnalogOutputElementConfiguration.TypeName)]
    [JsonSubtypes.KnownSubType(typeof(RgbElementConfiguration), RgbElementConfiguration.TypeName)]
    public interface IElementConfiguration
    {
        string Type { get; }
        string Name { get; }
    }

    public sealed class BinaryInputElementConfiguration : IElementConfiguration
    {
        public const string TypeName = "binaryInput";

        public string Type { get; } = TypeName;
        public string Name { get; set; }
        public int Input { get; set; }
        public bool Invert { get; set; }
    }

    public sealed class BinaryOutputElementConfiguration : IElementConfiguration
    {
        public const string TypeName = "binaryOutput";

        public string Type { get; } = TypeName;
        public string Name { get; set; }
        public int Output { get; set; }
    }

    public sealed class AnalogOutputElementConfiguration : IElementConfiguration
    {
        public const string TypeName = "analogOutput";

        public string Type { get; } = TypeName;
        public string Name { get; set; }
        public int Output { get; set; }
        public IElementReference Reference { get; set; }
    }

    public sealed class RgbElementConfiguration : IElementConfiguration
    {
        public const string TypeName = "rgb";

        public string Type { get; } = TypeName;
        public string Name { get; set; }
        public int Zone { get; set; }
    }
}

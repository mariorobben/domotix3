using System.Collections.Generic;

namespace DeviceHost.Configuration
{
    public sealed class ProjectConfiguration
    {
        public List<IDeviceConfiguration> Devices { get; set; }
        public InfrastructureConfiguration Infrastructure { get; set; }
    }
}

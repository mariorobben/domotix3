using System.Collections.Generic;
using DeviceHost.InfrastructureConfiguration;

namespace DeviceHost.Configuration
{
    public sealed class ProjectConfiguration
    {
        public List<IDeviceConfiguration> Devices { get; set; }
        public List<GroupConfiguration> Infrastructure { get; set; }
    }
}

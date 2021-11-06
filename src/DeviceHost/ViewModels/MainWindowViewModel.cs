using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DeviceHost.Configuration;
using DeviceHost.DevicesConfiguration;

namespace DeviceHost.ViewModels
{
    class MainWindowViewModel
    {
        public MainWindowViewModel(IDeviceRepository deviceRepository, ProjectConfiguration projectConfiguration)
        {
            Project = new ProjectViewModel(deviceRepository, projectConfiguration.Infrastructure);
        }

        public ProjectViewModel Project { get; }
    }
}

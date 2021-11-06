using System;
using System.Collections.Generic;
using System.Linq;
using DeviceHost.Configuration;
using DeviceHost.Core;
using DeviceHost.Devices;
using DeviceHost.Devices.Wago;
using DeviceHost.DevicesConfiguration;
using DeviceHost.Elements;
using DeviceHost.Engine.MiLight;
using DeviceHost.ViewModels.MiLight;

namespace DeviceHost.ViewModels
{

    public class ProjectViewModel : ViewModelBase
    {
        public ProjectViewModel(IDeviceRepository deviceRepository, InfrastructureConfiguration infrastructureConfiguration)
        {
            foreach (var device in deviceRepository.GetAllDevices())
            {
                DeviceViewModel deviceViewModel = null;

                if (device is WagoDevice wagoDevice)
                {
                    deviceViewModel = new WagoViewModel(wagoDevice);
                }
                else if (device is MiLightDevice miLightDevice)
                {
                    deviceViewModel = new MiLightViewModel(miLightDevice);
                }
                
                if (deviceViewModel is null)
                {
                    throw new InvalidOperationException("Unknown device.");
                }

                _deviceViewModels.Add(deviceViewModel);
            }

            _infrastructureViewModel = new InfrastructureViewModel(this, infrastructureConfiguration);
        }

        public IEnumerable<Object> Pages
        {
            get
            {
                foreach (var device in _deviceViewModels)
                {
                    yield return new { Caption = device.Device.DeviceName, Content = device };
                }

                yield return new { Caption = "Infrastructure", Content = _infrastructureViewModel };
            }
        }

        public Object GetElementViewModel(IElementReference elementReference)
        {
            return _deviceViewModels.Select(d => d.QueryElementViewModel(elementReference)).First(o => o is not null);
        }
        
        private InfrastructureViewModel _infrastructureViewModel;
        private IList<DeviceViewModel> _deviceViewModels = new List<DeviceViewModel>();
    }
}

using DeviceHost.Core;
using DeviceHost.Elements;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DeviceHost.Devices;
using DeviceHost.Engine.MiLight;

namespace DeviceHost.ViewModels.MiLight
{
    public class MiLightViewModel : DeviceViewModel
    {
        public MiLightViewModel(Engine.MiLight.MiLightDevice device)
            : base(device)
        {
            using (var provider = new MiLightElementViewModelProvider(this))
            {
                var zones = new List<ZoneViewModel>();
                Int32 zoneNumber = 0;
                foreach (var zone in device.Zones)
                {
                    var zoneViewModel = new ZoneViewModel(zone);
                    zoneViewModel.Name = zoneNumber.ToString();
                    zones.Add(zoneViewModel);
                    zoneNumber++;

                    MiLightElementViewModelProvider.Current.AddZone(zoneViewModel);
                }
                _zones = zones.ToArray();

                _elementViewModels = provider.ElementViewModels;
            }
        }

        public ZoneViewModel[] Zones
        {
            get
            {
                return _zones;
            }
        }

        #region IElementViewModelRepository implementation

        public override object QueryElementViewModel(IElementReference element)
        {
            Object objFound;
            if (_elementViewModels.TryGetValue(element, out objFound) == true)
            {
                return objFound;
            }
            return null;
        }

        #endregion

        private ZoneViewModel[] _zones;
        
        private IDictionary<IElementReference, Object> _elementViewModels;
    }
}

using DeviceHost.Core;
using DeviceHost.ViewModels.MiLight;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DeviceHost.ElementReferences;

namespace DeviceHost.Engine.MiLight
{
    class MiLightElementViewModelProvider : IDisposable
    {
        [ThreadStatic]
        private static MiLightElementViewModelProvider _instance;

        public static MiLightElementViewModelProvider Current { get { return _instance; } }

        public MiLightElementViewModelProvider(MiLightViewModel milight)
        {
            _milight = milight;

            _elementViewModels = new Dictionary<IElementReference, Object>();
            _numberZones = 0;

            _instance = this;
        }

        public MiLightViewModel MiLight { get { return _milight; } }

        public void AddZone(Object value)
        {
            _elementViewModels.Add(new RgbElementReference(_numberZones++), value);
        }

        public IDictionary<IElementReference, Object> ElementViewModels { get { return _elementViewModels; } }

        public void Dispose()
        {
            _instance = null;
        }

        private MiLightViewModel _milight;
        private IDictionary<IElementReference, Object> _elementViewModels;
        private Int32 _numberZones;
    }
}

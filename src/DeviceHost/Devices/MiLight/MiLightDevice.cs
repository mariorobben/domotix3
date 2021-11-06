using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using DeviceHost.Devices;
using Domotix3.Devices.MiLight;

namespace DeviceHost.Engine.MiLight
{
    public class MiLightDevice : IMiLight, IDevice
    {
        #region Constructors

        public MiLightDevice(string name)
        {
            _name = name;

            var channels = new List<Zone>();
            for (Int32 i = 0; i < Constants.NumberZones; i++)
            {
                var channel = new Zone(this);
                channels.Add(channel);
            }
            _zones = channels.ToArray();
            _SelectedZone = null;
        }

        #endregion

        public String Name
        {
            get { return _name; }
        }

        public IEnumerable<Zone> Zones
        {
            get
            {
                return _zones;
            }
        }

        #region IMiLight implementation

        public void AllZonesOn()
        {
            _zones.ToList().ForEach(ch => ch.IsOn = true);
            _SelectedZone = null;
        }

        public void AllZonesOff()
        {
            _zones.ToList().ForEach(ch => ch.IsOn = false);
            _SelectedZone = null;
        }

        public void ZoneOn(Domotix3.Devices.MiLight.Zone zone)
        {
            _zones[zone.Id].IsOn = true;
            _SelectedZone = zone;
        }

        public void ZoneOff(Domotix3.Devices.MiLight.Zone zone)
        {
            _zones[zone.Id].IsOn = false;
            _SelectedZone = zone;
        }

        public void AllZonesWhite()
        {
            _zones.ToList().ForEach(ch => { ch.IsWhite = true; ch.IsDisco = false; });
            _SelectedZone = null;
        }

        public void ZoneWhite(Domotix3.Devices.MiLight.Zone zone)
        {
            _zones[zone.Id].IsWhite = true;
            _zones[zone.Id].IsDisco = false;
            _SelectedZone = zone;
        }

        public void Disco()
        {
            if (_SelectedZone.HasValue)
            {
                _zones[_SelectedZone.Value.Id].IsDisco = true;
                _zones[_SelectedZone.Value.Id].IsWhite = false;
            }
            else
            {
                _zones.ToList().ForEach(ch => { ch.IsDisco = true; ch.IsWhite = false; });
            }
        }

        public void DiscoIncSpeed()
        {
        }

        public void DiscoDecSpeed()
        {
        }

        public void Brightness(Brightness brightness)
        {
            if (_SelectedZone.HasValue)
            {
                _zones[_SelectedZone.Value.Id].Brightness = brightness.Level;
            }
            else
            {
                _zones.ToList().ForEach(ch => { ch.Brightness = brightness.Level; });
            }
        }

        public void Hue(byte hue)
        {
            if (_SelectedZone.HasValue)
            {
                _zones[_SelectedZone.Value.Id].Hue = hue;
                _zones[_SelectedZone.Value.Id].IsDisco = false;
                _zones[_SelectedZone.Value.Id].IsWhite = false;
            }
            else
            {
                _zones.ToList().ForEach(ch => { ch.Hue = hue; ch.IsDisco = false; ch.IsWhite = false; });
            }
        }

        #endregion

        #region IDevice implementation

        public string DeviceName
        {
            get { return _name; }
        }

        #endregion

        private String _name;
        private Zone[] _zones;
        private Domotix3.Devices.MiLight.Zone? _SelectedZone;
    }

    public class Zone : INotifyPropertyChanged
    {
        public Zone(MiLightDevice device)
        {
            _device = device;
            _brightness = Constants.MaxBrightnessLevels - 1;
            _isOn = false;
            _isWhite = false;
            _isDisco = false;
            _hue = 0;
        }

        public MiLightDevice Device
        {
            get
            {
                return _device;
            }
        }

        public Boolean IsOn
        {
            get
            {
                return _isOn;
            }

            set
            {
                if (_isOn != value)
                {
                    _isOn = value;
                    NotifyPropertyChanged(nameof(IsWhite));
                    NotifyPropertyChanged(nameof(IsDisco));
                    NotifyPropertyChanged(nameof(IsOn));
                }
            }
        }

        public Boolean IsWhite
        {
            get
            {
                return _isWhite & _isOn;
            }

            set
            {
                if (_isWhite != value)
                {
                    _isWhite = value;
                    NotifyPropertyChanged(nameof(IsWhite));
                }
            }
        }

        public Boolean IsDisco
        {
            get
            {
                return _isDisco & _isOn;
            }

            set
            {
                if (_isDisco != value)
                {
                    _isDisco = value;
                    NotifyPropertyChanged(nameof(IsDisco));
                }
            }
        }

        public Byte Brightness
        {
            get
            {
                return _brightness;
            }

            set
            {
                if (_brightness != value)
                {
                    _brightness = value;
                    NotifyPropertyChanged(nameof(Brightness));
                }
            }
        }

        public Byte Hue
        {
            get
            {
                return _hue;
            }

            set
            {
                if (_hue != value)
                {
                    _hue = value;
                    NotifyPropertyChanged(nameof(Hue));
                }
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void NotifyPropertyChanged(String info)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(info));
            }
        }

        private MiLightDevice _device;
        private bool _isOn;
        private bool _isWhite;
        private bool _isDisco;
        private byte _brightness;
        private byte _hue;
    }
}

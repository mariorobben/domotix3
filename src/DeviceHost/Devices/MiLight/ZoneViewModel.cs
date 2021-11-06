using System;
using DeviceHost.Engine.MiLight;

namespace DeviceHost.ViewModels.MiLight
{
    public class ZoneViewModel : ViewModelBase
    {
        public ZoneViewModel(Zone zone)
        {
            _zone = zone;
            _zone.PropertyChanged += _channel_PropertyChanged;
        }

        void _channel_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            RaisePropertyChanged(e.PropertyName);
        }

        public String Name
        {
            get
            {
                return _name;
            }
            set
            {
                _name = value;
                RaisePropertyChanged(() => this.Name);
            }
        }

        public Boolean IsOn
        {
            get
            {
                return _zone.IsOn;
            }
        }

        public Boolean IsWhite
        {
            get
            {
                return _zone.IsWhite;
            }
        }

        public Boolean IsDisco
        {
            get
            {
                return _zone.IsDisco;
            }
        }

        public Byte Brightness
        {
            get
            {
                return _zone.Brightness;
            }
        }

        public Byte Hue
        {
            get
            {
                return _zone.Hue;
            }
        }

        private Zone _zone;
        private string _name;
    }
}

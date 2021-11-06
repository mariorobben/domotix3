using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DeviceHost.ViewModels;

namespace DeviceHost.Devices.Wago
{
    public class AnalogOutputViewModel : ViewModelBase
    {
        public AnalogOutputViewModel()
        {
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

        public UInt16 Value
        {
            get
            {
                return _value;
            }

            set
            {
                if (_value != value)
                {
                    _value = value;
                    RaisePropertyChanged(() => this.Value);
                    RaisePropertyChanged(() => this.Voltage);
                }
            }
        }

        public Double Voltage
        {
            get
            {
                if (_value == 32767)
                    return 10.0;
                else
                    return (Double)_value * 10.0 / 32768.0;
            }
        }

        private String _name;
        private UInt16 _value;
    }
}

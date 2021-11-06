using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DeviceHost.ViewModels;

namespace DeviceHost.Devices.Wago
{
    public class BinaryOutputViewModel : ViewModelBase
    {
        public BinaryOutputViewModel()
        {
            _outputActivated = false;
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

        public Boolean OutputActivated
        {
            get
            {
                return _outputActivated;
            }

            set
            {
                _outputActivated = value;
                RaisePropertyChanged(() => this.OutputActivated);
            }
        }

        private String _name;
        private Boolean _outputActivated;
    }
}

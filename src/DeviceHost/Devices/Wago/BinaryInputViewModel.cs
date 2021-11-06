using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using DeviceHost.ViewModels;

namespace DeviceHost.Devices.Wago
{
    public class BinaryInputViewModel : ViewModelBase
    {
        public BinaryInputViewModel()
        {
            _pushed = false;
            _toggled = false;
            _inputActivated = false;

            _pushDownCommand = new DelegateCommand(PushDown);
            _pushUpCommand = new DelegateCommand(PushUp);
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

        public Boolean Pushed
        {
            get
            {
                return _pushed;
            }

            set
            {
                if (_pushed != value)
                {
                    _pushed = value;
                    RaisePropertyChanged(() => this.Pushed);

                    ChangeInputActivated();
                }
            }
        }

        public Boolean Toggled
        {
            get
            {
                return _toggled;
            }

            set
            {
                if (_toggled != value)
                {
                    _toggled = value;
                    RaisePropertyChanged(() => this.Toggled);

                    ChangeInputActivated();
                }
            }
        }

        public Boolean InputActivated
        {
            get
            {
                return _inputActivated;
            }

            private set
            {
                _inputActivated = value;
                RaisePropertyChanged(() => this.InputActivated);
            }
        }

        private void PushDown()
        {
            Pushed = true;
        }

        private void PushUp()
        {
            Pushed = false;
        }

        private void ChangeInputActivated()
        {
            InputActivated = (_pushed == true && _toggled == false) || (_pushed == false && _toggled == true);
        }

        public ICommand ToggleCommand { get { return _toggleCommand; } }
        public ICommand PushDownCommand { get { return _pushDownCommand; } }
        public ICommand PushUpCommand { get { return _pushUpCommand; } }

        private DelegateCommand _pushDownCommand;
        private DelegateCommand _pushUpCommand;
        private DelegateCommand _toggleCommand;

        private String _name;
        private Boolean _pushed;
        private Boolean _toggled;
        private Boolean _inputActivated;
    }
}

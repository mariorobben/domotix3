using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DeviceHost.Core;
using DeviceHost.Elements;
using DeviceHost.ViewModels;

namespace DeviceHost.Devices
{
    public abstract class DeviceViewModel : ViewModelBase, IElementViewModelQueryable
    {
        protected DeviceViewModel(IDevice device)
        {
            Device = device;
        }

        public abstract object QueryElementViewModel(IElementReference element);

        public IDevice Device { get; }
    }
}

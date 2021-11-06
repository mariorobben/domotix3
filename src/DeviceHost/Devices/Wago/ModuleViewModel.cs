using System;
using System.Collections.Generic;
using System.Linq;
using DeviceHost.ViewModels;

namespace DeviceHost.Devices.Wago
{
    public abstract class ElementsViewModelBase : ViewModelBase
    {
        public abstract IEnumerable<object> Elements { get; }
    }

    public abstract class ModuleViewModel<TModule> : ElementsViewModelBase where TModule : IModule
    {
        protected ModuleViewModel(TModule module, String name)
            : base()
        {
            _module = module;
            _name = name;
        }

        public TModule Module { get { return _module; } }
        public String Name { get { return _name; } }

        private TModule _module;
        private String _name;
    }
}

using DeviceHost.Core;
using DeviceHost.Elements;
using DeviceHost.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DeviceHost.Configuration;
using DeviceHost.Controllers;
using DeviceHost.ElementReferences;

namespace DeviceHost.Devices.Wago
{
    public class WagoViewModel : DeviceViewModel
    {
        public WagoViewModel(WagoDevice wago)
            : base(wago)
        {
            int numberBinaryInputs = 0;
            int numberBinaryOutputs = 0;
            int numberAnalogOutputs = 0;

            foreach (var module in wago.Modules)
            {
                ElementsViewModelBase moduleViewModel = null;

                if (module is Module_750_430 module750430)
                {
                    moduleViewModel = new Module_750_430_ViewModel(module750430);
                }
                else if (module is Module_750_530 module750530)
                {
                    moduleViewModel = new Module_750_530_ViewModel(module750530);
                }
                else if (module is Module_750_559 module750559)
                {
                    moduleViewModel = new Module_750_559_ViewModel(module750559);
                }
                
                _modules.Add(moduleViewModel);

                foreach (var element in moduleViewModel.Elements)
                {
                    if (element is BinaryInputViewModel binaryInputViewModel)
                    {
                        _elementViewModels.Add(new BinaryInputElementReference(numberBinaryInputs++), element);
                    }
                    else if (element is BinaryOutputViewModel binaryOutputViewModel)
                    {
                        _elementViewModels.Add(new BinaryOutputElementReference(numberBinaryOutputs++), element);
                    }
                    else if (element is AnalogOutputViewModel analogOutputViewModel)
                    {
                        _elementViewModels.Add(new AnalogOutputElementReference(numberAnalogOutputs++), element);
                    }
                }
            }
        }
        
        public IEnumerable<ViewModelBase> Modules
        {
            get
            {
                return _modules;
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

        private IList<ViewModelBase> _modules = new List<ViewModelBase>();
        private IDictionary<IElementReference, Object> _elementViewModels = new Dictionary<IElementReference, object>();
    }
}

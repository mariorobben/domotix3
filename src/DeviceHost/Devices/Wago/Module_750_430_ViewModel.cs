using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;

namespace DeviceHost.Devices.Wago
{
    public class Module_750_430_ViewModel : ModuleViewModel<Module_750_430>
    {
        public Module_750_430_ViewModel(Module_750_430 module)
            : base(module, "8-Channel Digital Input Module DC 24 V")
        {
            _bitPosition = new Dictionary<BinaryInputViewModel, Int32>();
            _binaryInputs = new List<BinaryInputViewModel>();
            for (Int32 i = 0; i < 8; i++)
            {
                var binaryInput = new BinaryInputViewModel();
                binaryInput.Name = (i + 1).ToString();
                binaryInput.PropertyChanged += BinaryInput_PropertyChanged;
                _binaryInputs.Add(binaryInput);
                _bitPosition.Add(binaryInput, i);
            }
        }

        void BinaryInput_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            Int32 bitPosition;
            if (_bitPosition.TryGetValue((BinaryInputViewModel)sender, out bitPosition) == true)
            {
                Module.InputBits[bitPosition] = _binaryInputs[bitPosition].InputActivated;
            }
        }

        public IEnumerable<BinaryInputViewModel> BinaryInputs => _binaryInputs;

        public override IEnumerable<object> Elements => _binaryInputs;

        private IList<BinaryInputViewModel> _binaryInputs;
        private IDictionary<BinaryInputViewModel, Int32> _bitPosition;
    }
}

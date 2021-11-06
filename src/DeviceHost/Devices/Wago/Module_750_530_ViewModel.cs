using System;
using System.Collections.Generic;
using System.Reflection.Metadata.Ecma335;

namespace DeviceHost.Devices.Wago
{
    public class Module_750_530_ViewModel : ModuleViewModel<Module_750_530>
    {
        public Module_750_530_ViewModel(Module_750_530 module)
            : base(module, "8-Channel Digital Output Module DC 24 V")
        {
            var binaryOutputs = new List<BinaryOutputViewModel>();
            for (Int32 i = 0; i < 8; i++)
            {
                var binaryOutput = new BinaryOutputViewModel();
                binaryOutput.Name = (i + 1).ToString();
                binaryOutputs.Add(binaryOutput);
            }
            _binaryOutputs = binaryOutputs.ToArray();

            Module.OutputsChanged += Module_OutputChanged;
        }

        void Module_OutputChanged(object sender, EventArgs e)
        {
            for (Int32 i = 0; i < 8; i++)
            {
                _binaryOutputs[i].OutputActivated = Module.OutputBits[i];
            }
        }

        public IEnumerable<BinaryOutputViewModel> BinaryOutputs => _binaryOutputs;

        public override IEnumerable<object> Elements
        {
            get => _binaryOutputs;
        }

        private BinaryOutputViewModel[] _binaryOutputs;
    }
}

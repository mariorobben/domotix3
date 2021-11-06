using System;
using System.Collections.Generic;
using System.Net;

namespace DeviceHost.Devices.Wago
{
    public class Module_750_559_ViewModel : ModuleViewModel<Module_750_559>
    {
        public Module_750_559_ViewModel(Module_750_559 module)
            : base(module, "4-Channel Analog Output Module ±10V/0-10 V")
        {
            var analogOutputs = new List<AnalogOutputViewModel>();
            for (Int32 i = 0; i < 4; i++)
            {
                var analogOutput = new AnalogOutputViewModel();
                analogOutput.Name = (i + 1).ToString();
                analogOutputs.Add(analogOutput);
            }
            _analogOutputs = analogOutputs.ToArray();

            Module.OutputsChanged += Module_OutputChanged;
        }

        void Module_OutputChanged(object sender, EventArgs e)
        {
            for (Int32 i = 0; i < 4; i++)
            {
                _analogOutputs[i].Value = (UInt16)IPAddress.NetworkToHostOrder(BitConverter.ToInt16(Module.OutputBytes, i * 2));
            }
        }

        public IEnumerable<AnalogOutputViewModel> AnalogOutputs => _analogOutputs;

        public override IEnumerable<object> Elements
        {
            get => _analogOutputs;
        }

        private readonly AnalogOutputViewModel[] _analogOutputs;
    }
}

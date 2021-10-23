using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceHost.Devices.Wago
{
    public interface IModule
    {
        Byte[] InputBytes { get; }
        BitArray InputBits { get; }
        Byte[] OutputBytes { get; }
        BitArray OutputBits { get; }

        void NotifyOutputsChanged();
    }
}

using System;

namespace DeviceHost.Devices.Wago
{
    public interface IModuleFactory
    {
        String Type { get; }
        String Description { get; }
        Int32 NumberInputBytes { get; }
        Int32 NumberInputBits { get; }
        Int32 NumberOutputBytes { get; }
        Int32 NumberOutputBits { get; }

        IModule CreateModule();
    }
}

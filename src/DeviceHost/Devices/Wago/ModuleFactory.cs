using DeviceHost.Configuration;
using System;
using System.Collections.Generic;

namespace DeviceHost.Devices.Wago
{
    public static class ModuleFactory
    {
        public static IModule CreateModule(IWagoModule module)
        {
            if (module is Wago_750_430_Module)
            {
                return new Module_750_430();
            }
            else if (module is Wago_750_530_Module)
            {
                return new Module_750_530();
            }
            else if (module is Wago_750_559_Module)
            {
                return new Module_750_559();
            }

            throw new ArgumentException("Invalid module.");
        }
    }
}

using System;
using System.Collections.Generic;
using DeviceHost.Devices.Wago.Modules._750_430;

namespace DeviceHost.Devices.Wago
{
    public static class ModuleFactory
    {
        static ModuleFactory()
        {
            _moduleFactories = new Dictionary<String, IModuleFactory>();

            _moduleFactories.Add("750-430", new Module_750_430_Factory());
        }

        public static IModule CreateModule(String type)
        {
            return _moduleFactories[type].CreateModule();
        }

        private static IDictionary<string, IModuleFactory> _moduleFactories;
    }
}

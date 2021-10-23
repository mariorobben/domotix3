using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceHost.Devices.Wago
{
    public abstract class AbstractModuleFactory : IModuleFactory
    {
        protected AbstractModuleFactory(Type moduleType, String type, String description, Int16 numberInputBytes, Int16 numberInputBits, Int16 numberOutputBytes, Int16 numberOutputBits)
        {
            _moduleType = moduleType;

            Type = type;
            Description = description;
            NumberInputBytes = numberInputBytes;
            NumberInputBits = numberInputBits;
            NumberOutputBytes = numberOutputBytes;
            NumberOutputBits = numberOutputBits;
        }

        public IModule CreateModule()
        {
            return (IModule)Activator.CreateInstance(_moduleType);
        }

        public String Type { get; init; }
        public String Description { get; init; }
        public Int32 NumberInputBytes { get; init; }
        public Int32 NumberInputBits { get; init; }
        public Int32 NumberOutputBytes { get; init; }
        public Int32 NumberOutputBits { get; init; }

        private Type _moduleType;
    }
}

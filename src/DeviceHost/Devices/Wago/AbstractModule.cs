using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DeviceHost.Devices.Wago
{
    public abstract class AbstractModule : IModule
    {
        protected AbstractModule(int type, Int16 numberInputBytes, Int16 numberInputBits, Int16 numberOutputBytes, Int16 numberOutputBits, int channels, int piFormat)
        {
            Type = type;
            _inputBytes = numberInputBytes > 0 ? new Byte[numberInputBytes] : null;
            _inputBits = numberInputBits > 0 ? new BitArray(numberInputBits) : null;
            _outputBytes = numberOutputBytes > 0 ? new Byte[numberOutputBytes] : null;
            _outputBits = numberOutputBits > 0 ? new BitArray(numberOutputBits) : null;
            Channels = channels;
            PiFormat = piFormat;
        }

        public int Type { get; }

        public Byte[] InputBytes
        {
            get { return _inputBytes; }
        }

        public BitArray InputBits
        {
            get { return _inputBits; }
        }

        public byte[] OutputBytes
        {
            get { return _outputBytes; }
        }

        public BitArray OutputBits
        {
            get { return _outputBits; }
        }

        public int Channels { get; }

        public int PiFormat { get; }

        public void NotifyOutputsChanged()
        {
            EventHandler outputsChanged = OutputsChanged;
            if (outputsChanged != null)
            {
                outputsChanged(this, new EventArgs());
            }
        }

        public event EventHandler OutputsChanged;
        
        private readonly String _description;
        protected readonly Byte[] _inputBytes;
        protected readonly BitArray _inputBits;
        protected readonly Byte[] _outputBytes;
        protected readonly BitArray _outputBits;
    }
}

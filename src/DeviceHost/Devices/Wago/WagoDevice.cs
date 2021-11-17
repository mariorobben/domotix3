using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using DeviceHost.Configuration;
using Domotix3.Devices.Wago;

namespace DeviceHost.Devices.Wago
{
    public sealed class WagoDevice : AbstractDevice, IWago
    {
        public WagoDevice(string deviceName, IReadOnlyCollection<IWagoModule> modules)
            : base(deviceName)
        {
            _modules = modules.Select(module => ModuleFactory.CreateModule(module)).ToList();
            _terminals = new List<Terminal>(_modules.Count);

            var byteInputOffset = 0;
            var byteOutputOffset = 0;
            _binaryInputOffset = _modules.Sum(m => m.InputBytes?.Length ?? 0) * 8;
            var bitInputOffset = _binaryInputOffset;
            _binaryOutputOffset = _modules.Sum(m => m.OutputBytes?.Length ?? 0) * 8;
            var bitOutputOffset = _binaryOutputOffset;
            _bitCount = 0;

            foreach (var module in _modules)
            {
                var bitSizeOut = module.OutputBytes?.Length * 8 ?? 0 + module.OutputBits?.Length ?? 0;
                var bitSizeIn = module.InputBytes?.Length * 8 ?? 0 + module.InputBits?.Length ?? 0;
                _bitCount += (bitSizeIn + bitSizeOut);

                _terminals.Add(new Terminal(_terminals.Count, module.Type, bitSizeOut == 0 ? 0 : module.OutputBytes is null ? bitOutputOffset : byteOutputOffset, bitSizeOut, bitSizeIn == 0 ? 0 : module.InputBytes is null ? bitInputOffset : byteInputOffset, bitSizeIn, module.Channels, module.PiFormat));

                bitInputOffset += module.InputBits?.Length ?? 0;
                byteInputOffset += module.InputBytes?.Length ?? 0;
                bitOutputOffset += module.OutputBits?.Length ?? 0;
                byteOutputOffset += module.OutputBytes?.Length ?? 0;
            }

            _inputImage = new Byte[Constants.IoSize];
            _outputImage = new Byte[Constants.IoSize];

            _numberBinaryInputs = _modules.Sum(m => m.InputBits?.Length ?? 0);
            _numberBinaryOutputs = _modules.Sum(m => m.OutputBits?.Length ?? 0);
        }
        
        public Status GetStatus()
        {
            return new Status(_bitCount, _modules.Count,_binaryInputOffset, _binaryOutputOffset, _numberBinaryInputs, _numberBinaryOutputs);
        }

        public IReadOnlyCollection<Terminal> GetTerminals()
        {
            return _terminals.ToList().AsReadOnly();
        }

        public void WriteRegion(WriteRegion region)
        {
            Array.Copy(region.Bytes, 0, _outputImage, region.Offset, region.Bytes.Length);

            WriteOutputImage(_outputImage);
        }

        public byte[] ReadRegion(ReadRegion region)
        {
            var inputImage = ReadInputImage();
            Array.Copy(inputImage, _inputImage, Math.Min(inputImage.Length, _inputImage.Length));

            var regionResult = new byte[region.Size];
            Array.Copy(_inputImage, region.Offset, regionResult, 0, region.Size);
            return regionResult;
        }

        public IEnumerable<IModule> Modules
        {
            get
            {
                return _modules;
            }
        }

        #region Private

        private Byte[] ReadInputImage()
        {
            var result = new List<Byte>();
            var allInputBits = new BitArray(_numberBinaryInputs);
            var inputBitOffset = 0;
            foreach (var module in _modules)
            {
                var inputBytes = module.InputBytes;
                if (inputBytes != null)
                {
                    result.AddRange(inputBytes);
                }
                var inputBits = module.InputBits;
                if (inputBits != null)
                {
                    for (Int32 j = 0; j < inputBits.Length; j++)
                    {
                        allInputBits.Set(inputBitOffset++, inputBits[j]);
                    }
                }
            }
            Byte[] inputBitsbytes = new Byte[allInputBits.Length + 7 / 8];
            allInputBits.CopyTo(inputBitsbytes, 0);
            result.AddRange(inputBitsbytes);
            return result.ToArray();
        }

        private void WriteOutputImage(Byte[] image)
        {
            Int32 outputByteOffset = 0;
            Int32 outputBitOffset = 0;
            Int32 minimumLength = _binaryOutputOffset/8 + (_numberBinaryOutputs + 7) / 8;
            if (image.Length < minimumLength)
            {
                var enlargedImage = new Byte[minimumLength];
                Buffer.BlockCopy(image, 0, enlargedImage, 0, image.Length);
                image = enlargedImage;
            }
            var allOutputBits = new BitArray(image.Skip(_binaryOutputOffset/8).ToArray());

            foreach (var module in _modules)
            {
                var outputBytes = module.OutputBytes;
                if (outputBytes != null && outputBytes.Length > 0)
                {
                    Buffer.BlockCopy(image, outputByteOffset, outputBytes, 0, outputBytes.Length);
                    outputByteOffset += outputBytes.Length;

                    module.NotifyOutputsChanged();
                }

                var outputBits = module.OutputBits;
                if (outputBits != null && outputBits.Length > 0)
                {
                    for (Int32 j = 0; j < outputBits.Length; j++)
                    {
                        outputBits.Set(j, allOutputBits[outputBitOffset++]);
                    }

                    module.NotifyOutputsChanged();
                }
            }
        }

        #endregion

        #region Fields

        private readonly string _name;
        private readonly IList<IModule> _modules;
        private readonly IList<Terminal> _terminals;
        private readonly Byte[] _inputImage;
        private readonly Byte[] _outputImage;

        private int _bitCount = 0;
        private int _binaryInputOffset = 0;
        private int _binaryOutputOffset = 0;
        private int _numberBinaryInputs = 0;
        private int _numberBinaryOutputs = 0;

        #endregion
    }
}

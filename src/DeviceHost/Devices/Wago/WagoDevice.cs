using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Domotix3.Devices.Wago;

namespace DeviceHost.Devices.Wago
{
    public sealed class WagoDevice : AbstractDevice, IWago
    {
        public WagoDevice(string deviceName, IReadOnlyCollection<string> moduleTypes)
            : base(deviceName)
        {
            _modules = moduleTypes.Select(moduleType => ModuleFactory.CreateModule(moduleType)).ToList();
            _terminals = new List<Terminal>(_modules.Count);

            _binaryInputOffset = 0;
            _numberBinaryInputs = 0;
            _binaryOutputOffset = 0;
            _numberBinaryOutputs = 0;

            foreach (var module in _modules)
            {
                _terminals.Add(new Terminal(_terminals.Count, 0, _binaryInputOffset, 0, 0, 0, 0, 0));

                _binaryInputOffset += (module.InputBytes != null ? module.InputBytes.Length : 0);
                _numberBinaryInputs += (module.InputBits != null ? module.InputBits.Length : 0);
                _binaryOutputOffset += (module.OutputBytes != null ? module.OutputBytes.Length : 0);
                _numberBinaryOutputs += (module.OutputBits != null ? module.OutputBits.Length : 0);
            }

            _inputImage = new Byte[Constants.IoSize];
            _outputImage = new Byte[Constants.IoSize];
        }
        
        public Status GetStatus()
        {
            return new Status(0, _modules.Count, _binaryInputOffset * 8, _binaryOutputOffset * 8, _numberBinaryInputs, _numberBinaryOutputs);
        }

        public IReadOnlyCollection<Terminal> GetTerminals()
        {
            return _terminals.ToList().AsReadOnly();
        }

        public void WriteRegions(IReadOnlyCollection<WriteRegion> regions)
        {
            foreach (var region in regions)
            {
                Array.Copy(region.Bytes, 0, _outputImage, region.Offset, region.Bytes.Length);
            }

            WriteOutputImage(_outputImage);
        }

        public IReadOnlyCollection<byte[]> ReadRegions(IReadOnlyCollection<ReadRegion> regions)
        {
            var result = new List<Byte[]>();

            var inputImage = ReadInputImage();
            Array.Copy(inputImage, _inputImage, Math.Min(inputImage.Length, _inputImage.Length));

            foreach (var region in regions)
            {
                var regionResult = new byte[region.Size];
                Array.Copy(_inputImage, region.Offset, regionResult, 0, region.Size);
                result.Add(regionResult);
            }

            return result;
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
            Int32 minimumLength = _binaryOutputOffset + (_numberBinaryOutputs + 7) / 8;
            if (image.Length < minimumLength)
            {
                var enlargedImage = new Byte[minimumLength];
                Buffer.BlockCopy(image, 0, enlargedImage, 0, image.Length);
                image = enlargedImage;
            }
            var allOutputBits = new BitArray(image.Skip(_binaryOutputOffset).ToArray());

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
        private readonly Int32 _binaryInputOffset;
        private readonly Int32 _binaryOutputOffset;
        private readonly Int32 _numberBinaryInputs;
        private readonly Int32 _numberBinaryOutputs;
        private readonly Byte[] _inputImage;
        private readonly Byte[] _outputImage;

        #endregion
    }
}

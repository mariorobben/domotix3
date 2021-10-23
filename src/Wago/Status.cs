namespace Domotix3.Devices.Wago
{
    public sealed class Status
    {
        public Status(int bitCount, int terminalCount, int bitCountAnalogInput, int bitCountAnalogOutput, int bitCountDigitalInput, int bitCountDigitalOutput)
        {
            BitCount = bitCount;
            TerminalCount = terminalCount;
            BitCountAnalogInput = bitCountAnalogInput;
            BitCountAnalogOutput = bitCountAnalogOutput;
            BitCountDigitalInput = bitCountDigitalInput;
            BitCountDigitalOutput = bitCountDigitalOutput;
        }

        public int BitCount { get; init; }
        public int TerminalCount { get; init; }
        public int BitCountAnalogInput { get; init; }
        public int BitCountAnalogOutput { get; init; }
        public int BitCountDigitalInput { get; init; }
        public int BitCountDigitalOutput { get; init; }
    }
}

namespace Domotix3.Devices.Wago
{
    public sealed class Terminal
    {
        public Terminal(int position, int type, int bitOffsetOut, int bitSizeOut, int bitOffsetIn, int bitSizeIn, int channels, int piFormat)
        {
            Position = position;
            Type = type;
            BitOffsetOut = bitOffsetOut;
            BitSizeOut = bitSizeOut;
            BitOffsetIn = bitOffsetIn;
            BitSizeIn = bitSizeIn;
            Channels = channels;
            PiFormat = piFormat;
        }

        public int Position { get; init; }
        public int Type { get; init; }
        public int BitOffsetOut { get; init; }
        public int BitSizeOut { get; init; }
        public int BitOffsetIn { get; init; }
        public int BitSizeIn { get; init; }
        public int Channels { get; init; }
        public int PiFormat { get; init; }
    }
}

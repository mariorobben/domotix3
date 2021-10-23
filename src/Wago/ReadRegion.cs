namespace Domotix3.Devices.Wago
{
    public sealed class ReadRegion
    {
        public ReadRegion(int offset, int size)
        {
            Offset = offset;
            Size = size;
        }

        public int Offset { get; init; }

        public int Size { get; init; }
    }
}

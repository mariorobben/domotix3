namespace Domotix3.Devices.MiLight
{
    public sealed class Hue
    {
        public Hue(byte value)
        {
            Value = value;
        }

        public byte Value { get; init; }
    }
}
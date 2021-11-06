using System;

namespace Domotix3.Devices.MiLight
{

    public struct Brightness : IComparable<Brightness>
    {
        public Brightness(Byte level)
        {
            if (level < 0 || level >= Constants.MaxBrightnessLevels)
            {
                throw new ArgumentOutOfRangeException("id");
            }

            _level = (Byte)level;
        }

        public Byte Level
        {
            get
            {
                return _level;
            }
        }

        public int CompareTo(Brightness other)
        {
            return _level - other.Level;
        }

        public override string ToString()
        {
            return String.Format("brightness {0}", _level);
        }

        private Byte _level;
    }
}

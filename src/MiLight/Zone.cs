using System;

namespace Domotix3.Devices.MiLight
{

    public struct Zone : IComparable<Zone>
    {
        public Zone(Byte id)
        {
            if (id < 0 || id >= Constants.NumberZones)
            {
                throw new ArgumentOutOfRangeException("id");
            }

            _id = (Byte)id;
        }

        public Byte Id
        {
            get
            {
                return _id;
            }
        }

        public int CompareTo(Zone other)
        {
            return _id - other.Id;
        }

        public override string ToString()
        {
            return String.Format("zone {0}", _id);
        }

        private Byte _id;
    }
}

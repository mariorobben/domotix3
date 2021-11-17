using System;

namespace Domotix3.Devices.MiLight
{
    public interface IMiLight
    {
        void AllZonesOn();
        void AllZonesOff();
        void ZoneOn(Zone zone);
        void ZoneOff(Zone zone);
        void AllZonesWhite();
        void ZoneWhite(Zone zone);
        void Disco();
        void DiscoIncSpeed();
        void DiscoDecSpeed();
        void Brightness(Brightness brightness);
        void Hue(Hue hue);
    }
}

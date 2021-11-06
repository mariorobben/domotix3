using System;
using System.Windows.Media;

namespace DeviceHost.Engine.MiLight
{
    public static class ColorHelperMethods
    {
        public static Color ColorFromHSV(double hue, double saturation, double value)
        {
            int hi = Convert.ToInt32(Math.Floor(hue / 60)) % 6;
            double f = hue / 60 - Math.Floor(hue / 60);

            value = value * 255;
            int v = Convert.ToInt32(value);
            int p = Convert.ToInt32(value * (1 - saturation));
            int q = Convert.ToInt32(value * (1 - f * saturation));
            int t = Convert.ToInt32(value * (1 - (1 - f) * saturation));

            if (hi == 0)
                return Color.FromArgb(255, (Byte)v, (Byte)t, (Byte)p);
            else if (hi == 1)
                return Color.FromArgb(255, (Byte)q, (Byte)v, (Byte)p);
            else if (hi == 2)
                return Color.FromArgb(255, (Byte)p, (Byte)v, (Byte)t);
            else if (hi == 3)
                return Color.FromArgb(255, (Byte)p, (Byte)q, (Byte)v);
            else if (hi == 4)
                return Color.FromArgb(255, (Byte)t, (Byte)p, (Byte)v);
            else
                return Color.FromArgb(255, (Byte)v, (Byte)p, (Byte)q);
        }
    }
}

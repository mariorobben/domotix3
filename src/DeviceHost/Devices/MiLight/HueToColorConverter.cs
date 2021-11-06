using System;
using System.Windows.Data;
using System.Windows.Media;
using Domotix3.Devices.MiLight;

namespace DeviceHost.Engine.MiLight
{
    public class HueToColorConverter : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            double h = (Byte)values[0] / .256;
            double v = ((Byte)values[2] + 1) / (double)Constants.MaxBrightnessLevels;
            return new SolidColorBrush(ColorHelperMethods.ColorFromHSV(h, (Boolean)values[1] == false ? 1 : 0, (Boolean)values[1] == false ? v : 1));
        }

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, System.Globalization.CultureInfo culture)
        {
            return null;
        }
    }
}

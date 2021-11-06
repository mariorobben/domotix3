using System;
using System.Windows.Data;
using System.Windows.Media;

namespace DeviceHost.ValueConverters
{
    public class BooleanToColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return ((Boolean)value == true) ? new SolidColorBrush(Colors.Lime) : new SolidColorBrush(Colors.Black);
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return null;
        }
    }
}

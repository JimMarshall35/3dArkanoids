using _3dArkanoidsEditor.Models;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Media;

namespace _3dArkanoidsEditor.utils.converters
{
    class GameBlockTypeToBrushConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var input = (GameBlockType)value;
            var brush = new SolidColorBrush(input.Colour.ToWpfColor());
            return brush;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var input = (SolidColorBrush)value;
            var r = (float)input.Color.R / 255.0f;
            var g = (float)input.Color.G / 255.0f;
            var b = (float)input.Color.B / 255.0f;
            var a = (float)input.Color.A / 255.0f;

            return new GameBlockType(0, r, g, b, a);

        }
    }
}

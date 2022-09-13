using _3dArkanoidsEditor.Models;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace _3dArkanoidsEditor.utils.converters
{
    class SerializablePropertyTypeEnumToStringConvertor : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if(parameter != null)
            {
                return  $"({Enum.GetName(typeof(SerializablePropertyType), value)}) {(string) parameter}";
            }
            return Enum.GetName(typeof(SerializablePropertyType), value);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var input = (string)value;

            return Enum.Parse(typeof(SerializablePropertyType), input);
        }
    }
}

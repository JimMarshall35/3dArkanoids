using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using _3dArkanoidsEditor.ViewModels;

namespace _3dArkanoidsEditor.utils.converters
{
    class BoardEditorToolTypeToIsButtonEnabledConvertor : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return ((BoardEditorToolType)value) != ((BoardEditorToolType)parameter);
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}

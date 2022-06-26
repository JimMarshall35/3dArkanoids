using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace _3dArkanoidsEditor.utils
{
    static class ColorExtensionMethods
    {
        public static Color WithAlpha(this Color color, byte alpha)
        {
            var c = new Color();
            c.R = color.R;
            c.G = color.G;
            c.B = color.B;
            c.A = alpha;
            return c;
        }
    }
}

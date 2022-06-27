using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace _3dArkanoidsEditor.Models
{
    public class Colour
    {
        public Colour(float r, float g, float b, float a)
        {
            Red = r;
            Green = g;
            Blue = b;
        }
        public float Red { get; private set; }
        public float Green { get; private set; }
        public float Blue { get; private set; }
        public float Alpha { get; private set; }
        public Color ToWpfColor()
        {
            byte r = (byte)(Red * 255.0);
            byte g = (byte)(Green * 255.0);
            byte b = (byte)(Blue * 255.0);

            return Color.FromArgb(0xff,r, g, b);
        }
    }
}

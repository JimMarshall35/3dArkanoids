using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.Models
{
    public class SingleTileEdit
    {
        public SingleTileEdit(int xPos, int yPos, int zPos, byte newValue, byte oldValue)
        {
            XPos = xPos;
            YPos = yPos;
            ZPos = zPos;
            NewValue = newValue;
            OldValue = oldValue;
        }

        public int XPos { get; private set; }
        public int YPos { get; private set; }
        public int ZPos { get; private set; }
        public byte NewValue { get; private set; }
        public byte OldValue { get; private set; }

    }
}

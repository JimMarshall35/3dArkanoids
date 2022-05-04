using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.Models
{
    internal class GameBoardDescription
    {
        public GameBoardDescription(List<byte> playfieldArray, int width, int height, int depth)
        {
            PlayfieldArray = playfieldArray;
            Width = width;
            Height = height;
            Depth = depth;
        }
        public List<byte> PlayfieldArray { get; private set; }
        public int Width { get; private set; }
        public int Height { get; private set; }
        public int Depth { get; private set; }

    }
}

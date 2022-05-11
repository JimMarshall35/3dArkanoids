using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.Models
{
    public class GameBoardDescription
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

        #region helpers
        public byte GetAt(int x, int y, int z)
        {
            return PlayfieldArray[x * Height * Depth + y * Depth + z];
        }
        public GameBoardDescription SetAt(int x, int y, int z, byte newValue)
        {
            PlayfieldArray[x * Height * Depth + y * Depth + z] = newValue;
            return new GameBoardDescription(PlayfieldArray, Width, Height, Depth);
        }
        #endregion

    }
}

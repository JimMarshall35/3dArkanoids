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
        private void SetAtBase(int x, int y, int z, byte newValue)
        {
            PlayfieldArray[x * Height * Depth + y * Depth + z] = newValue;
        }
        public GameBoardDescription SetAt(int x, int y, int z, byte newValue)
        {
            SetAtBase(x, y, z, newValue);
            return new GameBoardDescription(PlayfieldArray, Width, Height, Depth);
        }

        public GameBoardDescription FloodFill(int x, int y, int z, byte newVal)
        {

            var indexQ = new Queue<int>();
            var indexOfCoords = x * Height * Depth + y * Depth + z;
            indexQ.Enqueue(x * Height * Depth + y * Depth + z);
            var valueToReplace = PlayfieldArray[indexOfCoords];
            while (indexQ.Count > 0)
            {
                var front = indexQ.Dequeue();
                if(PlayfieldArray[front] != valueToReplace)
                {
                    continue;
                }

                PlayfieldArray[front] = newVal;
                int topindex, bottomindex, leftindex, rightindex;
                topindex = front - Depth;
                bottomindex = front + Depth;
                leftindex = front - Height * Depth;
                rightindex = front + Height * Depth;
                if (topindex >= 0)
                {
                    indexQ.Enqueue(topindex);
                }
                if (bottomindex < PlayfieldArray.Count)
                {
                    indexQ.Enqueue(bottomindex);
                }
                //test
                if (leftindex >= 0)
                    indexQ.Enqueue(leftindex);
                if (rightindex < PlayfieldArray.Count)
                    indexQ.Enqueue(rightindex);
            }

            return new GameBoardDescription(PlayfieldArray, Width, Height, Depth);
        }

        #endregion

    }
}

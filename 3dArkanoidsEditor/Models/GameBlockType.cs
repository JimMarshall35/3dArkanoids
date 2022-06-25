using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.Models
{
    /// <summary>
    /// A type of block in the game.
    /// </summary>
    internal class GameBlockType
    {
        public GameBlockType(byte gameBlockTypeCode, float r, float g, float b)
        {
            GameBlockTypeCode = gameBlockTypeCode;
            Colour = new Colour(r, g, b);
        }

        /// <summary>
        /// the code used by the game to represent this type of block
        /// </summary>
        public byte GameBlockTypeCode { get; private set; }

        /// <summary>
        /// Colour of the block in game.
        /// </summary>
        public Colour Colour { get; private set; }
    }
}

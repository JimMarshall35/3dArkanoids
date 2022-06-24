using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.Models
{
    internal class GameSettings
    {
        public GameSettings(List<GameBlockType> gameBlockTypes)
        {
            GameBlockTypes = gameBlockTypes;
        }
        public List<GameBlockType> GameBlockTypes { get; private set; }
    }
}

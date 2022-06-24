using _3dArkanoidsEditor.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.Services
{
    internal class GameConnectionAquiredEventArgs
    {
        public GameConnectionAquiredEventArgs(GameBoardDescription gameBoardDescription, GameSettings gameSettings)
        {
            GameBoardDescription = gameBoardDescription;
            GameSettings = gameSettings;
        }
        public GameBoardDescription GameBoardDescription { get; private set; }
        public GameSettings GameSettings { get; private set; }
    }
}

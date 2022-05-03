using _3dArkanoidsEditor.Models;

namespace _3dArkanoidsEditor.Services
{
    internal delegate void GameConnectionAquiredHandler(object sender, GameBoardDescription e);

    internal delegate void GameConnectionLostHandler(object sender, GameConnectionChangedArgs e);

    internal interface IGameConnectionService
    {
        event GameConnectionAquiredHandler GameConnectionAquired;
        event GameConnectionLostHandler GameConnectionLost;

        void Connect();
        IGameEditorClient Client { get; }
    }
}
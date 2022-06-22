using _3dArkanoidsEditor.Models;
using System.Threading;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.Services
{
    internal delegate void GameConnectionAquiredHandler(object sender, GameBoardDescription e);

    internal delegate void GameConnectionLostHandler(object sender, GameConnectionChangedArgs e);

    internal interface IGameConnectionService
    {
        event GameConnectionAquiredHandler GameConnectionAquired;
        event GameConnectionLostHandler GameConnectionLost;

        Task Connect(CancellationToken ct);
        IGameEditorClient Client { get; }
    }
}
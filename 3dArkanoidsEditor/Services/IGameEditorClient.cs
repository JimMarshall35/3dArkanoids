using _3dArkanoidsEditor.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using _3dArkanoidsEditor.Models;
using System.Threading;

namespace _3dArkanoidsEditor.Services
{
    public delegate void SetNewDescription(Models.GameBoardDescription newDescription);
    interface IGameEditorClient
    {
        Task<GameBoardDescription> GetBoardStateAsync();
        Task<EditBlockResult> ChangeBlockAsync(SingleTileEdit edit);
        Task<GameSettings> InitialConnectionHandshakeAsync(ClientInfo info);
        Task<List<SerializablePropertiesNode>> GetSerializableNodesAsync();
        Task<SetSerializablePropertyResult> SetSerializablePropertyAsync(SerializableProperty prop, string path);
        Task<SetNewBoardStateResult> SetBoardStateAsync(GameBoardDescription description);
        Task GameBoardStream(CancellationToken ct, SetNewDescription setNewDescription);
    }
}

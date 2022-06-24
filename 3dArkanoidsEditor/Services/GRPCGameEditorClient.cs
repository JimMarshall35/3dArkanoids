using _3dArkanoidsEditor.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PlayBoardEditClient = EditorGRPC.PlayBoardEdit.PlayBoardEditClient;

namespace _3dArkanoidsEditor.Services
{
    internal class GRPCGameEditorClient : IGameEditorClient
    {
        public GRPCGameEditorClient(PlayBoardEditClient client)
        {
            m_client = client;
        }
        public async Task<GameBoardDescription> GetBoardStateAsync()
        {
            var BoardState = await m_client.GetBoardStateAsync(new EditorGRPC.Void());
            return BoardState.ToModel();
        }

        public async Task<EditBlockResult> ChangeBlockAsync(SingleTileEdit edit)
        {
            var grpc = edit.ToGRPCMessage();
            var result = await m_client.ChangeBlockAsync(grpc);
            return result.ToModel();
        }

        public async Task<GameSettings> InitialConnectionHandshakeAsync(ClientInfo info)
        {
            var result = await m_client.InitialConnectionHandshakeAsync(new EditorGRPC.ClientInfo());
            return result.ToModel();
        }
        private PlayBoardEditClient m_client;
    }
}

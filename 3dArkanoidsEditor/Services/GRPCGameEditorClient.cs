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
        private PlayBoardEditClient m_client;
    }
}

using _3dArkanoidsEditor.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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

        public async Task<List<SerializablePropertiesNode>> GetSerializableNodesAsync()
        {
            var result = await m_client.GetSerializableNodesAsync(new EditorGRPC.Void());
            return result.ToModel();
        }

        public async Task<SetSerializablePropertyResult> SetSerializablePropertyAsync(SerializableProperty prop, string path)
        {
            var result = await m_client.SetSerializablePropertyAsync(prop.ToGRPCSetMessage(path));
            switch (result.Result)
            {
                case EditorGRPC.SetSerializablePropertyResult.Types.Result.Success:
                    return SetSerializablePropertyResult.Success;
                case EditorGRPC.SetSerializablePropertyResult.Types.Result.Failure:
                    return SetSerializablePropertyResult.Failure;
                default:
                    return SetSerializablePropertyResult.Failure;
            }
        }

        public async Task<SetNewBoardStateResult> SetBoardStateAsync(GameBoardDescription description)
        {
            var result = await m_client.SetBoardStateAsync(description.ToGRPCMessage());
            switch (result.Result)
            {
                case EditorGRPC.SetBoardDescriptionResult.Types.Result.Success:
                    return SetNewBoardStateResult.SUCCESS;
                    break;
                case EditorGRPC.SetBoardDescriptionResult.Types.Result.FailureTooManyBytesForWhd:
                    return SetNewBoardStateResult.FAILURE_TOO_MANY_BYTES_FOR_WHD;
                    break;
                case EditorGRPC.SetBoardDescriptionResult.Types.Result.FailureTooFewBytesForWhd:
                    return SetNewBoardStateResult.FAILURE_TOO_FEW_BYTES_FOR_WHD;
                    break;

            }
            return SetNewBoardStateResult.OTHER_FAILURE;
        }

        public async Task GameBoardStream(CancellationToken ct, SetNewDescription setNewDescription)
        {
            
            using (var call = m_client.GetUpdatedBoardStream(new EditorGRPC.Void()))
            {
                while (await call.ResponseStream.MoveNext(ct))
                {
                    var board = call.ResponseStream.Current;
                    setNewDescription(board.ToModel());
                }
            }
        }

        private PlayBoardEditClient m_client;
    }
}

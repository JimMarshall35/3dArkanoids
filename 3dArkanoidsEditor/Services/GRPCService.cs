using Grpc.Net.Client;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.Services
{

    internal class GRPCService : IGameConnectionService
    {
        public IGameEditorClient Client { get; private set; }
        private GrpcChannel m_channel;
        public event GameConnectionAquiredHandler GameConnectionAquired;
        public event GameConnectionLostHandler GameConnectionLost;

        public async Task Connect()
        {
            m_channel = GrpcChannel.ForAddress("http://localhost:50051");
            Client = new GRPCGameEditorClient(
                    new EditorGRPC.PlayBoardEdit.PlayBoardEditClient(m_channel)
                );
            var result = await Client.GetBoardStateAsync();
            // need to do this a better way using m_channel.State
            GameConnectionAquired(this,result);
        }
    }
}

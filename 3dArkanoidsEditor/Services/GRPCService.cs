﻿using _3dArkanoidsEditor.utils;
using Grpc.Net.Client;
using System;
using System.Threading;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.Services
{

    internal class GRPCService : IGameConnectionService
    {
        public IGameEditorClient Client { get; private set; }
        private GrpcChannel m_channel;
        public event GameConnectionAquiredHandler GameConnectionAquired;
        public event GameConnectionLostHandler GameConnectionLost;

        public async Task Connect(CancellationToken ct)
        {
            try
            {
                m_channel = GrpcChannel.ForAddress("http://localhost:50051");
                Client = new GRPCGameEditorClient(
                        new EditorGRPC.PlayBoardEdit.PlayBoardEditClient(m_channel)
                    );
                var settings = await Client.InitialConnectionHandshakeAsync(new Models.ClientInfo())
                    .WaitOrCancel(ct);

                var board = await Client.GetBoardStateAsync()
                    .WaitOrCancel(ct);

                GameConnectionAquired(this, new GameConnectionAquiredEventArgs(board, settings));
            }
            catch (OperationCanceledException)
            {
                GameConnectionLost(this, new GameConnectionLostEventArgs("attempt to connect timed out"));
            }
            catch (Exception e)
            {
                GameConnectionLost(this, new GameConnectionLostEventArgs(e.Message));
            }
        }
    }
}

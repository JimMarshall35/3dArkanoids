#pragma once
#include <memory>
#include <thread>
#include "ILevelEditorServer.h"
#include "ILevelEditorServerGame.h"
#include "GRPCCallData.h"
#include "Array3d.h"
#include "TSQueue.h"
#include <functional>
#include <atomic>



using EditorGRPC::Void;
using EditorGRPC::BlockEdit;
using EditorGRPC::BoardDescription;
using EditorGRPC::Point;
using EditorGRPC::EditBlockResult;
using EditorGRPC::GameSettings;
using EditorGRPC::ClientInfo;
using EditorGRPC::SetSerializablePropertyData;
using EditorGRPC::SetSerializablePropertyResult;
using EditorGRPC::SetBoardDescriptionResult;
using EditorGRPC::BoardDescription;


using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;


namespace grpc {
    class Server;
};
class ILevelEditorServerGame;
//class CallData;
class ISerializable;






class GrpcLevelEditorServer :
    public ILevelEditorServer
{
public:
    ~GrpcLevelEditorServer();
    GrpcLevelEditorServer(ILevelEditorServerGame* game);
    void Run();
    TSQueue<std::function<void()>>& GetEditorQueue() { return m_editorQueue; };
    void SetConnectedFlag() { m_editorConnected = true; }
    bool GetConnectedFlag() { return m_editorConnected; }

private:
    bool m_editorConnected = false;
    ILevelEditorServerGame* m_game;
    std::unique_ptr<grpc::Server> m_server;
    ServerCompletionQueue* m_cq;
    EditorGRPC::PlayBoardEdit::AsyncService m_service;
    std::unique_ptr<std::thread> m_serverThread;
    TSQueue<std::function<void()>> m_editorQueue;

    std::atomic<bool> m_shouldServerContinue = true;
    std::atomic<bool> m_newBoardStateFlag = false;
    const Array3D<unsigned char>* m_newBoardState;
    


    // Inherited via ILevelEditorServer
    virtual void OnEvent(EngineUpdateFrameEventArgs e) override;


    // Inherited via ILevelEditorServer
    virtual void NotifyNewBoardState(const Array3D<unsigned char>& newState) override;

};


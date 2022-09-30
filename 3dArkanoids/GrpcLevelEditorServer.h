#pragma once
#include <memory>
#include <thread>
#include "ILevelEditorServer.h"
#include "ILevelEditorServerGame.h"
#include "GRPCCallData.h"
#include "Array3d.h"
#include "TSQueue.h"
#include <functional>


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
private:
    ILevelEditorServerGame* m_game;
    std::unique_ptr<grpc::Server> m_server;
    ServerCompletionQueue* m_cq;
    EditorGRPC::PlayBoardEdit::AsyncService m_service;
    std::thread* m_serverThread = nullptr;
    TSQueue<std::function<void()>> m_editorQueue;


    

    


    // Inherited via ILevelEditorServer
    virtual void OnEvent(EngineUpdateFrameEventArgs e) override;

};


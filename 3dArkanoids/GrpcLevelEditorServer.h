#pragma once
#include <memory>
#include <thread>
#include "ILevelEditorServer.h"
#include "ILevelEditorServerGame.h"
#include "PlayBoardEdit.grpc.pb.h"
#include "Array3d.h"
#include "TSQueue.h"



using EditorGRPC::Void;
using EditorGRPC::BoardDescription;
using EditorGRPC::Point;
using EditorGRPC::EditBlockResult;

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
class CallData;
enum RPCType : char {
    Invalid,
    AddBlock,
    RemoveBlock,
    GetBlocks
};
struct LevelEditorRPC {
    CallData* callData;
    RPCType rpcType;
};

class CallData {
public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    CallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game)
        : service_(service), cq_(cq), status_(CREATE), m_game(game) {
        
    }

    void Proceed() {
        if (status_ == CREATE) {
            // Make this instance progress to the PROCESS state.
            status_ = PROCESS;

            // As part of the initial CREATE state, we *request* that the system
            // start processing SayHello requests. In this request, "this" acts are
            // the tag uniquely identifying the request (so that different CallData
            // instances can serve different requests concurrently), in this case
            // the memory address of this CallData instance.
            RequestOnCreate();
        }
        else if (status_ == PROCESS) {
            OnProcess();
        }
        else {
            GPR_ASSERT(status_ == FINISH);
            // Once in the FINISH state, deallocate ourselves (CallData).
            delete this;
        }
    }

    virtual void Finish() = 0;

private:
    // Let's implement a tiny state machine with the following states.
protected:
    // The means of communication with the gRPC runtime for an asynchronous
    // server.
    EditorGRPC::PlayBoardEdit::AsyncService* service_;
    // The producer-consumer queue where for asynchronous server notifications.
    ServerCompletionQueue* cq_;
    // Context for the rpc, allowing to tweak aspects of it such as the use
    // of compression, authentication, as well as to send metadata back to the
    // client.
    ServerContext ctx_;

    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus status_;  // The current serving state.

    ILevelEditorServerGame* m_game;
protected:
    virtual void RequestOnCreate() = 0;
    virtual void OnProcess() = 0;
};




class GrpcLevelEditorServer :
    public ILevelEditorServer
{
public:
    ~GrpcLevelEditorServer();
    GrpcLevelEditorServer(ILevelEditorServerGame* game);
    void Run();
private:
    void EnactRPC(const LevelEditorRPC& edit);
private:
    ILevelEditorServerGame* m_game;
    std::unique_ptr<grpc::Server> m_server;
    ServerCompletionQueue* m_cq;
    EditorGRPC::PlayBoardEdit::AsyncService m_service;
    std::thread* m_serverThread = nullptr;
    TSQueue<LevelEditorRPC> m_editorQueue;

    class GetBoardStateCallData : CallData {
    public:
        GetBoardStateCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
            :CallData(service,cq,game), responder_(&ctx_), server_(server)
        {
            Proceed();
        }
    public:
        // What we get from the client.
        Void request_;
        // What we send back to the client.
        BoardDescription reply_;

    private:
        // The means to get back to the client.
        ServerAsyncResponseWriter<BoardDescription> responder_;

        GrpcLevelEditorServer* server_;
    protected:
        // Inherited via CallData
        virtual void RequestOnCreate() override;
        virtual void OnProcess() override;

        // Inherited via CallData
        virtual void Finish() override;
    };

    class AddBlockCallData : CallData {
    public:
        AddBlockCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
            :CallData(service, cq, game), responder_(&ctx_), server_(server)
        {
            Proceed();
        }
    private:
        // What we get from the client.
        Point request_;
        // What we send back to the client.
        EditBlockResult reply_;
        // The means to get back to the client.
        ServerAsyncResponseWriter<EditBlockResult> responder_;

        GrpcLevelEditorServer* server_;
    protected:
        // Inherited via CallData
        virtual void RequestOnCreate() override;
        virtual void OnProcess() override;

        // Inherited via CallData
        virtual void Finish() override;
    };

    class RemoveBlockCallData : CallData {
    public:
        RemoveBlockCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
            :CallData(service, cq, game), responder_(&ctx_), server_(server)
        {
            Proceed();
        }
    private:
        // What we get from the client.
        Point request_;
        // What we send back to the client.
        EditBlockResult reply_;
        // The means to get back to the client.
        ServerAsyncResponseWriter<EditBlockResult> responder_;

        GrpcLevelEditorServer* server_;
    protected:
        // Inherited via CallData
        virtual void RequestOnCreate() override;
        virtual void OnProcess() override;

        // Inherited via CallData
        virtual void Finish() override;
    };


    // Inherited via ILevelEditorServer
    virtual void OnEvent(EngineUpdateFrameEventArgs e) override;

};


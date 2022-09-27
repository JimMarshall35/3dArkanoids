#pragma once
#include <memory>
#include <thread>
#include "ILevelEditorServer.h"
#include "ILevelEditorServerGame.h"
#include "PlayBoardEdit.grpc.pb.h"
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

using ProtoBufSerializablePropertiesNode = EditorGRPC::SerializablePropertiesNode;
using ProtoBufSerializablePropertiesNodes = EditorGRPC::SerializablePropertiesNodes;
using ProtoBufSerializablePropertyData = EditorGRPC::SerializablePropertyData;

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
class ISerializable;

class CallData {
public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    CallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game)
        : m_service(service), m_cq(cq), m_status(CREATE), m_game(game) {
        
    }

    void Proceed() {
        if (m_status == CREATE) {
            // Make this instance progress to the PROCESS state.
            m_status = PROCESS;

            // As part of the initial CREATE state, we *request* that the system
            // start processing SayHello requests. In this request, "this" acts are
            // the tag uniquely identifying the request (so that different CallData
            // instances can serve different requests concurrently), in this case
            // the memory address of this CallData instance.
            RequestOnCreate();
        }
        else if (m_status == PROCESS) {
            OnProcess();
        }
        else {
            GPR_ASSERT(m_status == FINISH);
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
    EditorGRPC::PlayBoardEdit::AsyncService* m_service;
    // The producer-consumer queue where for asynchronous server notifications.
    ServerCompletionQueue* m_cq;
    // Context for the rpc, allowing to tweak aspects of it such as the use
    // of compression, authentication, as well as to send metadata back to the
    // client.
    ServerContext m_ctx;

    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus m_status;  // The current serving state.

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
    ILevelEditorServerGame* m_game;
    std::unique_ptr<grpc::Server> m_server;
    ServerCompletionQueue* m_cq;
    EditorGRPC::PlayBoardEdit::AsyncService m_service;
    std::thread* m_serverThread = nullptr;
    TSQueue<std::function<void()>> m_editorQueue;

    class GetBoardStateCallData : public CallData {
    public:
        GetBoardStateCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
            :CallData(service,cq,game), m_responder(&m_ctx), m_server(server)
        {
            Proceed();
        }
    public:
        // What we get from the client.
        Void m_request;
        // What we send back to the client.
        BoardDescription m_reply;

    private:
        // The means to get back to the client.
        ServerAsyncResponseWriter<BoardDescription> m_responder;

        GrpcLevelEditorServer* m_server;
    protected:
        // Inherited via CallData
        virtual void RequestOnCreate() override;
        virtual void OnProcess() override;

        // Inherited via CallData
        virtual void Finish() override;
    };

    class ChangeBlockCallData : public CallData {
    public:
        ChangeBlockCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
            :CallData(service, cq, game), m_responder(&m_ctx), m_server(server)
        {
            Proceed();
        }
    public:
        // What we get from the client.
        BlockEdit m_request;
        // What we send back to the client.
        EditBlockResult m_reply;

    private:
        // The means to get back to the client.
        ServerAsyncResponseWriter<EditBlockResult> m_responder;

        GrpcLevelEditorServer* m_server;
    protected:
        // Inherited via CallData
        virtual void RequestOnCreate() override;
        virtual void OnProcess() override;

        // Inherited via CallData
        virtual void Finish() override;
    };

    class InitialConnectionHandshakeCallData : public CallData {
    public:
        InitialConnectionHandshakeCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
            :CallData(service, cq, game), m_responder(&m_ctx), m_server(server)
        {
            Proceed();
        }
    public:
        // What we get from the client.
        ClientInfo m_request;
        // What we send back to the client.
        GameSettings m_reply;

    private:
        // The means to get back to the client.
        ServerAsyncResponseWriter<GameSettings> m_responder;

        GrpcLevelEditorServer* m_server;
    protected:
        // Inherited via CallData
        virtual void Finish() override;
        virtual void RequestOnCreate() override;
        virtual void OnProcess() override;
    };

    class GetSerializableNodesCallData : public CallData {
    public:
        GetSerializableNodesCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
            :CallData(service, cq, game), m_responder(&m_ctx), m_server(server)
        {
            Proceed();
        }
    public:
        // What we get from the client.
        Void m_request;
        // What we send back to the client.
        ProtoBufSerializablePropertiesNodes m_reply;

    private:
        // The means to get back to the client.
        ServerAsyncResponseWriter<ProtoBufSerializablePropertiesNodes> m_responder;

        GrpcLevelEditorServer* m_server;
    private:
        void ProcessSinglePropertiesNode(const ISerializablePropertiesNode* node, ProtoBufSerializablePropertiesNode* replyNode);
    protected:
        // Inherited via CallData
        virtual void Finish() override;
        virtual void RequestOnCreate() override;
        virtual void OnProcess() override;
    };

    class SetSerializablePropertyCallData : public CallData {
    public:
        SetSerializablePropertyCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
            :CallData(service, cq, game), m_responder(&m_ctx), m_server(server)
        {
            Proceed();
        }
    public:
        // What we get from the client.
        SetSerializablePropertyData m_request;
        // What we send back to the client.
        SetSerializablePropertyResult m_reply;

    private:
        // The means to get back to the client.
        ServerAsyncResponseWriter<SetSerializablePropertyResult> m_responder;

        GrpcLevelEditorServer* m_server;
    protected:
        // Inherited via CallData
        virtual void Finish() override;
        virtual void RequestOnCreate() override;
        virtual void OnProcess() override;
    };

    class SetBoardStateCallData : public CallData {
    public:
        SetBoardStateCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
            :CallData(service, cq, game), m_responder(&m_ctx), m_server(server)
        {
            Proceed();
        }
    public:
        // What we get from the client.
        BoardDescription m_request;
        // What we send back to the client.
        SetBoardDescriptionResult m_reply;

    private:
        // The means to get back to the client.
        ServerAsyncResponseWriter<SetBoardDescriptionResult> m_responder;

        GrpcLevelEditorServer* m_server;
    protected:
        // Inherited via CallData
        virtual void Finish() override;
        virtual void RequestOnCreate() override;
        virtual void OnProcess() override;
    };


    // Inherited via ILevelEditorServer
    virtual void OnEvent(EngineUpdateFrameEventArgs e) override;

};


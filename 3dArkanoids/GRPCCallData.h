#pragma once
#include "PlayBoardEdit.grpc.pb.h"
#include "IProceed.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;
class GrpcLevelEditorServer;
class ILevelEditorServerGame;

template<class RequestT, class ResponseT>
class GRPCCallData : IProceed {
public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    GRPCCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
        : m_service(service), m_cq(cq), m_status(CREATE), m_game(game), m_responder(&m_ctx), m_server(server){}

    void Proceed() override {
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

protected:

    enum CallStatus { CREATE, PROCESS, FINISH };
    // The means of communication with the gRPC runtime for an asynchronous
    // server.
    EditorGRPC::PlayBoardEdit::AsyncService* m_service;
    // The producer-consumer queue where for asynchronous server notifications.
    ServerCompletionQueue* m_cq;
    // Context for the rpc, allowing to tweak aspects of it such as the use
    // of compression, authentication, as well as to send metadata back to the
    // client.
    ServerContext m_ctx;
    CallStatus m_status;  // The current serving state.
    ServerAsyncResponseWriter<ResponseT> m_responder;
    ILevelEditorServerGame* m_game;
    RequestT m_request;
    ResponseT m_reply;
    GrpcLevelEditorServer* m_server;
protected:
    virtual void RequestOnCreate() = 0;
    virtual void OnProcess() = 0;
};


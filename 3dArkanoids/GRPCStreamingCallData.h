#pragma once
#include "PlayBoardEdit.grpc.pb.h"
#include "IProceed.h"
#include <grpcpp/alarm.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerAsyncWriter;

class GrpcLevelEditorServer;
class ILevelEditorServerGame;

/*
Class based on this article:
https://www.gresearch.co.uk/blog/article/lessons-learnt-from-writing-asynchronous-streaming-grpc-services-in-c/

Lessons learnt from writing asynchronous streaming gRPC services in C++
by Hong Hai Chu & Stefans Mezulis

*/
template<class RequestT, class ResponseT>
class GRPCStreamingCallData : IProceed {
public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    GRPCStreamingCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
        : m_service(service), m_cq(cq), m_status(CREATE), m_game(game), m_responder(&m_ctx), m_server(server) {}

    void Proceed() override {
        if (m_status == CREATE) {
            // Make this instance progress to the PROCESS state.
            m_status = PUSH_TO_BACK;

            // As part of the initial CREATE state, we *request* that the system
            // start processing SayHello requests. In this request, "this" acts are
            // the tag uniquely identifying the request (so that different CallData
            // instances can serve different requests concurrently), in this case
            // the memory address of this CallData instance.
            RequestOnCreate();
        }
        else if (m_status == PROCESS) {
            if (m_times++ == 0)
            {
                AllocateNew(m_service, m_cq, m_game, m_server);
            }
            //if (times_++ >= 3) // we want to send the response 3 times (for whatever reason)
            //{
            //    status_ = FINISH;
            //    responder_.Finish(Status::OK, this);
            //}
            OnProcess();
            m_status = PUSH_TO_BACK;
        }
        else if (m_status == PUSH_TO_BACK) {
            if (m_times++ == 0)
            {
                AllocateNew(m_service, m_cq, m_game, m_server);
            }
            //m_status = PROCESS;
            m_alarm.Set(m_cq, gpr_now(gpr_clock_type::GPR_CLOCK_REALTIME), this);

        }
        else {
            GPR_ASSERT(m_status == FINISH);
            // Once in the FINISH state, deallocate ourselves (CallData).
            delete this;
        }
    }

private:
    grpc::Alarm m_alarm;
protected:

    enum CallStatus { CREATE, PROCESS, FINISH, PUSH_TO_BACK };
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
    ServerAsyncWriter<ResponseT> m_responder;
    ILevelEditorServerGame* m_game;
    RequestT m_request;
    ResponseT m_reply;
    GrpcLevelEditorServer* m_server;
    int m_times = 0;
protected:
    virtual void RequestOnCreate() = 0;
    virtual void OnProcess() = 0;
    virtual GRPCStreamingCallData<RequestT, ResponseT>* AllocateNew(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server) = 0;
public:
    void PushNewData(const ResponseT& newData) {
        if (!m_server->GetConnectedFlag()) {
            return;
        }
        m_reply = newData;
        m_status = PROCESS;
    }
    void CloseStream() {
        m_status = FINISH;
        m_responder->Finish(Status::OK, this);
    }
};


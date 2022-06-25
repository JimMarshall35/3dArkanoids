// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: editor_proto_files/PlayBoardEdit.proto

#include "editor_proto_files/PlayBoardEdit.pb.h"
#include "editor_proto_files/PlayBoardEdit.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace EditorGRPC {

static const char* PlayBoardEdit_method_names[] = {
  "/EditorGRPC.PlayBoardEdit/AddBlock",
  "/EditorGRPC.PlayBoardEdit/RemoveBlock",
  "/EditorGRPC.PlayBoardEdit/ChangeBlock",
  "/EditorGRPC.PlayBoardEdit/GetBoardState",
  "/EditorGRPC.PlayBoardEdit/SetBoardState",
  "/EditorGRPC.PlayBoardEdit/InitialConnectionHandshake",
};

std::unique_ptr< PlayBoardEdit::Stub> PlayBoardEdit::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< PlayBoardEdit::Stub> stub(new PlayBoardEdit::Stub(channel, options));
  return stub;
}

PlayBoardEdit::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_AddBlock_(PlayBoardEdit_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_RemoveBlock_(PlayBoardEdit_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_ChangeBlock_(PlayBoardEdit_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetBoardState_(PlayBoardEdit_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_SetBoardState_(PlayBoardEdit_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_InitialConnectionHandshake_(PlayBoardEdit_method_names[5], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status PlayBoardEdit::Stub::AddBlock(::grpc::ClientContext* context, const ::EditorGRPC::Point& request, ::EditorGRPC::EditBlockResult* response) {
  return ::grpc::internal::BlockingUnaryCall< ::EditorGRPC::Point, ::EditorGRPC::EditBlockResult, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_AddBlock_, context, request, response);
}

void PlayBoardEdit::Stub::async::AddBlock(::grpc::ClientContext* context, const ::EditorGRPC::Point* request, ::EditorGRPC::EditBlockResult* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::EditorGRPC::Point, ::EditorGRPC::EditBlockResult, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_AddBlock_, context, request, response, std::move(f));
}

void PlayBoardEdit::Stub::async::AddBlock(::grpc::ClientContext* context, const ::EditorGRPC::Point* request, ::EditorGRPC::EditBlockResult* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_AddBlock_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::EditorGRPC::EditBlockResult>* PlayBoardEdit::Stub::PrepareAsyncAddBlockRaw(::grpc::ClientContext* context, const ::EditorGRPC::Point& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::EditorGRPC::EditBlockResult, ::EditorGRPC::Point, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_AddBlock_, context, request);
}

::grpc::ClientAsyncResponseReader< ::EditorGRPC::EditBlockResult>* PlayBoardEdit::Stub::AsyncAddBlockRaw(::grpc::ClientContext* context, const ::EditorGRPC::Point& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncAddBlockRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status PlayBoardEdit::Stub::RemoveBlock(::grpc::ClientContext* context, const ::EditorGRPC::Point& request, ::EditorGRPC::EditBlockResult* response) {
  return ::grpc::internal::BlockingUnaryCall< ::EditorGRPC::Point, ::EditorGRPC::EditBlockResult, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_RemoveBlock_, context, request, response);
}

void PlayBoardEdit::Stub::async::RemoveBlock(::grpc::ClientContext* context, const ::EditorGRPC::Point* request, ::EditorGRPC::EditBlockResult* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::EditorGRPC::Point, ::EditorGRPC::EditBlockResult, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_RemoveBlock_, context, request, response, std::move(f));
}

void PlayBoardEdit::Stub::async::RemoveBlock(::grpc::ClientContext* context, const ::EditorGRPC::Point* request, ::EditorGRPC::EditBlockResult* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_RemoveBlock_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::EditorGRPC::EditBlockResult>* PlayBoardEdit::Stub::PrepareAsyncRemoveBlockRaw(::grpc::ClientContext* context, const ::EditorGRPC::Point& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::EditorGRPC::EditBlockResult, ::EditorGRPC::Point, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_RemoveBlock_, context, request);
}

::grpc::ClientAsyncResponseReader< ::EditorGRPC::EditBlockResult>* PlayBoardEdit::Stub::AsyncRemoveBlockRaw(::grpc::ClientContext* context, const ::EditorGRPC::Point& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncRemoveBlockRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status PlayBoardEdit::Stub::ChangeBlock(::grpc::ClientContext* context, const ::EditorGRPC::BlockEdit& request, ::EditorGRPC::EditBlockResult* response) {
  return ::grpc::internal::BlockingUnaryCall< ::EditorGRPC::BlockEdit, ::EditorGRPC::EditBlockResult, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_ChangeBlock_, context, request, response);
}

void PlayBoardEdit::Stub::async::ChangeBlock(::grpc::ClientContext* context, const ::EditorGRPC::BlockEdit* request, ::EditorGRPC::EditBlockResult* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::EditorGRPC::BlockEdit, ::EditorGRPC::EditBlockResult, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ChangeBlock_, context, request, response, std::move(f));
}

void PlayBoardEdit::Stub::async::ChangeBlock(::grpc::ClientContext* context, const ::EditorGRPC::BlockEdit* request, ::EditorGRPC::EditBlockResult* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ChangeBlock_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::EditorGRPC::EditBlockResult>* PlayBoardEdit::Stub::PrepareAsyncChangeBlockRaw(::grpc::ClientContext* context, const ::EditorGRPC::BlockEdit& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::EditorGRPC::EditBlockResult, ::EditorGRPC::BlockEdit, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_ChangeBlock_, context, request);
}

::grpc::ClientAsyncResponseReader< ::EditorGRPC::EditBlockResult>* PlayBoardEdit::Stub::AsyncChangeBlockRaw(::grpc::ClientContext* context, const ::EditorGRPC::BlockEdit& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncChangeBlockRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status PlayBoardEdit::Stub::GetBoardState(::grpc::ClientContext* context, const ::EditorGRPC::Void& request, ::EditorGRPC::BoardDescription* response) {
  return ::grpc::internal::BlockingUnaryCall< ::EditorGRPC::Void, ::EditorGRPC::BoardDescription, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetBoardState_, context, request, response);
}

void PlayBoardEdit::Stub::async::GetBoardState(::grpc::ClientContext* context, const ::EditorGRPC::Void* request, ::EditorGRPC::BoardDescription* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::EditorGRPC::Void, ::EditorGRPC::BoardDescription, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetBoardState_, context, request, response, std::move(f));
}

void PlayBoardEdit::Stub::async::GetBoardState(::grpc::ClientContext* context, const ::EditorGRPC::Void* request, ::EditorGRPC::BoardDescription* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetBoardState_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::EditorGRPC::BoardDescription>* PlayBoardEdit::Stub::PrepareAsyncGetBoardStateRaw(::grpc::ClientContext* context, const ::EditorGRPC::Void& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::EditorGRPC::BoardDescription, ::EditorGRPC::Void, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetBoardState_, context, request);
}

::grpc::ClientAsyncResponseReader< ::EditorGRPC::BoardDescription>* PlayBoardEdit::Stub::AsyncGetBoardStateRaw(::grpc::ClientContext* context, const ::EditorGRPC::Void& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetBoardStateRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status PlayBoardEdit::Stub::SetBoardState(::grpc::ClientContext* context, const ::EditorGRPC::BoardDescription& request, ::EditorGRPC::SetBoardDescriptionResult* response) {
  return ::grpc::internal::BlockingUnaryCall< ::EditorGRPC::BoardDescription, ::EditorGRPC::SetBoardDescriptionResult, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_SetBoardState_, context, request, response);
}

void PlayBoardEdit::Stub::async::SetBoardState(::grpc::ClientContext* context, const ::EditorGRPC::BoardDescription* request, ::EditorGRPC::SetBoardDescriptionResult* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::EditorGRPC::BoardDescription, ::EditorGRPC::SetBoardDescriptionResult, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SetBoardState_, context, request, response, std::move(f));
}

void PlayBoardEdit::Stub::async::SetBoardState(::grpc::ClientContext* context, const ::EditorGRPC::BoardDescription* request, ::EditorGRPC::SetBoardDescriptionResult* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_SetBoardState_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::EditorGRPC::SetBoardDescriptionResult>* PlayBoardEdit::Stub::PrepareAsyncSetBoardStateRaw(::grpc::ClientContext* context, const ::EditorGRPC::BoardDescription& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::EditorGRPC::SetBoardDescriptionResult, ::EditorGRPC::BoardDescription, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_SetBoardState_, context, request);
}

::grpc::ClientAsyncResponseReader< ::EditorGRPC::SetBoardDescriptionResult>* PlayBoardEdit::Stub::AsyncSetBoardStateRaw(::grpc::ClientContext* context, const ::EditorGRPC::BoardDescription& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncSetBoardStateRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status PlayBoardEdit::Stub::InitialConnectionHandshake(::grpc::ClientContext* context, const ::EditorGRPC::ClientInfo& request, ::EditorGRPC::GameSettings* response) {
  return ::grpc::internal::BlockingUnaryCall< ::EditorGRPC::ClientInfo, ::EditorGRPC::GameSettings, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_InitialConnectionHandshake_, context, request, response);
}

void PlayBoardEdit::Stub::async::InitialConnectionHandshake(::grpc::ClientContext* context, const ::EditorGRPC::ClientInfo* request, ::EditorGRPC::GameSettings* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::EditorGRPC::ClientInfo, ::EditorGRPC::GameSettings, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_InitialConnectionHandshake_, context, request, response, std::move(f));
}

void PlayBoardEdit::Stub::async::InitialConnectionHandshake(::grpc::ClientContext* context, const ::EditorGRPC::ClientInfo* request, ::EditorGRPC::GameSettings* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_InitialConnectionHandshake_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::EditorGRPC::GameSettings>* PlayBoardEdit::Stub::PrepareAsyncInitialConnectionHandshakeRaw(::grpc::ClientContext* context, const ::EditorGRPC::ClientInfo& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::EditorGRPC::GameSettings, ::EditorGRPC::ClientInfo, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_InitialConnectionHandshake_, context, request);
}

::grpc::ClientAsyncResponseReader< ::EditorGRPC::GameSettings>* PlayBoardEdit::Stub::AsyncInitialConnectionHandshakeRaw(::grpc::ClientContext* context, const ::EditorGRPC::ClientInfo& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncInitialConnectionHandshakeRaw(context, request, cq);
  result->StartCall();
  return result;
}

PlayBoardEdit::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      PlayBoardEdit_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< PlayBoardEdit::Service, ::EditorGRPC::Point, ::EditorGRPC::EditBlockResult, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](PlayBoardEdit::Service* service,
             ::grpc::ServerContext* ctx,
             const ::EditorGRPC::Point* req,
             ::EditorGRPC::EditBlockResult* resp) {
               return service->AddBlock(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      PlayBoardEdit_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< PlayBoardEdit::Service, ::EditorGRPC::Point, ::EditorGRPC::EditBlockResult, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](PlayBoardEdit::Service* service,
             ::grpc::ServerContext* ctx,
             const ::EditorGRPC::Point* req,
             ::EditorGRPC::EditBlockResult* resp) {
               return service->RemoveBlock(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      PlayBoardEdit_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< PlayBoardEdit::Service, ::EditorGRPC::BlockEdit, ::EditorGRPC::EditBlockResult, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](PlayBoardEdit::Service* service,
             ::grpc::ServerContext* ctx,
             const ::EditorGRPC::BlockEdit* req,
             ::EditorGRPC::EditBlockResult* resp) {
               return service->ChangeBlock(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      PlayBoardEdit_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< PlayBoardEdit::Service, ::EditorGRPC::Void, ::EditorGRPC::BoardDescription, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](PlayBoardEdit::Service* service,
             ::grpc::ServerContext* ctx,
             const ::EditorGRPC::Void* req,
             ::EditorGRPC::BoardDescription* resp) {
               return service->GetBoardState(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      PlayBoardEdit_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< PlayBoardEdit::Service, ::EditorGRPC::BoardDescription, ::EditorGRPC::SetBoardDescriptionResult, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](PlayBoardEdit::Service* service,
             ::grpc::ServerContext* ctx,
             const ::EditorGRPC::BoardDescription* req,
             ::EditorGRPC::SetBoardDescriptionResult* resp) {
               return service->SetBoardState(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      PlayBoardEdit_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< PlayBoardEdit::Service, ::EditorGRPC::ClientInfo, ::EditorGRPC::GameSettings, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](PlayBoardEdit::Service* service,
             ::grpc::ServerContext* ctx,
             const ::EditorGRPC::ClientInfo* req,
             ::EditorGRPC::GameSettings* resp) {
               return service->InitialConnectionHandshake(ctx, req, resp);
             }, this)));
}

PlayBoardEdit::Service::~Service() {
}

::grpc::Status PlayBoardEdit::Service::AddBlock(::grpc::ServerContext* context, const ::EditorGRPC::Point* request, ::EditorGRPC::EditBlockResult* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status PlayBoardEdit::Service::RemoveBlock(::grpc::ServerContext* context, const ::EditorGRPC::Point* request, ::EditorGRPC::EditBlockResult* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status PlayBoardEdit::Service::ChangeBlock(::grpc::ServerContext* context, const ::EditorGRPC::BlockEdit* request, ::EditorGRPC::EditBlockResult* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status PlayBoardEdit::Service::GetBoardState(::grpc::ServerContext* context, const ::EditorGRPC::Void* request, ::EditorGRPC::BoardDescription* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status PlayBoardEdit::Service::SetBoardState(::grpc::ServerContext* context, const ::EditorGRPC::BoardDescription* request, ::EditorGRPC::SetBoardDescriptionResult* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status PlayBoardEdit::Service::InitialConnectionHandshake(::grpc::ServerContext* context, const ::EditorGRPC::ClientInfo* request, ::EditorGRPC::GameSettings* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace EditorGRPC

#include "GrpcLevelEditorServer.h"
#include <string>
#include "PlayBoardEditImpl.h"
#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <thread>
#include "BlockColourDefs.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using std::chrono::system_clock;

GrpcLevelEditorServer::~GrpcLevelEditorServer()
{
	if (m_server != nullptr) {
		m_server->Shutdown();
	}
	if (m_serverThread != nullptr) {
		delete m_serverThread;
	}
}

GrpcLevelEditorServer::GrpcLevelEditorServer(ILevelEditorServerGame* game)
{
	m_game = game;
	Run();
}

void GrpcLevelEditorServer::Run()
{
	m_serverThread = new std::thread([this]() {
		std::string server_address("127.0.0.1:50051");
		//PlayBoardEditImpl service(m_game);
		ServerBuilder builder;
		builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
		builder.RegisterService(&m_service);
		auto cq = builder.AddCompletionQueue();
		m_server = builder.BuildAndStart();
		std::cout << "Server listening on " << server_address << std::endl;

		// Spawn a new CallData instance to serve new clients.
		new GetBoardStateCallData(&m_service, cq.get(), m_game, this);
		new AddBlockCallData(&m_service, cq.get(), m_game, this);
		void* tag;  // uniquely identifies a request.
		bool ok;
		while (true) {
			// Block waiting to read the next event from the completion queue. The
			// event is uniquely identified by its tag, which in this case is the
			// memory address of a CallData instance.
			// The return value of Next should always be checked. This return value
			// tells us whether there is any kind of event or cq_ is shutting down.
			GPR_ASSERT(cq->Next(&tag, &ok));
			GPR_ASSERT(ok);
			static_cast<CallData*>(tag)->Proceed();
		}
		});
	
}

void GrpcLevelEditorServer::EnactRPC(const LevelEditorRPC& rpc)
{
	switch (rpc.rpcType) {
		case AddBlock:
			std::cout << "AddBlock" << std::endl;
			break;
		case RemoveBlock:
			std::cout << "RemoveBlock" << std::endl;
			break;
		case GetBlocks:
			{
				std::cout << "GetBlocks" << std::endl;
				const auto& boardState = m_game->GetBoardState();
				auto ptr = boardState.getPtr();
				auto w = boardState.getW(); auto h = boardState.getH(); auto d = boardState.getD();
				auto size = w * d * h;
				auto getBoardStateCallDataPtr = (GetBoardStateCallData*)rpc.callData;
				getBoardStateCallDataPtr->reply_.set_data(std::string(ptr, ptr + size));
				getBoardStateCallDataPtr->reply_.set_depth(d);
				getBoardStateCallDataPtr->reply_.set_height(h);
				getBoardStateCallDataPtr->reply_.set_width(w);
			}
			
			break;
	}
}

void GrpcLevelEditorServer::OnEvent(EngineUpdateFrameEventArgs e)
{
	while (!m_editorQueue.IsEmpty()) {
		auto rpc = m_editorQueue.PopFront();
		EnactRPC(rpc);
		rpc.callData->Finish();
	}
}

void GrpcLevelEditorServer::GetBoardStateCallData::RequestOnCreate()
{
	service_->RequestGetBoardState(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void GrpcLevelEditorServer::GetBoardStateCallData::OnProcess()
{
	// Spawn a new CallData instance to serve new clients while we process
	// the one for this CallData. The instance will deallocate itself as
	// part of its FINISH state.
			
	new GetBoardStateCallData(service_, cq_, m_game, server_);

	auto rpc = LevelEditorRPC{ this,GetBlocks };
	server_->m_editorQueue.Push(rpc);
	
}

void GrpcLevelEditorServer::GetBoardStateCallData::Finish()
{
	responder_.Finish(reply_, Status::OK, this);
	status_ = FINISH;

}

void GrpcLevelEditorServer::AddBlockCallData::RequestOnCreate()
{
	service_->RequestAddBlock(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void GrpcLevelEditorServer::AddBlockCallData::OnProcess()
{
	using namespace EditorGRPC;
	new AddBlockCallData(service_, cq_, m_game, server_);
	auto rpc = LevelEditorRPC{ this,AddBlock };
	server_->m_editorQueue.Push(rpc);

	
}

void GrpcLevelEditorServer::AddBlockCallData::Finish()
{
	responder_.Finish(reply_, Status::OK, this);
	status_ = FINISH;

}

void GrpcLevelEditorServer::RemoveBlockCallData::RequestOnCreate()
{
	service_->RequestRemoveBlock(&ctx_, &request_, &responder_, cq_, cq_, this);

}

void GrpcLevelEditorServer::RemoveBlockCallData::OnProcess()
{
	new RemoveBlockCallData(service_, cq_, m_game, server_);
	auto rpc = LevelEditorRPC{ this,RemoveBlock };
	server_->m_editorQueue.Push(rpc);

}

void GrpcLevelEditorServer::RemoveBlockCallData::Finish()
{
	responder_.Finish(reply_, Status::OK, this);
	status_ = FINISH;
}

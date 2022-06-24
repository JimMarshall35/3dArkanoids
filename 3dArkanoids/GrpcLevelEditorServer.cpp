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
		new ChangeBlockCallData(&m_service, cq.get(), m_game, this);
		new InitialConnectionHandshakeCallData(&m_service, cq.get(), m_game, this);
		
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



void GrpcLevelEditorServer::OnEvent(EngineUpdateFrameEventArgs e)
{
	while (!m_editorQueue.IsEmpty()) {
		auto rpc = m_editorQueue.PopFront();
		rpc();
	}
}

void GrpcLevelEditorServer::GetBoardStateCallData::RequestOnCreate()
{
	m_service->RequestGetBoardState(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
}

void GrpcLevelEditorServer::GetBoardStateCallData::OnProcess()
{
	// Spawn a new CallData instance to serve new clients while we process
	// the one for this CallData. The instance will deallocate itself as
	// part of its FINISH state.
			
	new GetBoardStateCallData(m_service, m_cq, m_game, m_server);

	m_server->m_editorQueue.Push([this]() {
			std::cout << "GetBlocks" << std::endl;
			const auto& boardState = m_game->GetBoardState();
			auto ptr = boardState.getPtr();
			auto w = boardState.getW(); auto h = boardState.getH(); auto d = boardState.getD();
			auto size = w * d * h;
			m_reply.set_data(std::string(ptr, ptr + size));
			m_reply.set_depth(d);
			m_reply.set_height(h);
			m_reply.set_width(w);
			Finish();
		});
	
}

void GrpcLevelEditorServer::GetBoardStateCallData::Finish()
{
	m_responder.Finish(m_reply, Status::OK, this);
	m_status = FINISH;
}

void GrpcLevelEditorServer::ChangeBlockCallData::RequestOnCreate()
{
	m_service->RequestChangeBlock(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
}

void GrpcLevelEditorServer::ChangeBlockCallData::OnProcess()
{
	new ChangeBlockCallData(m_service, m_cq, m_game, m_server);
	m_server->m_editorQueue.Push([this]() {
		// TODO: implement
		auto blockCoords = glm::ivec3(m_request.x(), m_request.y(), m_request.z());

		std::cout <<
			"change block x: " << m_request.x() <<
			" y: " << m_request.y() <<
			" z: " << m_request.z() <<
			" newVal: " << m_request.newval() << std::endl;
		auto newVal = static_cast<unsigned char>(m_request.newval());
		unsigned char oldBlockVal;

		auto result = m_game->AddOrChangeBlock(
			blockCoords,
			newVal,
			oldBlockVal
		);

		switch (result) {
		case SPACE_EMPTY:
			m_reply.set_result(EditorGRPC::EditBlockResult::SPACE_EMPTY);
			break;
		case BLOCK_AT_SPACE:
			m_reply.set_blockcode(oldBlockVal);
			m_reply.set_result(EditorGRPC::EditBlockResult::BLOCK_AT_SPACE);
			break;
		case FAILURE_POINT_OUT_OF_BOUNDS:
			m_reply.set_result(EditorGRPC::EditBlockResult::FAILURE_POINT_OUT_OF_BOUNDS);
			break;
		case OTHER_FAILURE:
			m_reply.set_result(EditorGRPC::EditBlockResult::OTHER_FAILURE);
			m_reply.set_errormessage("An unknown error occured");
			break;
		case NO_CHANGE:
			m_reply.set_result(EditorGRPC::EditBlockResult::OTHER_FAILURE);
			m_reply.set_errormessage("The edit resulted in no change");
			break;
		case INVALID_NEW_BYTE:
			m_reply.set_result(EditorGRPC::EditBlockResult::OTHER_FAILURE);
			m_reply.set_errormessage("You chose a byte value that is too high");
			break;
		}
		Finish();
	});
}

void GrpcLevelEditorServer::ChangeBlockCallData::Finish()
{
	m_responder.Finish(m_reply, Status::OK, this);
	m_status = FINISH;
}

void GrpcLevelEditorServer::InitialConnectionHandshakeCallData::Finish()
{
	m_responder.Finish(m_reply, Status::OK, this);
	m_status = FINISH;
}

void GrpcLevelEditorServer::InitialConnectionHandshakeCallData::RequestOnCreate()
{
	m_service->RequestInitialConnectionHandshake(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
}

void GrpcLevelEditorServer::InitialConnectionHandshakeCallData::OnProcess()
{
	new InitialConnectionHandshakeCallData(m_service, m_cq, m_game, m_server);
	m_server->m_editorQueue.Push([this]() {
		auto blockTypes = m_game->GetPossibleBlocks();
		for (const auto& block : blockTypes) {
			auto output = m_reply.add_possibleblocks();
			output->set_gameenginecode(block.GameEngineBlockCode);
			output->set_red(block.Rgb.x);
			output->set_green(block.Rgb.y);
			output->set_blue(block.Rgb.z);
		}
		Finish();
	});
}

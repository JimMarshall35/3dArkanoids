#include "GrpcLevelEditorServer.h"
#include <string>
#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <thread>
#include "SerializationFunctions.h"
#include "SerializableProperty.h"
#include "GetBoardStateCallData.h"
#include "ChangeBlockCallData.h"
#include "InitialConnectionHandshakeCallData.h"
#include "GetSerializableNodesCallData.h"
#include "SetSerializablePropertyCallData.h"
#include "SetBoardStateCallData.h"
#include "GetUpdatedBoardStreamCallData.h"

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

	if (m_serverThread != nullptr) {
		m_shouldServerContinue = false;
		m_serverThread->join();
	}
}

GrpcLevelEditorServer::GrpcLevelEditorServer(ILevelEditorServerGame* game)
{
	m_game = game;
	Run();
}

void GrpcLevelEditorServer::Run()
{
	m_serverThread = std::make_unique<std::thread>([this]() {
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
		new GetSerializableNodesCallData(&m_service, cq.get(), m_game, this);
		new SetSerializablePropertyCallData(&m_service, cq.get(), m_game, this);
		new SetBoardStateCallData(&m_service, cq.get(), m_game, this);
		auto updatedBoardStream = new GetUpdatedBoardStreamCallData(&m_service, cq.get(), m_game, this);
		
		void* tag;  // uniquely identifies a request.
		bool ok;
		while (m_shouldServerContinue) {
			if (m_newBoardStateFlag == true) {
				m_newBoardStateFlag = false;
				EditorGRPC::BoardDescription description;
				description.set_data(std::string(m_newBoardState->getPtr(), m_newBoardState->getPtr() + m_newBoardState->getW() * m_newBoardState->getH() * m_newBoardState->getD()));
				description.set_depth(m_newBoardState->getD());
				description.set_height(m_newBoardState->getH());
				description.set_width(m_newBoardState->getW());
				updatedBoardStream->PushNewData(description);
			}
			// Block waiting to read the next event from the completion queue. The
			// event is uniquely identified by its tag, which in this case is the
			// memory address of a CallData instance.
			// The return value of Next should always be checked. This return value
			// tells us whether there is any kind of event or cq_ is shutting down.
			GPR_ASSERT(cq->Next(&tag, &ok));
			GPR_ASSERT(ok);
			static_cast<IProceed*>(tag)->Proceed();
		}
		m_server->Shutdown();
		// Always shutdown the completion queue after the server.
		cq->Shutdown();
	});
}



void GrpcLevelEditorServer::OnEvent(EngineUpdateFrameEventArgs e)
{
	while (!m_editorQueue.IsEmpty()) {
		auto rpc = m_editorQueue.PopFront();
		rpc();
	}
}

void GrpcLevelEditorServer::NotifyNewBoardState(const Array3D<unsigned char>& newState)
{
	m_newBoardState = &newState;
	m_newBoardStateFlag = true;
}





#include "GrpcLevelEditorServer.h"
#include <string>
#include "PlayBoardEditImpl.h"
#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <thread>
#include "BlockColourDefs.h"
#include "GameFramework.h"
#include "GameToUiMessage.h"
#include "SerializationFunctions.h"
#include "SerializableProperty.h"

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
		new GetSerializableNodesCallData(&m_service, cq.get(), m_game, this);
		new SetSerializablePropertyCallData(&m_service, cq.get(), m_game, this);
		new SetBoardStateCallData(&m_service, cq.get(), m_game, this);
		
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
			output->set_red(block.Rgba.x);
			output->set_green(block.Rgba.y);
			output->set_blue(block.Rgba.z);
			output->set_alpha(block.Rgba[3]);
		}
		GameFramework::SendFrameworkMessage(GameToUiMessage{ 420,true });
		Finish();
	});
}

void GrpcLevelEditorServer::GetSerializableNodesCallData::ProcessSinglePropertiesNode(const ISerializablePropertiesNode* node, ProtoBufSerializablePropertiesNode* replyNode)
{
	
	replyNode->set_name(node->GetSerializableNodeName());
	for (const auto& p : node->GetSerializableProperties()) {
		auto replyProp = replyNode->add_props();
		replyProp->set_name(p.name);
		switch (p.type) {
		case SerializablePropertyType::Uint32:
			replyProp->set_type(EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Uint32);
			{
				auto data = new ProtoBufSerializablePropertyData();
				data->set_u32(p.data.dataUnion.Uint32);
				replyProp->set_allocated_data(data);
			}
			break;
		case SerializablePropertyType::Uint16:
			replyProp->set_type(EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Uint16);
			{
				auto data = new ProtoBufSerializablePropertyData();
				data->set_u16(p.data.dataUnion.Uint16);
				replyProp->set_allocated_data(data);
			}
			break;
		case SerializablePropertyType::Uint8:
			replyProp->set_type(EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Uint8);
			{
				auto data = new ProtoBufSerializablePropertyData();
				data->set_u8(p.data.dataUnion.Uint8);
				replyProp->set_allocated_data(data);
			}
			
			break;
		case SerializablePropertyType::Int32:
			replyProp->set_type(EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Int32);
			{
				auto data = new ProtoBufSerializablePropertyData();
				data->set_i32(p.data.dataUnion.Int32);
				replyProp->set_allocated_data(data);
			}
			break;
		case SerializablePropertyType::Int16:
			replyProp->set_type(EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Int16);
			{
				auto data = new ProtoBufSerializablePropertyData();
				data->set_i16(p.data.dataUnion.Int16);
				replyProp->set_allocated_data(data);
			}
			break;
		case SerializablePropertyType::Int8:
			replyProp->set_type(EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Int8);
			{
				auto data = new ProtoBufSerializablePropertyData();
				data->set_i8(p.data.dataUnion.Int8);
				replyProp->set_allocated_data(data);
			}
			break;
		case SerializablePropertyType::Float:
			replyProp->set_type(EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Float);
			{
				auto data = new ProtoBufSerializablePropertyData();
				data->set_f(p.data.dataUnion.Float);
				replyProp->set_allocated_data(data);
			}
			break;
		case SerializablePropertyType::Double:
			replyProp->set_type(EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Double);
			{
				auto data = new ProtoBufSerializablePropertyData();
				data->set_d(p.data.dataUnion.Double);
				replyProp->set_allocated_data(data);
			}
			break;
		case SerializablePropertyType::Bytes:
			replyProp->set_type(EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Bytes);
			{
				auto data = new ProtoBufSerializablePropertyData();
				data->set_b(p.data.dataUnion.Bytes);
				replyProp->set_allocated_data(data);
			}
			break;
		case SerializablePropertyType::Vec2:
			replyProp->set_type(EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Vec2);
			{
				auto data = new ProtoBufSerializablePropertyData();
				auto vec2 = new EditorGRPC::Vec2();
				vec2->set_x(p.data.dataUnion.Vec2.x);
				vec2->set_y(p.data.dataUnion.Vec2.y);
				data->set_allocated_v2(vec2);
				replyProp->set_allocated_data(data);
			}
			break;
		case SerializablePropertyType::Vec3:
			replyProp->set_type(EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Vec3);
			{
				auto data = new ProtoBufSerializablePropertyData();
				auto vec3 = new EditorGRPC::Vec3();
				vec3->set_x(p.data.dataUnion.Vec3.x);
				vec3->set_y(p.data.dataUnion.Vec3.y);
				vec3->set_z(p.data.dataUnion.Vec3.z);
				data->set_allocated_v3(vec3);
				replyProp->set_allocated_data(data);
			}
			break;
		case SerializablePropertyType::Vec4:
			replyProp->set_type(EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Vec4);
			{
				auto data = new ProtoBufSerializablePropertyData();
				auto vec4 = new EditorGRPC::Vec4();
				vec4->set_r(p.data.dataUnion.Vec4.r);
				vec4->set_g(p.data.dataUnion.Vec4.g);
				vec4->set_b(p.data.dataUnion.Vec4.b);
				vec4->set_a(p.data.dataUnion.Vec4.a);
				data->set_allocated_v4(vec4);
				replyProp->set_allocated_data(data);
			}
			break;
		case SerializablePropertyType::SerializableNode:
			replyProp->set_type(EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_SerializableNode);
			{

			}
			break;
		case SerializablePropertyType::SerializableNodesArray:
			replyProp->set_type(EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_SerializableNodesArray);
			{
				int numNodes = p.data.SizeIfApplicable;
				auto data = new ProtoBufSerializablePropertyData();
				auto children = new EditorGRPC::ChildNodes();
				for (int i = 0; i < numNodes; i++) {
					const auto node = p.data.dataUnion.childNodes[i];
					

					auto childNode = children->add_nodes();
					ProcessSinglePropertiesNode(node, childNode);

					
				}
				data->set_allocated_children(children);
				replyProp->set_allocated_data(data);
			}
			break;
		}
	}
}

void GrpcLevelEditorServer::GetSerializableNodesCallData::Finish()
{
	m_responder.Finish(m_reply, Status::OK, this);
	m_status = FINISH;
}

void GrpcLevelEditorServer::GetSerializableNodesCallData::RequestOnCreate()
{
	m_service->RequestGetSerializableNodes(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
}

void GrpcLevelEditorServer::GetSerializableNodesCallData::OnProcess()
{
	new GetSerializableNodesCallData(m_service, m_cq, m_game, m_server);
	m_server->m_editorQueue.Push([this]() {
		const auto& list = AutoList<ISerializable>::GetList();
		for (const auto node : list) {
			auto replyNode = m_reply.add_nodes();
			ProcessSinglePropertiesNode(node,replyNode);
		}

		Finish();
	});
}

void GrpcLevelEditorServer::SetSerializablePropertyCallData::Finish()
{
	m_responder.Finish(m_reply, Status::OK, this);
	m_status = FINISH;
}

void GrpcLevelEditorServer::SetSerializablePropertyCallData::RequestOnCreate()
{
	m_service->RequestSetSerializableProperty(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
}

void GrpcLevelEditorServer::SetSerializablePropertyCallData::OnProcess()
{
	new SetSerializablePropertyCallData(m_service, m_cq, m_game, m_server);
	m_server->m_editorQueue.Push([this]() {
		auto path = m_request.path();
		auto newval = m_request.newval();
		auto newValCpp = SerializableProperty();
		newValCpp.name = m_request.newval().name();
		switch (newval.type())
		{
		//case EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Bytes:
			//newValCpp.type = SerializablePropertyType::Bytes;
			//newValCpp.data.dataUnion.Bytes = newval.Bytes;
			//break;
		case EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Double:
			newValCpp.type = SerializablePropertyType::Double;
			newValCpp.data.dataUnion.Double = newval.data().d();
			break;
		case EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Float:
			newValCpp.type = SerializablePropertyType::Float;
			newValCpp.data.dataUnion.Float = newval.data().f();
			break;
		case EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Int16:
			newValCpp.type = SerializablePropertyType::Int16;
			newValCpp.data.dataUnion.Int16 = newval.data().i16();
			break;
		case EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Int32:
			newValCpp.type = SerializablePropertyType::Int32;
			newValCpp.data.dataUnion.Int32 = newval.data().i32();
			break;
		case EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Int8:
			newValCpp.type = SerializablePropertyType::Int8;
			newValCpp.data.dataUnion.Int8 = newval.data().i8();
			break;
		case EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Uint16:
			newValCpp.type = SerializablePropertyType::Uint16;
			newValCpp.data.dataUnion.Uint16 = newval.data().u16();
			break;
		case EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Uint32:
			newValCpp.type = SerializablePropertyType::Uint32;
			newValCpp.data.dataUnion.Uint32 = newval.data().u32();
			break;
		case EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Uint8:
			newValCpp.type = SerializablePropertyType::Uint8;
			newValCpp.data.dataUnion.Uint8 = newval.data().u8();
			break;
		case EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Vec2:
			newValCpp.type = SerializablePropertyType::Vec2;
			newValCpp.data.dataUnion.Vec2.x = newval.data().v2().x();
			newValCpp.data.dataUnion.Vec2.y = newval.data().v2().y();
			break;
		case EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Vec3:
			newValCpp.type = SerializablePropertyType::Vec3;
			newValCpp.data.dataUnion.Vec3.x = newval.data().v3().x();
			newValCpp.data.dataUnion.Vec3.y = newval.data().v3().y();
			newValCpp.data.dataUnion.Vec3.z = newval.data().v3().z();
			break;
		case EditorGRPC::SerializableProperty_Type::SerializableProperty_Type_Vec4:
			newValCpp.type = SerializablePropertyType::Vec4;
			newValCpp.data.dataUnion.Vec4.r = newval.data().v4().r();
			newValCpp.data.dataUnion.Vec4.g = newval.data().v4().g();
			newValCpp.data.dataUnion.Vec4.b = newval.data().v4().b();
			newValCpp.data.dataUnion.Vec4.a = newval.data().v4().a();
			break;
		default:
			break;
		}
		SetSerializableProperty(path, newValCpp);

		Finish();
	});
}

void GrpcLevelEditorServer::SetBoardStateCallData::Finish()
{
	m_responder.Finish(m_reply, Status::OK, this);
	m_status = FINISH;
}

void GrpcLevelEditorServer::SetBoardStateCallData::RequestOnCreate()
{
	m_service->RequestSetBoardState(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
}

void GrpcLevelEditorServer::SetBoardStateCallData::OnProcess()
{
	new SetBoardStateCallData(m_service, m_cq, m_game, m_server);
	m_server->m_editorQueue.Push([this]() {
		int w, d, h;
		w = m_request.width();
		h = m_request.height();
		d = m_request.depth();
		auto newArray = Array3D<unsigned char>(w,h,d);
		if (m_request.data().length() > w * h * d) {
			m_reply.set_result(EditorGRPC::SetBoardDescriptionResult_Result::SetBoardDescriptionResult_Result_FAILURE_TOO_MANY_BYTES_FOR_WHD);
			Finish();
			return;
		}
		else if (m_request.data().length() > w * h * d) {
			m_reply.set_result(EditorGRPC::SetBoardDescriptionResult_Result::SetBoardDescriptionResult_Result_FAILURE_TOO_FEW_BYTES_FOR_WHD);
			Finish();
			return;
		}
		memcpy(newArray.getPtr(), m_request.data().c_str(), w * h * d);
		m_game->SetBoardState(newArray);
		Finish();
	});
}

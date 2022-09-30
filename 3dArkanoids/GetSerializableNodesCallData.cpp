#include "GetSerializableNodesCallData.h"
#include "ISerializablePropertiesNode.h"
#include "SerializablePropertyType.h"
#include "GRPCLevelEditorServer.h"


using ProtoBufSerializablePropertyData = EditorGRPC::SerializablePropertyData;

void GetSerializableNodesCallData::ProcessSinglePropertiesNode(const ISerializablePropertiesNode* node, ProtoBufSerializablePropertiesNode* replyNode)
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

void GetSerializableNodesCallData::Finish()
{
	m_responder.Finish(m_reply, Status::OK, this);
	m_status = FINISH;
}

void GetSerializableNodesCallData::RequestOnCreate()
{
	m_service->RequestGetSerializableNodes(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
}

void GetSerializableNodesCallData::OnProcess()
{
	new GetSerializableNodesCallData(m_service, m_cq, m_game, m_server);
	auto& q = m_server->GetEditorQueue();
	q.Push([this]() {
		const auto& list = AutoList<ISerializable>::GetList();
		for (const auto node : list) {
			auto replyNode = m_reply.add_nodes();
			ProcessSinglePropertiesNode(node, replyNode);
		}

		Finish();
		});
}

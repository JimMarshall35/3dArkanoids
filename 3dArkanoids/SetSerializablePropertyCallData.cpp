#include "SetSerializablePropertyCallData.h"
#include "SerializablePropertyType.h"
#include "SerializationFunctions.h"
#include "SerializableProperty.h"
#include "GrpcLevelEditorServer.h"

void SetSerializablePropertyCallData::Finish()
{
	m_responder.Finish(m_reply, Status::OK, this);
	m_status = FINISH;
}

void SetSerializablePropertyCallData::RequestOnCreate()
{
	m_service->RequestSetSerializableProperty(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
}

void SetSerializablePropertyCallData::OnProcess()
{
	new SetSerializablePropertyCallData(m_service, m_cq, m_game, m_server);
	auto& q = m_server->GetEditorQueue();
	q.Push([this]() {
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

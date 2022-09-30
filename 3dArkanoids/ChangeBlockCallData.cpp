#include "ChangeBlockCallData.h"
#include "ILevelEditorServerGame.h"
#include "GrpcLevelEditorServer.h"

void ChangeBlockCallData::RequestOnCreate()
{
	m_service->RequestChangeBlock(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
}

void ChangeBlockCallData::OnProcess()
{
	new ChangeBlockCallData(m_service, m_cq, m_game, m_server);
	auto& q = m_server->GetEditorQueue();
	q.Push([this]() {
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

void ChangeBlockCallData::Finish()
{
	m_responder.Finish(m_reply, Status::OK, this);
	m_status = FINISH;
}
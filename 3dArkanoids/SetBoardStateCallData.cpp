#include "SetBoardStateCallData.h"
#include "GrpcLevelEditorServer.h"
#include "Array3d.h"

void SetBoardStateCallData::Finish()
{
	m_responder.Finish(m_reply, Status::OK, this);
	m_status = FINISH;
}

void SetBoardStateCallData::RequestOnCreate()
{
	m_service->RequestSetBoardState(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
}

void SetBoardStateCallData::OnProcess()
{
	new SetBoardStateCallData(m_service, m_cq, m_game, m_server);
	auto& q = m_server->GetEditorQueue();
	q.Push([this]() {
		int w, d, h;
		w = m_request.width();
		h = m_request.height();
		d = m_request.depth();
		auto newArray = Array3D<unsigned char>(w, h, d);
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

#include "GetBoardStateCallData.h"
#include "GrpcLevelEditorServer.h"
#include "TSQueue.h"
#include <functional>

void GetBoardStateCallData::RequestOnCreate()
{
	m_service->RequestGetBoardState(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
}

void GetBoardStateCallData::OnProcess()
{
	// Spawn a new CallData instance to serve new clients while we process
	// the one for this CallData. The instance will deallocate itself as
	// part of its FINISH state.

	new GetBoardStateCallData(m_service, m_cq, m_game, m_server);
	auto& q = m_server->GetEditorQueue();
	q.Push([this]() {
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

void GetBoardStateCallData::Finish()
{
	m_responder.Finish(m_reply, Status::OK, this);
	m_status = FINISH;
}

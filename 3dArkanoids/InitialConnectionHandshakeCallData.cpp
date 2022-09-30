#pragma once
#include "InitialConnectionHandshakeCallData.h"
#include "GrpcLevelEditorServer.h"
#include "GameFramework.h"
#include "GameToUiMessage.h"

void InitialConnectionHandshakeCallData::Finish()
{
	m_responder.Finish(m_reply, Status::OK, this);
	m_status = FINISH;
}

void InitialConnectionHandshakeCallData::RequestOnCreate()
{
	m_service->RequestInitialConnectionHandshake(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
}

void InitialConnectionHandshakeCallData::OnProcess()
{
	new InitialConnectionHandshakeCallData(m_service, m_cq, m_game, m_server);
	auto& q = m_server->GetEditorQueue();
	q.Push([this]() {
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
		m_server->SetConnectedFlag();
		Finish();
	});
}

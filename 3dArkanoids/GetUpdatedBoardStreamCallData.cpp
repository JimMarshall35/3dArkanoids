#include "GetUpdatedBoardStreamCallData.h"


void GetUpdatedBoardStreamCallData::RequestOnCreate()
{
	m_service->RequestGetUpdatedBoardStream(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
}

void GetUpdatedBoardStreamCallData::OnProcess()
{
	m_responder.Write(m_reply, this);

}

GRPCStreamingCallData<Void, BoardDescription>* GetUpdatedBoardStreamCallData::AllocateNew(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
{
	return new GetUpdatedBoardStreamCallData(service,cq,game,server);
}

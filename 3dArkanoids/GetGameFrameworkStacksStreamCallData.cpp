#include "GetGameFrameworkStacksStreamCallData.h"

void GetGameFrameworkStacksStreamCallData::RequestOnCreate()
{
    m_service->RequestGetGameFramworkStackStream(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
}

void GetGameFrameworkStacksStreamCallData::OnProcess()
{
    m_responder.Write(m_reply, this);
}

GRPCStreamingCallData<Void, GameFrameworkLayers>* GetGameFrameworkStacksStreamCallData::AllocateNew(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
{
    return new GetGameFrameworkStacksStreamCallData(service, cq, game, server);
}

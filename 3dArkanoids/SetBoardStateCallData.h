#pragma once
#include "GRPCCallData.h"
using EditorGRPC::BoardDescription;
using EditorGRPC::SetBoardDescriptionResult;

class SetBoardStateCallData : public GRPCCallData<BoardDescription, SetBoardDescriptionResult> {
public:
    SetBoardStateCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
        :GRPCCallData(service, cq, game, server)
    {
        Proceed();
    }
protected:
    // Inherited via CallData
    virtual void Finish() override;
    virtual void RequestOnCreate() override;
    virtual void OnProcess() override;
};
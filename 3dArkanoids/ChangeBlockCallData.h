#pragma once
#include "GRPCCallData.h"

using EditorGRPC::BlockEdit;
using EditorGRPC::EditBlockResult;


class ChangeBlockCallData : public GRPCCallData<BlockEdit, EditBlockResult> {
public:
    ChangeBlockCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
        :GRPCCallData(service, cq, game, server)
    {
        Proceed();
    }
protected:
    // Inherited via CallData
    virtual void RequestOnCreate() override;
    virtual void OnProcess() override;

    // Inherited via CallData
    virtual void Finish() override;
};
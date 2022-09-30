#pragma once
#include "GRPCCallData.h"
using EditorGRPC::SetSerializablePropertyData;
using EditorGRPC::SetSerializablePropertyResult;

class SetSerializablePropertyCallData : public GRPCCallData<SetSerializablePropertyData, SetSerializablePropertyResult> {
public:
    SetSerializablePropertyCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
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
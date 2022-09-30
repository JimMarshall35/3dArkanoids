#pragma once
#include "GRPCCallData.h"

class GrpcLevelEditorServer;
class ILevelEditorServerGame;
class ServerCompletionQueue;

using EditorGRPC::Void;
using EditorGRPC::BoardDescription;


class GetBoardStateCallData : public GRPCCallData<Void, BoardDescription> {
public:
    GetBoardStateCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
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
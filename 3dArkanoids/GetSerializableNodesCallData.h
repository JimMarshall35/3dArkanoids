#pragma once
#include "GRPCCallData.h"
using EditorGRPC::Void;
using ProtoBufSerializablePropertiesNodes = EditorGRPC::SerializablePropertiesNodes;
using ProtoBufSerializablePropertiesNode = EditorGRPC::SerializablePropertiesNode;


class ISerializablePropertiesNode;
class GetSerializableNodesCallData : public GRPCCallData<Void, ProtoBufSerializablePropertiesNodes> {
public:
    GetSerializableNodesCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
        :GRPCCallData(service, cq, game, server)
    {
        Proceed();
    }
private:
    void ProcessSinglePropertiesNode(const ISerializablePropertiesNode* node, ProtoBufSerializablePropertiesNode* replyNode);
protected:
    // Inherited via CallData
    virtual void Finish() override;
    virtual void RequestOnCreate() override;
    virtual void OnProcess() override;
};
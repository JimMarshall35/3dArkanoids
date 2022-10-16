#pragma once
#include "GRPCStreamingCallData.h"

using EditorGRPC::Void;
using EditorGRPC::GameFrameworkLayers;


class GetGameFrameworkStacksStreamCallData
	: public GRPCStreamingCallData<Void,GameFrameworkLayers>
{
public:
	GetGameFrameworkStacksStreamCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
		:GRPCStreamingCallData<Void, GameFrameworkLayers>(service, cq, game, server)
	{
		Proceed();
	}
	// Inherited via GRPCStreamingCallData
	virtual void RequestOnCreate() override;
	virtual void OnProcess() override;
	virtual GRPCStreamingCallData<Void, GameFrameworkLayers>* AllocateNew(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server) override;
};


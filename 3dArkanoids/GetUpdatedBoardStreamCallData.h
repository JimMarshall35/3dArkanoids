#pragma once
#include "GRPCStreamingCallData.h"

using EditorGRPC::Void;
using EditorGRPC::BoardDescription;

class GetUpdatedBoardStreamCallData 
: public GRPCStreamingCallData<Void,BoardDescription>{
public:
	GetUpdatedBoardStreamCallData(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server)
		:GRPCStreamingCallData<Void, BoardDescription>(service,cq,game,server)
	{
		Proceed();
	}
	// Inherited via GRPCStreamingCallData

	virtual void RequestOnCreate() override;

	virtual void OnProcess() override;




	// Inherited via GRPCStreamingCallData
	virtual GRPCStreamingCallData<Void, BoardDescription>* AllocateNew(EditorGRPC::PlayBoardEdit::AsyncService* service, ServerCompletionQueue* cq, ILevelEditorServerGame* game, GrpcLevelEditorServer* server) override;

};
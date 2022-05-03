#pragma once
#include "ILevelEditorServer.h"
#include "PlayBoardEdit.grpc.pb.h"
#include <memory>
class PlayBoardEditImpl : public EditorGRPC::PlayBoardEdit::Service
{
public:
	PlayBoardEditImpl(ILevelEditorServerGame* game);
private:
    ILevelEditorServerGame* m_game;
	virtual ::grpc::Status GetBoardState(::grpc::ServerContext* context, const ::EditorGRPC::Void* request, ::EditorGRPC::BoardDescription* response);

};


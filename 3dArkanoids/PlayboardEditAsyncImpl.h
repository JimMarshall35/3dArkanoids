#pragma once
#include "ILevelEditorServer.h"
#include "PlayBoardEdit.grpc.pb.h"
#include <memory>
class PlayboardEditAsyncImpl : public EditorGRPC::PlayBoardEdit::AsyncService
{
public:
	PlayboardEditAsyncImpl(ILevelEditorServerGame* game);
private:
	ILevelEditorServerGame* m_game;
};


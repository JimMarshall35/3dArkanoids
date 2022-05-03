#include "PlayBoardEditImpl.h"
#include "ILevelEditorServerGame.h"
#include "Array3d.h"
PlayBoardEditImpl::PlayBoardEditImpl(ILevelEditorServerGame* game)
{
    m_game = game;
}

::grpc::Status PlayBoardEditImpl::GetBoardState(::grpc::ServerContext* context, const::EditorGRPC::Void* request, ::EditorGRPC::BoardDescription* response)
{
    std::cout << "Hello grpc world" << std::endl;
    /*const auto& data = m_game->GetBoardState();
    
    response->set_width(data.getW());
    response->set_depth(data.getD());
    response->set_height(data.getH());
    auto ptr = data.getPtr();*/
    //response->set_data(ptr, data.getD()* data.getW()* data.getH());
    return ::grpc::Status::OK;
}

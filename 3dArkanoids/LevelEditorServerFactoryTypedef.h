#pragma once
#include <functional>
#include <memory>
class ILevelEditorServer;
class ILevelEditorServerGame;
using LevelEditorServerFactory = std::function<std::unique_ptr<ILevelEditorServer> (ILevelEditorServerGame*)>;

#pragma once
#include <functional>
class ILevelEditorServer;
class ILevelEditorServerGame;
using LevelEditorServerFactory = std::function<ILevelEditorServer* (ILevelEditorServerGame*)>;

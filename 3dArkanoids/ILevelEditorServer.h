#pragma once
#include "LevelEditorServerFactoryTypedef.h"
#include "Event.h"
#include "EngineUpdateFrameEventArgs.h"

class ILevelEditorServer : public EventListener<EngineUpdateFrameEventArgs> {
public:

};
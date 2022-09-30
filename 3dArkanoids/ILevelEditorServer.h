#pragma once
#include "LevelEditorServerFactoryTypedef.h"
#include "Event.h"
#include "EngineUpdateFrameEventArgs.h"
#include "Array3d.h"

class ILevelEditorServer : public EventListener<EngineUpdateFrameEventArgs> {
public:
	virtual void NotifyNewBoardState(const Array3D<unsigned char>& newState) = 0;
};
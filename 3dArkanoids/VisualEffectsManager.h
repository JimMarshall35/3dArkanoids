#pragma once
#include "Event.h"
#include "BallComboEventArgs.h"

class VisualEffectsManager
	:public EventListener<BallComboEventArgs>
{
public:
	// Inherited via EventListener
	virtual void OnEvent(BallComboEventArgs e) override;
private:
};


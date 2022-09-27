#pragma once
#include <string>

struct GameToUiMessage {
	int newScore;
	bool editorConnected = false;
	double ballAngleWithYAxis = 0.0;
};
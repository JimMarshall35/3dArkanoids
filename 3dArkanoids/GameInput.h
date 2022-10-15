#pragma once


struct GameInput {
	double XChange; // for moving the bat left and right, change in mouse x on pc.
	bool Firing = false; // for firing the ball, left mouse on pc.
	double xPos;
	double yPos;
};
#pragma once

#include "Event.h"
#include "EngineUpdateFrameEventArgs.h"
#include <glm/glm.hpp>
#include <functional>



class Game;
class GameInput;
#define MAX_NUM_BALLS 128
class BallManager
	:public EventListener<EngineUpdateFrameEventArgs>
{
public:
	void Init(Game* game, Event<EngineUpdateFrameEventArgs>& updateEvent);
	void AddBall(const glm::vec3& pos, glm::vec3 direction, bool stuckToBat);
	void ReceiveInput(const GameInput& gameInput);
private:
	struct Ball {
		glm::vec3 pos;
		glm::vec3 direction;
		bool stuckToBat;

		Ball* nextBall = nullptr;
		
	};
	using BallIteratorFunctionWithCurrentAndPrevious = std::function<bool(Ball*, Ball*, int)>; // return value signals continue / don't continue
	Game* m_game;
	Ball* m_ballListHead = nullptr;
	Ball m_balls[MAX_NUM_BALLS];
	size_t m_numBalls = 0;
	size_t m_recycledBallIndices[MAX_NUM_BALLS];
	size_t m_numRecycledBallIndices = 0;
private:
	void PushRecylcedIndex(size_t index);
	size_t PopRecycledIndex();
	Ball& GetNextFreeBall();
	void RemoveBallAtListIndex(int index);
	void IterateBallList(BallIteratorFunctionWithCurrentAndPrevious iterationFunction);
public:
	//void TestRemoveFunc(int index) {
	//	RemoveBallAtListIndex(index);
	//}
	// Inherited via EventListener
	virtual void OnEvent(EngineUpdateFrameEventArgs e) override;
};


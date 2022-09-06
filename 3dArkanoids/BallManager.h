#pragma once

#include "Event.h"
#include "EngineUpdateFrameEventArgs.h"
#include <glm/glm.hpp>
#include <functional>

class Game;
class IRenderer;
class GameInput;
class Camera;
class Bat;
#define MAX_NUM_BALLS 128
#define DEFAULT_BALL_RADIUS 2.0f
#define DEFAULT_BALL_SPEED 2.0f

class BallManager
	:public EventListener<EngineUpdateFrameEventArgs>
{
public:
	void Init(Game* game, Event<EngineUpdateFrameEventArgs>& updateEvent, const Bat* bat);
	void AddBall(const glm::vec3& pos, glm::vec3 direction, bool stuckToBat, float radius = DEFAULT_BALL_RADIUS, float speed = DEFAULT_BALL_SPEED);
	void ReceiveInput(double changeInBatX);
	void Draw(const IRenderer* renderer, const Camera& camera) const;
	void ReleaseBalls();
private:
	struct Ball {
		glm::vec3 pos;
		float radius;
		float speed;
		glm::vec3 direction;
		bool stuckToBat;

		Ball* nextBall = nullptr;
		
	};
	using BallIteratorFunctionWithCurrentAndPrevious = std::function<bool(Ball*, Ball*, int)>; // return value signals continue / don't continue
	Game* m_game;
	const Bat* m_bat;
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
	void IterateBallList(BallIteratorFunctionWithCurrentAndPrevious iterationFunction) const;
private:
	static void ReflectBall(Ball* thisBall, const glm::vec2& newPos, const glm::vec2& nearestPoint);
public:
	//void TestRemoveFunc(int index) {
	//	RemoveBallAtListIndex(index);
	//}
	// Inherited via EventListener
	virtual void OnEvent(EngineUpdateFrameEventArgs e) override;
};


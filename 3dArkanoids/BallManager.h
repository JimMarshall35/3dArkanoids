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
#define LOOKAHEAD_BUFFER_SIZE 300

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
		bool jumping = false;
		Ball* nextBall = nullptr;
		double jumpTimer = 0.0;
		
	};
	enum class BallAdvanceResult {
		HIT_NOTHING,
		HIT_LEFT_SIDE,
		HIT_RIGHT_SIDE,
		HIT_TOP_SIDE,
		OUT_OF_PLAY,
		HIT_BAT,
		HIT_BLOCK
	};
	using BallIteratorFunctionWithCurrentAndPrevious = std::function<bool(Ball*, Ball*, int)>; // return value signals continue / don't continue
private:
	Game* m_game;
	const Bat* m_bat;
	Ball* m_ballListHead = nullptr;
	Ball m_balls[MAX_NUM_BALLS];
	size_t m_numBalls = 0;
	size_t m_recycledBallIndices[MAX_NUM_BALLS];
	size_t m_numRecycledBallIndices = 0;
	
	size_t m_lookaheadBy = 75;
	size_t m_numAdvancesToCheckForBatHit = 100;
	glm::vec3 m_lookAheadPositions[LOOKAHEAD_BUFFER_SIZE];
	bool m_drawProjectedBalls = false;
	
private:
	void PushRecylcedIndex(size_t index);
	size_t PopRecycledIndex();
	Ball& GetNextFreeBall();
	void RemoveBallAtListIndex(int index);
	void IterateBallList(BallIteratorFunctionWithCurrentAndPrevious iterationFunction) const;
	void IterateBallList(BallIteratorFunctionWithCurrentAndPrevious iterationFunction);

	BallAdvanceResult AdvanceBall(const Ball* ball, glm::vec3& posToChange, glm::vec3& dirToChange, double& jumpTimerToChange, bool& jumpingToChange, bool deleteBlock = true);
	void LookAhead(const Ball* thisBall);
private:
	static void ReflectBall(glm::vec3& directionToChange, const glm::vec2& newPos, const glm::vec2& nearestPoint, const glm::vec3& oldDirection);
public:
	//void TestRemoveFunc(int index) {
	//	RemoveBallAtListIndex(index);
	//}
	// Inherited via EventListener
	virtual void OnEvent(EngineUpdateFrameEventArgs e) override;
};


#pragma once

#include "Event.h"
#include "EngineUpdateFrameEventArgs.h"
#include "ISerializable.h"
#include <glm/glm.hpp>
#include <functional>
#include "Event.h"
#include "BallComboEventArgs.h"
#include "SoundEffectEventArgs.h"

class Game;
class IRenderer;
class GameInput;
class Camera;
class Bat;
#define MAX_NUM_BALLS 128
#define DEFAULT_BALL_RADIUS 2.0f
#define DEFAULT_BALL_SPEED 1.0f
#define BALL_ADVANCES_PER_FRAME 3
#define LOOKAHEAD_BUFFER_SIZE 300

class BallManager
	:public EventListener<EngineUpdateFrameEventArgs>,
	public ISerializable
{
public:
	void Init(Game* game, Event<EngineUpdateFrameEventArgs>& updateEvent, const Bat* bat);
	void AddBall(const glm::vec3& pos, glm::vec3 direction, bool stuckToBat, float radius = DEFAULT_BALL_RADIUS, float speed = DEFAULT_BALL_SPEED);
	void ReceiveInput(double changeInBatX);
	void Draw(const IRenderer* renderer, const Camera& camera) const;
	void ReleaseBalls();
	void SubscribeToBallComboEvent(EventListener<BallComboEventArgs>* listener) {
		m_ballComboEvent += listener;
	}
	void UnSubscribeFromBallComboEvent(EventListener<BallComboEventArgs>* listener) {
		m_ballComboEvent -= listener;
	}
	void SubscribeToSoundEffectEvent(EventListener<SoundEffectEventArgs>* listener) {
		m_soundEffectEvent += listener;
	}
	void UnSubscribeToSoundEffectEvent(EventListener<SoundEffectEventArgs>* listener) {
		m_soundEffectEvent -= listener;
	}

private:
	struct Ball {
		glm::vec3 pos;
		float radius;
		float speed;
		glm::vec3 direction;
		bool stuckToBat;
		bool jumping = false;
		Ball* nextBall = nullptr;

		double jumpAmount = 5.0;
		double jumpTimer = 0.0;
		double jumpTime = 1.0;

		int blockCombo = 0;
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
	double m_mostObtuseAngleAllowed = 70.0;
	float m_minBallY;

	Event<BallComboEventArgs> m_ballComboEvent;
	Event<SoundEffectEventArgs> m_soundEffectEvent;
private:
	void PushRecylcedIndex(size_t index);
	size_t PopRecycledIndex();
	Ball& GetNextFreeBall();
	void RemoveBallAtListIndex(int index);
	void IterateBallList(BallIteratorFunctionWithCurrentAndPrevious iterationFunction) const;
	void IterateBallList(BallIteratorFunctionWithCurrentAndPrevious iterationFunction);

	// todo: just change this to not take a const Ball ptr and get rid of other params except deleteblock
	BallAdvanceResult AdvanceBall(Ball* thisBall, bool deleteBlock=true);
	void LookAhead(const Ball* thisBall);
private:
	static void ReflectBall(glm::vec3& directionToChange, const glm::vec3& newPos, const glm::vec3& nearestPoint, const glm::vec3& oldDirection);
public:
	//void TestRemoveFunc(int index) {
	//	RemoveBallAtListIndex(index);
	//}
	// Inherited via EventListener
	virtual void OnEvent(EngineUpdateFrameEventArgs e) override;

	// Inherited via ISerializable
	virtual const std::vector<SerializableProperty>& GetSerializableProperties() const override;
	virtual bool SetSerializableProperty(const SerializableProperty& p) override;
	virtual int GetNumSerializableProperties() const override;
	virtual std::string GetSerializableNodeName() const override;
	virtual void SaveToFile(std::string filePath) const override;
	virtual void LoadFromFile(std::string filePath) override;
	virtual char* SaveToBuffer(char* destinaion) const override;
	virtual const char* LoadFromBuffer(const char* source) override;
	virtual size_t GetBinaryFileNumBytes() const override;
	virtual bool ShouldLoadAndSaveFromBigFile()const override { return false; }
};


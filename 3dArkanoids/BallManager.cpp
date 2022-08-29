#include "BallManager.h"
#include "GameInput.h"

void BallManager::Init(Game* game, Event<EngineUpdateFrameEventArgs>& updateEvent)
{
	m_game = game;
	updateEvent += this;
}

void BallManager::AddBall(const glm::vec3& pos, glm::vec3 direction, bool stuckToBat)
{
	auto& ball = GetNextFreeBall();
	ball.pos = pos;
	ball.stuckToBat = stuckToBat;
	ball.direction = direction;
	ball.nextBall = nullptr;
	Ball** endOfBallList = &m_ballListHead;
	if (*endOfBallList == nullptr) {
		*endOfBallList = &ball;
	}
	else {
		do {
			endOfBallList = &(*endOfBallList)->nextBall;
		} while (*endOfBallList != nullptr);
		*endOfBallList = &ball;
	}
}

void BallManager::ReceiveInput(const GameInput& gameInput)
{
}

void BallManager::PushRecylcedIndex(size_t index)
{
	if (m_numRecycledBallIndices < MAX_NUM_BALLS) {
		m_recycledBallIndices[m_numRecycledBallIndices++] = index;
	}
	m_numBalls--;
}

size_t BallManager::PopRecycledIndex()
{
	assert(m_numRecycledBallIndices > 0);
	
	return m_recycledBallIndices[--m_numRecycledBallIndices];
}

BallManager::Ball& BallManager::GetNextFreeBall()
{
	int index;
	if (m_numRecycledBallIndices > 0) {
		m_numBalls++;
		return m_balls[PopRecycledIndex()];
	}
	else {
		return m_balls[m_numBalls++];
	}
}


void BallManager::RemoveBallAtListIndex(int index)
{
	IterateBallList([this, index](Ball* thisBall, Ball* lastBall, int onIteration) {
		auto previousListNodePtr = &m_ballListHead;
		if (onIteration == index) {
			if (lastBall == nullptr) {
				m_ballListHead = thisBall->nextBall;
			}
			else {
				lastBall->nextBall = thisBall->nextBall;
			}
			auto indexInArray = thisBall - m_balls;
			PushRecylcedIndex(index);
			return false;
		}
		return true;
	});
}

void BallManager::IterateBallList(BallIteratorFunctionWithCurrentAndPrevious iterationFunction)
{
	if (m_ballListHead->nextBall == nullptr) {
		return;
	}
	Ball* lastBall = nullptr;
	Ball* thisBall = m_ballListHead;
	int onIteration = 0;
	do {
		if (!iterationFunction(thisBall, lastBall, onIteration++)) {
			return;
		}

		lastBall = thisBall;
		thisBall = thisBall->nextBall;
	} while (thisBall != nullptr);
	
}

void BallManager::OnEvent(EngineUpdateFrameEventArgs e)
{
}

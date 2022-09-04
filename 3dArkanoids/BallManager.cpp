#include "BallManager.h"
#include "GameInput.h"
#include "IRenderer.h"
#include "Camera.h"

void BallManager::Init(Game* game, Event<EngineUpdateFrameEventArgs>& updateEvent)
{
	m_game = game;
	updateEvent += this;
}

void BallManager::AddBall(const glm::vec3& pos, glm::vec3 direction, bool stuckToBat, float radius = 3.0f)
{
	auto& ball = GetNextFreeBall();
	ball.pos = pos;
	ball.stuckToBat = stuckToBat;
	ball.direction = direction;
	ball.nextBall = nullptr;
	ball.radius = radius;
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

void BallManager::ReceiveInput(double changeInBatX)
{
	IterateBallList([this, changeInBatX](Ball* thisBall, Ball* lastBall, int onIteration) {
		if (!thisBall->stuckToBat) {
			return true;
		}
		thisBall->pos.x += changeInBatX;
		return true;
	});
}

void BallManager::Draw(const IRenderer* renderer, const Camera& camera) const
{
	IterateBallList([renderer, camera](Ball* thisBall, Ball* lastBall, int onIteration) {
		auto r = thisBall->radius;
		renderer->DrawSphere(
			thisBall->pos,
			{r,r,r},
			camera,
			{1.0,1.0,1.0}
		);
		return true;
	});
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

void BallManager::IterateBallList(BallIteratorFunctionWithCurrentAndPrevious iterationFunction) const
{
	if (m_ballListHead == nullptr) {
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


#include <iostream>
#include "BallManager.h"
#include "GameInput.h"
#include "IRenderer.h"
#include "Camera.h"
#include "PlayfieldDefs.h"
#include "game.h"
#include "Bat.h"

#include <glm/gtx/rotate_vector.hpp>


glm::ivec3 WorldSpaceToTileCoordsRound(const glm::vec3& ballCenter);
glm::ivec3 WorldSpaceToTileCoordsFloor(const glm::vec3& ballCenter);
static inline glm::ivec2 IVec3ToIVec2(const glm::ivec3& vec3);
static inline glm::vec2 Vec3ToVec2(const glm::vec3& vec3);
static inline glm::vec2 AdjustTileCoordsToWorld(const glm::ivec2& vCell);


void BallManager::Init(Game* game, Event<EngineUpdateFrameEventArgs>& updateEvent, const Bat* bat)
{
	m_game = game;
	m_bat = bat;
	updateEvent += this;
}

void BallManager::AddBall(const glm::vec3& pos, glm::vec3 direction, bool stuckToBat, float radius, float speed)
{
	auto& ball = GetNextFreeBall();
	ball.pos = pos;
	ball.stuckToBat = stuckToBat;
	ball.direction = direction;
	ball.nextBall = nullptr;
	ball.radius = radius;
	ball.speed = speed;
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
	static glm::vec3 lookaheadPositions[LOOKAHEAD_BUFFER_SIZE];
	static glm::vec3 lookaheadDirections[LOOKAHEAD_BUFFER_SIZE];

	IterateBallList([this,renderer, camera](Ball* thisBall, Ball* lastBall, int onIteration) {
		auto r = thisBall->radius;
		renderer->DrawSphere(
			thisBall->pos,
			{r,r,r},
			camera,
			{1.0,1.0,1.0, 1.0}
		);

		if (m_drawProjectedBalls) {
			for (int i = 0; i < m_lookaheadBy; i++) {
				renderer->DrawSphere(
					m_lookAheadPositions[i],
					{ r,r,r },
					camera,
					{ 1.0,1.0,1.0, 0.3}

				);
			}
		}
		
		return true;
	});
}

void BallManager::ReleaseBalls()
{
	IterateBallList([](Ball* thisBall, Ball* lastBall, int onIteration) {
		if (thisBall->stuckToBat) {
			thisBall->stuckToBat = false;
		}
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

void BallManager::IterateBallList(BallIteratorFunctionWithCurrentAndPrevious iterationFunction)
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





glm::ivec3 WorldSpaceToTileCoordsRound(const glm::vec3& ballCenter) {
	return glm::ivec3{
		std::round((ballCenter.x + ((float)BLOCK_WIDTH_UNITS / 2.0f)) / (float)BLOCK_WIDTH_UNITS),
		std::round((ballCenter.y + ((float)BLOCK_HEIGHT_UNITS / 2.0f)) / (float)BLOCK_HEIGHT_UNITS),
		std::round((ballCenter.z + ((float)BLOCK_DEPTH_UNITS / 2.0f)) / (float)BLOCK_DEPTH_UNITS)
	};
}

glm::ivec3 WorldSpaceToTileCoordsFloor(const glm::vec3& ballCenter) {
	return glm::ivec3{
		std::floor((ballCenter.x + ((float)BLOCK_WIDTH_UNITS / 2.0f)) / (float)BLOCK_WIDTH_UNITS),
		std::floor((ballCenter.y + ((float)BLOCK_HEIGHT_UNITS / 2.0f)) / (float)BLOCK_HEIGHT_UNITS),
		std::floor((ballCenter.z + ((float)BLOCK_DEPTH_UNITS / 2.0f)) / (float)BLOCK_DEPTH_UNITS)
	};
}

static inline glm::ivec2 IVec3ToIVec2(const glm::ivec3& vec3) {
	return glm::ivec2(vec3.x, vec3.y);
}

static inline glm::vec2 Vec3ToVec2(const glm::vec3& vec3) {
	return glm::vec2(vec3.x, vec3.y);
}

static inline glm::vec2 AdjustTileCoordsToWorld(const glm::ivec2& vCell) {
	return glm::vec2{
		float(vCell.x) * BLOCK_WIDTH_UNITS - ((float)BLOCK_WIDTH_UNITS / 2.0f),
		float(vCell.y) * BLOCK_HEIGHT_UNITS - ((float)BLOCK_HEIGHT_UNITS / 2.0f)
	};
}

void BallManager::ReflectBall(glm::vec3& directionToChange, const glm::vec2& newPos, const glm::vec2& nearestPoint, const glm::vec3& oldDirection)
{
	auto vec2Relected = glm::reflect(
		Vec3ToVec2(oldDirection),
		glm::normalize(newPos - nearestPoint)
	);
	directionToChange.x = vec2Relected.x;
	directionToChange.y = vec2Relected.y;
}


BallManager::BallAdvanceResult BallManager::AdvanceBall(const Ball* thisBall, glm::vec3& posToChange, glm::vec3& dirToChange, bool deleteBlock)
{

	// refactor from here

	auto oldBallDirection = thisBall->direction;
	auto& ballCenter = thisBall->pos;


	glm::vec3 vPotentialPosition = {
		thisBall->pos.x + thisBall->direction.x * thisBall->speed,
		thisBall->pos.y + thisBall->direction.y * thisBall->speed,
		thisBall->pos.z + thisBall->direction.z * thisBall->speed,
	};

	if (thisBall->pos.x < -((float)BLOCK_WIDTH_UNITS * 0.5f) + thisBall->radius) {
		posToChange.x = -((float)BLOCK_WIDTH_UNITS * 0.5f) + thisBall->radius;
		dirToChange.x *= -1.0f;
		return BallAdvanceResult::HIT_LEFT_SIDE;
	}
	if (thisBall->pos.x > (float)m_game->GetBoardState().getW() * (float)BLOCK_WIDTH_UNITS - (float)BLOCK_WIDTH_UNITS * 0.5f) {
		posToChange.x = (float)m_game->GetBoardState().getW() * (float)BLOCK_WIDTH_UNITS - (float)BLOCK_WIDTH_UNITS * 0.5f;
		dirToChange.x *= -1.0f;
		return BallAdvanceResult::HIT_RIGHT_SIDE;
	}
	if (thisBall->pos.y > (float)m_game->GetBoardState().getH() * (float)BLOCK_HEIGHT_UNITS - (float)BLOCK_HEIGHT_UNITS * 0.5f) {
		posToChange.y = (float)m_game->GetBoardState().getH() * (float)BLOCK_HEIGHT_UNITS - (float)BLOCK_HEIGHT_UNITS * 0.5f;
		dirToChange.y *= -1.0f;
		return BallAdvanceResult::HIT_TOP_SIDE;
	}


	auto batX = m_bat->GetXPos();
	auto batDistanceFromFirstRow = m_bat->GetDistanceFromFirstRow();
	const auto& batDepthAndHeight = m_bat->GetDepthAndHeight();
	auto batWidth = m_bat->GetWidth();

	auto topOfBat = -(batDistanceFromFirstRow + BLOCK_DEPTH_UNITS / 2.0f) + batDepthAndHeight.x * 0.5f + thisBall->radius;

	if (vPotentialPosition.y < topOfBat && vPotentialPosition.y > topOfBat - batDepthAndHeight.x) {
		bool collidingWithBat = (vPotentialPosition.x <= batX + batWidth / 2.0f + thisBall->radius)
			&& (vPotentialPosition.x >= batX - batWidth / 2.0f + thisBall->radius);
		if (collidingWithBat) {
			posToChange.y = topOfBat;
			posToChange.x = vPotentialPosition.x;
			dirToChange.y *= -1.0f;

			auto rotationMagnitude = (thisBall->pos.x - batX) / (batWidth / 2 + thisBall->radius);

			auto rotatedVec = glm::rotate(Vec3ToVec2(thisBall->direction), -(float)rotationMagnitude * glm::radians(35.0f));
			dirToChange.x = rotatedVec.x;
			dirToChange.y = rotatedVec.y;

			return BallAdvanceResult::HIT_BAT;
		}

	}

	// to here


	// see https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/OneLoneCoder_PGE_CircleVsRect.cpp for collision detection algorithm


	auto currentCellTileCoords = WorldSpaceToTileCoordsFloor(thisBall->pos);

	auto targetCellTileCoords = WorldSpaceToTileCoordsRound(vPotentialPosition);

	auto vAreaTL = glm::min(IVec3ToIVec2(currentCellTileCoords), IVec3ToIVec2(targetCellTileCoords)) - glm::ivec2(1, 1);
	auto vAreaBR = glm::max(IVec3ToIVec2(currentCellTileCoords), IVec3ToIVec2(targetCellTileCoords)) + glm::ivec2(1, 1);

	auto returnVal = BallAdvanceResult::HIT_NOTHING;
	glm::vec2 vRayToNearest;

	glm::ivec2 vCell;
	for (vCell.y = vAreaTL.y; vCell.y <= vAreaBR.y; vCell.y++)
	{
		for (vCell.x = vAreaTL.x; vCell.x <= vAreaBR.x; vCell.x++)
		{
			unsigned char blockCode;
			// Check if the cell is actually solid...
			if (vCell.x < 0 || vCell.y < 0) {
				continue;
			}
			if (m_game->BlockAtLocation({ vCell.x, vCell.y, 0 }, blockCode) == EditBlockResultCode::BLOCK_AT_SPACE)
			{
				// ...it is! So work out nearest point to future player position, around perimeter
				// of cell rectangle. We can test the distance to this point to see if we have
				// collided. 
				auto adjustedCellXY = AdjustTileCoordsToWorld(vCell);

				auto adjustedCellPlusOneXY = AdjustTileCoordsToWorld(vCell + glm::ivec2{ 1,1 });

				glm::vec2 vNearestPoint;
				vNearestPoint.x = std::max(adjustedCellXY.x, std::min(vPotentialPosition.x, adjustedCellPlusOneXY.x));
				vNearestPoint.y = std::max(adjustedCellXY.y, std::min(vPotentialPosition.y, adjustedCellPlusOneXY.y));

				glm::vec2 vRayToNearest = vNearestPoint - Vec3ToVec2(vPotentialPosition);
				auto len = glm::length(vRayToNearest);
				float fOverlap = thisBall->radius - len;
				if (std::isnan(fOverlap)) {
					fOverlap = 0;
				}

				// If overlap is positive, then a collision has occurred, so we displace backwards by the 
				// overlap amount. The potential position is then tested against other tiles in the area
				// therefore "statically" resolving the collision
				if (fOverlap > 0)
				{
					returnVal = BallAdvanceResult::HIT_BLOCK;
					// Statically resolve the collision
					auto newPos = Vec3ToVec2(vPotentialPosition) - glm::normalize(vRayToNearest) * fOverlap;
					vPotentialPosition.x = newPos.x;
					vPotentialPosition.y = newPos.y;

					ReflectBall(dirToChange, newPos, vNearestPoint, oldBallDirection);

					// delete block (will have more sophisiticated handling for different blocks)
					unsigned char oldval;

					if (deleteBlock) {
						m_game->RemoveBlock({ vCell.x,vCell.y,0 });
					}

					//goto loop_end;
				}
			}
		}
	}
loop_end:
	//if (blockHit) {
	//	m_game->RemoveBlock({ coordsHit.x,coordsHit.y,0 });

	//}
	posToChange = vPotentialPosition;
	return returnVal;
}

void BallManager::LookAhead(const Ball* thisBall)
{
	auto ball = *thisBall;
	m_drawProjectedBalls = false;

	for (int i = 0; i < m_numAdvancesToCheckForBatHit; i++) {
		if (AdvanceBall(&ball, ball.pos, ball.direction, false) == BallAdvanceResult::HIT_BAT) {
			m_drawProjectedBalls = true;
			break;
		}

	}
	if (m_drawProjectedBalls) {
		for (int i = 0; i < m_lookaheadBy; i++) {
			AdvanceBall(&ball, ball.pos, ball.direction, false);
			m_lookAheadPositions[i] = ball.pos;

		}
	}
}

void BallManager::OnEvent(EngineUpdateFrameEventArgs e)
{
	IterateBallList([this](Ball* thisBall, Ball* lastBall, int onIteration) {
		if (thisBall->stuckToBat) {
			return true;
		}

		AdvanceBall(thisBall, thisBall->pos, thisBall->direction);

		LookAhead(thisBall);

		return true;
	});
}

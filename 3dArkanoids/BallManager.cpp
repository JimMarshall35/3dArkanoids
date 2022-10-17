
#include <iostream>
#include "BallManager.h"
#include "GameInput.h"
#include "IRenderer.h"
#include "Camera.h"
#include "PlayfieldDefs.h"
#include "game.h"
#include "Bat.h"


#include <glm/gtx/rotate_vector.hpp>
#include "GameToUiMessage.h"
#define TARGET_MS_PER_FRAME 30

#define BIG_JUMP_TIME 1.0
#define BIG_JUMP_AMOUNT 5.0

#define SMALL_JUMP_TIME 0.4
#define SMALL_JUMP_AMOUNT 2.0


// helpers
static glm::ivec3 WorldSpaceToTileCoordsRound(const glm::vec3& ballCenter);
static glm::ivec3 WorldSpaceToTileCoordsFloor(const glm::vec3& ballCenter);
static inline glm::ivec2 IVec3ToIVec2(const glm::ivec3& vec3);
static inline glm::vec2 Vec3ToVec2(const glm::vec3& vec3);
static inline glm::vec2 AdjustTileCoordsToWorld(const glm::ivec2& vCell);


void BallManager::Init(Game* game, const Bat* bat)
{
	m_game = game;
	m_bat = bat;
	m_minBallY = -(bat->GetDistanceFromFirstRow() + 80.0f);

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

void BallManager::ClearBalls()
{
	m_numBalls = 0;
	m_numRecycledBallIndices = 0;
	m_ballListHead = nullptr;
}

void BallManager::ReceiveInput(double changeInBatX)
{
	IterateBallList([this, changeInBatX](Ball* thisBall, Ball* lastBall, int onIteration) {
		if (!thisBall->stuckToBat) {
			return true; // this is the same as continue
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
	IterateBallList([this](Ball* thisBall, Ball* lastBall, int onIteration) {
		if (thisBall->stuckToBat) {
			thisBall->stuckToBat = false;
		}
		else {
			if (!thisBall->jumping && m_numBalls == 1) {
				m_soundEffectEvent({ SoundEffect::Jump });
				thisBall->jumping = true;
				thisBall->jumpTime = BIG_JUMP_TIME;
				thisBall->jumpAmount = BIG_JUMP_AMOUNT;
			}
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





static glm::ivec3 WorldSpaceToTileCoordsRound(const glm::vec3& ballCenter) {
	return glm::ivec3{
		std::round((ballCenter.x + ((float)BLOCK_WIDTH_UNITS / 2.0f)) / (float)BLOCK_WIDTH_UNITS),
		std::round((ballCenter.y + ((float)BLOCK_HEIGHT_UNITS / 2.0f)) / (float)BLOCK_HEIGHT_UNITS),
		std::round((ballCenter.z + ((float)BLOCK_DEPTH_UNITS / 2.0f)) / (float)BLOCK_DEPTH_UNITS)
	};
}

static glm::ivec3 WorldSpaceToTileCoordsFloor(const glm::vec3& ballCenter) {
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

static inline glm::vec3 AdjustTileCoordsToWorld(const glm::ivec3& vCell) {
	return glm::vec3{
		float(vCell.x) * BLOCK_WIDTH_UNITS - ((float)BLOCK_WIDTH_UNITS / 2.0f),
		float(vCell.y) * BLOCK_HEIGHT_UNITS - ((float)BLOCK_HEIGHT_UNITS / 2.0f),
		float(vCell.z) * BLOCK_DEPTH_UNITS - ((float)BLOCK_DEPTH_UNITS / 2.0f)

	};
}

void BallManager::ReflectBall(glm::vec3& directionToChange, const glm::vec3& newPos, const glm::vec3& nearestPoint, const glm::vec3& oldDirection)
{
	auto vec2Relected = glm::normalize(glm::reflect(
		oldDirection,
		glm::normalize(newPos - nearestPoint)
	));

	directionToChange.x = vec2Relected.x;
	directionToChange.y = vec2Relected.y;
	//directionToChange = glm::normalize(directionToChange);
	//directionToChange.z = vec2Relected.z;
}

void BallManager::ResetAfterDeath()
{
	const auto ballStartingY =
		-(m_bat->GetDistanceFromFirstRow() + BLOCK_DEPTH_UNITS * 0.5f) + DEFAULT_BALL_RADIUS + (m_bat->GetDepthAndHeight().x * 0.5f);
	AddBall(
		{ m_bat->GetXPos() , ballStartingY, 0 },
		glm::normalize(glm::vec3{ -0.5,1,0 }),
		true);

}

static float ClampDirection(glm::vec3& dirToChange, double mostObtuseAngleAllowed){
	// returns angle against y axis
	const double Pi = 3.14159265358979323846264;
	const double toRadians = Pi / 180.0;
	const double toDegrees = 180.0 / Pi;
	auto angle = (atan2(abs(dirToChange.y), dirToChange.x) - (90.0 * toRadians)) * toDegrees;
	auto absAngle = abs(angle);
	if (absAngle > mostObtuseAngleAllowed) {
		auto rotated = glm::rotate(Vec3ToVec2(dirToChange), ((float)(angle / absAngle) * (float)(mostObtuseAngleAllowed - absAngle)) * (float)toRadians);
		dirToChange.x = rotated.x;
		dirToChange.y = rotated.y;
	}
	return angle;
}

BallManager::BallAdvanceResult BallManager::AdvanceBall(Ball* thisBall, bool deleteBlock)
{
	auto angle = ClampDirection(thisBall->direction, m_mostObtuseAngleAllowed);
	if (deleteBlock) {
		GameToUiMessage m;
		m.ballAngleWithYAxis = angle;
		GameFramework::SendFrameworkMessage(m);
	}
	


	if (thisBall->jumping) {
		thisBall->jumpTimer += ((double)TARGET_MS_PER_FRAME / 1000.0f)/(double)BALL_ADVANCES_PER_FRAME;
		if (thisBall->jumpTimer >= thisBall->jumpTime) {
			thisBall->jumpTimer = 0;
			thisBall->jumping = false;
			thisBall->direction.z = 0;
			thisBall->pos.z = 0;
			thisBall->direction = glm::normalize(thisBall->direction);
			thisBall->blockCombo = 0;
		}
		else {
			const auto ballJumpFromBottomToPeakTime = thisBall->jumpTime / 2.0;
			auto jumpProgress = (ballJumpFromBottomToPeakTime - thisBall->jumpTimer) / ballJumpFromBottomToPeakTime;
			//std::cout << jumpProgress << "\n";
			auto directionXY = glm::vec3(thisBall->direction.x, thisBall->direction.y, 0);
			directionXY.z = jumpProgress * thisBall->jumpAmount;
			thisBall->direction = directionXY;
		}
		
	}

	auto vec2Dir = glm::vec2(thisBall->direction.x, thisBall->direction.y);


	// refactor from here

	
	auto& ballCenter = thisBall->pos;



	glm::vec3 vPotentialPosition = {
		thisBall->pos.x + thisBall->direction.x * thisBall->speed,
		thisBall->pos.y + thisBall->direction.y * thisBall->speed,
		thisBall->pos.z + thisBall->direction.z * thisBall->speed,
	};

	if (thisBall->pos.y < m_minBallY) {
		return BallAdvanceResult::OUT_OF_PLAY;
	}
	if (thisBall->pos.x < -((float)BLOCK_WIDTH_UNITS * 0.5f) + thisBall->radius) {
		thisBall->pos.x = -((float)BLOCK_WIDTH_UNITS * 0.5f) + thisBall->radius;
		thisBall->direction.x *= -1.0f;
		if (deleteBlock) {
			m_soundEffectEvent({ SoundEffect::HitSide });
		}
		
		return BallAdvanceResult::HIT_LEFT_SIDE;
	}
	if (thisBall->pos.x > (float)m_game->GetBoardState().getW() * (float)BLOCK_WIDTH_UNITS - (float)BLOCK_WIDTH_UNITS * 0.5f) {
		thisBall->pos.x = (float)m_game->GetBoardState().getW() * (float)BLOCK_WIDTH_UNITS - (float)BLOCK_WIDTH_UNITS * 0.5f;
		thisBall->direction.x *= -1.0f;
		if (deleteBlock) {
			m_soundEffectEvent({ SoundEffect::HitSide });
		}

		return BallAdvanceResult::HIT_RIGHT_SIDE;
	}
	if (thisBall->pos.y > (float)m_game->GetBoardState().getH() * (float)BLOCK_HEIGHT_UNITS - (float)BLOCK_HEIGHT_UNITS * 0.5f) {
		thisBall->pos.y = (float)m_game->GetBoardState().getH() * (float)BLOCK_HEIGHT_UNITS - (float)BLOCK_HEIGHT_UNITS * 0.5f;
		thisBall->direction.y *= -1.0f;
		if (deleteBlock) {
			m_soundEffectEvent({ SoundEffect::HitSide });
		}

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
			thisBall->pos.y = topOfBat;
			thisBall->pos.x = vPotentialPosition.x;
			thisBall->direction.y *= -1.0f;

			auto rotationMagnitude = (thisBall->pos.x - batX) / (batWidth / 2 + thisBall->radius);

			auto rotatedVec = glm::rotate(Vec3ToVec2(thisBall->direction), -(float)rotationMagnitude * glm::radians(35.0f));
			thisBall->direction.x = rotatedVec.x;
			thisBall->direction.y = rotatedVec.y;
			if (deleteBlock) {
				m_soundEffectEvent({ SoundEffect::HitBat });
			}
			return BallAdvanceResult::HIT_BAT;
		}

	}

	// to here
	auto oldBallDirection = thisBall->direction;


	// see https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/OneLoneCoder_PGE_CircleVsRect.cpp for collision detection algorithm


	auto currentCellTileCoords = WorldSpaceToTileCoordsFloor(thisBall->pos);

	auto targetCellTileCoords = WorldSpaceToTileCoordsRound(vPotentialPosition);

	auto zPos = targetCellTileCoords.z - 1;

	auto vAreaTL = glm::min(currentCellTileCoords, targetCellTileCoords) - glm::ivec3(1, 1, 1);
	auto vAreaBR = glm::max(currentCellTileCoords, targetCellTileCoords) + glm::ivec3(1, 1, 1);

	auto returnVal = BallAdvanceResult::HIT_NOTHING;
	glm::vec2 vRayToNearest;

	glm::ivec3 vCell;
	int numcellsChecked = 0;
	for (vCell.z = vAreaTL.z; vCell.z <= vAreaBR.z; vCell.z++) {
		for (vCell.y = vAreaTL.y; vCell.y <= vAreaBR.y; vCell.y++)
		{
			for (vCell.x = vAreaTL.x; vCell.x <= vAreaBR.x; vCell.x++)
			{
				numcellsChecked++;
				unsigned char blockCode;
				// Check if the cell is actually solid...
				if (vCell.x < 0 || vCell.y < 0) {
					continue;
				}
				if (m_game->BlockAtLocation({ vCell.x, vCell.y, vCell.z }, blockCode) == EditBlockResultCode::BLOCK_AT_SPACE)
				{
					// ...it is! So work out nearest point to future player position, around perimeter
					// of cell rectangle. We can test the distance to this point to see if we have
					// collided. 
					auto adjustedCellXY = AdjustTileCoordsToWorld(vCell);

					auto adjustedCellPlusOneXY = AdjustTileCoordsToWorld(vCell + glm::ivec3{ 1,1,1 });

					glm::vec3 vNearestPoint;
					vNearestPoint.x = std::max(adjustedCellXY.x, std::min(vPotentialPosition.x, adjustedCellPlusOneXY.x));
					vNearestPoint.y = std::max(adjustedCellXY.y, std::min(vPotentialPosition.y, adjustedCellPlusOneXY.y));
					vNearestPoint.z = std::max(adjustedCellXY.z, std::min(vPotentialPosition.z, adjustedCellPlusOneXY.z));


					glm::vec3 vRayToNearest = vNearestPoint - vPotentialPosition;
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
						if (thisBall->jumping && deleteBlock) {
							thisBall->blockCombo++;
							if (thisBall->blockCombo >= 2) {
								m_ballComboEvent({ thisBall->blockCombo, thisBall->pos.x, thisBall->pos.y, thisBall->pos.z });
							}
						}
						returnVal = BallAdvanceResult::HIT_BLOCK;
						// Statically resolve the collision
						auto newPos = vPotentialPosition - glm::normalize(vRayToNearest) * fOverlap;
						vPotentialPosition.x = newPos.x;
						vPotentialPosition.y = newPos.y;

						ReflectBall(thisBall->direction, newPos, vNearestPoint, oldBallDirection);

						// delete block (will have more sophisiticated handling for different blocks)
						unsigned char oldval;

						if (deleteBlock) {
							m_soundEffectEvent({ SoundEffect::HitBlock });
							if (m_game->RemoveBlock({ vCell.x,vCell.y,vCell.z }) != EditBlockResultCode::BLOCK_AT_SPACE) {

							}
						}
						if (thisBall->direction.z < 0) {
							// do mini jump
							thisBall->direction.z = 0;
							thisBall->jumping = true;
							thisBall->jumpTimer = 0;
							thisBall->jumpAmount = SMALL_JUMP_AMOUNT;
							thisBall->jumpTime = SMALL_JUMP_TIME;
						}
					}
				}
			}
		}
	}
	
	if(!std::isnan(vPotentialPosition.x) && !std::isnan(vPotentialPosition.y) && !std::isnan(vPotentialPosition.z)){
		thisBall->pos = vPotentialPosition;
		
	}
	else {
		thisBall->direction = oldBallDirection;
	}
	

	return returnVal;
}

void BallManager::LookAhead(const Ball* thisBall)
{
	auto ball = *thisBall;
	m_drawProjectedBalls = false;
	double jumpTimer = ball.jumpTimer;
	bool jumping;
	double jumpAmount = 5.0;
	double jumpTime = 0.0;

	Ball copiedBall = *thisBall;

	for (int i = 0; i < m_numAdvancesToCheckForBatHit; i++) {
		if (AdvanceBall(&ball, false) == BallAdvanceResult::HIT_BAT) {
			m_drawProjectedBalls = true;
			break;
		}

	}
	if (m_drawProjectedBalls) {
		for (int i = 0; i < m_lookaheadBy; i++) {
			AdvanceBall(&ball, false);
			m_lookAheadPositions[i] = ball.pos;

		}
	}
}

void BallManager::OnEvent(EngineUpdateFrameEventArgs e)
{
	size_t deletedIndices[MAX_NUM_BALLS];
	size_t numDeletedIndicies = 0;
	IterateBallList([this, &numDeletedIndicies,&deletedIndices](Ball* thisBall, Ball* lastBall, int onIteration) {
		if (thisBall->stuckToBat) {
			return true;
		}
		BallAdvanceResult lastRes;
		for (int i = 0; i < BALL_ADVANCES_PER_FRAME; i++) {
			lastRes = AdvanceBall(thisBall);
			if (lastRes == BallAdvanceResult::OUT_OF_PLAY) {
				deletedIndices[numDeletedIndicies++] = onIteration;
				break;
			}
		}
		
		if (lastRes != BallAdvanceResult::OUT_OF_PLAY) {
			LookAhead(thisBall);
		}

		return true;
	});
	for (int i = 0; i < numDeletedIndicies; i++) {
		auto indexToDelete = deletedIndices[i];
		RemoveBallAtListIndex(indexToDelete);
	}
	if (m_numBalls == 0) {
		ResetAfterDeath();
	}
}

const std::vector<SerializableProperty>& BallManager::GetSerializableProperties() const
{
	static SerializablePropertyData lookAheadByData;
	static SerializablePropertyData mostObtuseAllowedData;
	mostObtuseAllowedData.Double = m_mostObtuseAngleAllowed;
	lookAheadByData.Uint32 = m_lookaheadBy;
	static const std::vector<SerializableProperty> props = {
		{"MostObtuseAngleAllowed", SerializablePropertyType::Double, {mostObtuseAllowedData}},
		{"LookAheadBy", SerializablePropertyType::Uint32, {lookAheadByData}}
	};
	return props;
	// TODO: insert return statement here
}

bool BallManager::SetSerializableProperty(const SerializableProperty& p)
{
	if (p.name == "MostObtuseAngleAllowed") {
		if (p.type != SerializablePropertyType::Double) {
			return false;
		}
		m_mostObtuseAngleAllowed = p.data.dataUnion.Double;
		return true;
	}
	else if (p.name == "LookAheadBy") {
		if (p.type != SerializablePropertyType::Uint32) {
			return false;
		}
		m_lookaheadBy = p.data.dataUnion.Uint32;
		return true;
	}
	return false;
}

int BallManager::GetNumSerializableProperties() const
{
	return 2;
}

std::string BallManager::GetSerializableNodeName() const
{
	return "BallManager";
}

void BallManager::SaveToFile(std::string filePath) const
{
	// not implemented
}

void BallManager::LoadFromFile(std::string filePath)
{
	// not implemented

}

char* BallManager::SaveToBuffer(char* destinaion) const
{
	// not implemented

	return nullptr;
}

const char* BallManager::LoadFromBuffer(const char* source)
{
	// not implemented

	return nullptr;
}

size_t BallManager::GetBinaryFileNumBytes() const
{
	// not implemented

	return size_t();
}

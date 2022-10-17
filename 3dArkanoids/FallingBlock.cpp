#include "FallingBlock.h"
#include "Array3d.h"
#include "BlockInstanceRenderData.h"
#include <iostream>
#include "Event.h"
#include "FallingBlockFinishedEventArgs.h"

// 9.8 m/s^2 <- actual gravity
#define GRAVITY 19.6 * 4
#define GRAVITYHALVED (GRAVITY * 0.5)

FallingBlockColumn::FallingBlockColumn()
{
	
}

float CalculateFallTime(float startz, float finishz) {
	// new method trial 
	double falltime = sqrt((startz - finishz) / GRAVITYHALVED);
	// old method
	const float timeToMoveOneUnit = 0.3f;
	return falltime;// (startz - finishz) * timeToMoveOneUnit;
}


void FallingBlockColumn::StartFalling(
	BlockInstanceRenderData* fallingBlockListHead,
	const glm::vec3& fallDestinationWorldSpace,
	const glm::ivec3& fallDestinationGridCoords,
	EventListener<InternalFallingBlockFinishedEventArgs>* manager,
	size_t index
)
{
	m_falling = true;
	m_fallStartPointWorldSpace = fallingBlockListHead->worldPos;
	m_fallingColumnListHead = fallingBlockListHead;
	m_fallDestinationWorldSpace = fallDestinationWorldSpace;
	m_fallDestinationGridCoords = fallDestinationGridCoords;
	m_manager = manager;
	m_managerArrayIndex = index;
	m_fallTimeTotal = CalculateFallTime(fallingBlockListHead->worldPos.z, m_fallDestinationWorldSpace.z);
	m_fallTimeElapsed = 0.0f;
	m_velocity = 0.0;
}

void FallingBlockColumn::StopFalling()
{
	m_fallingColumnListHead->IterateOverChildBlocksAndSelf([this](BlockInstanceRenderData* workingBlock, int onZLayer) {
			auto zoffset = glm::vec3(0, 0, onZLayer * (float)BLOCK_DEPTH_UNITS);
			workingBlock->SetNewWorldPos(
				m_fallDestinationWorldSpace + zoffset
			);
		});
	m_falling = false;
	m_manager->OnEvent(InternalFallingBlockFinishedEventArgs(m_fallingColumnListHead, m_managerArrayIndex));
}


void FallingBlockColumn::Update(float deltaTime)
{
	m_fallTimeElapsed += deltaTime;
	if (m_fallTimeElapsed >= m_fallTimeTotal) {
		StopFalling();
		return;
	}

	m_fallingColumnListHead->IterateOverChildBlocksAndSelf([this](BlockInstanceRenderData* workingBlock, int onZLayer) {
			auto zoffset = onZLayer * (float)BLOCK_DEPTH_UNITS;
			auto newz = m_fallStartPointWorldSpace.z - (0.5 * GRAVITY * m_fallTimeElapsed) * m_fallTimeElapsed;
			workingBlock->SetNewWorldPos(
				glm::vec3(
					m_fallStartPointWorldSpace.x,// + zoffset,
					m_fallStartPointWorldSpace.y,// + zoffset,
					newz + zoffset//m_fallTimeElapsed / m_fallTimeTotal
				)
			);
		});	
}

bool FallingBlockColumn::IsFalling() const
{
	return m_falling;
}

size_t FallingBlockColumn::GetIndex()
{
	return m_managerArrayIndex;
}

glm::ivec3 FallingBlockColumn::GetDestinationGridCoords()
{
	return m_fallDestinationGridCoords;
}

void FallingBlockColumn::UpdateDestination(const glm::vec3& newFallDestinationWorldSpace, const glm::ivec3& newFallDestinationGridCoords)
{
	m_fallDestinationWorldSpace = newFallDestinationWorldSpace;
	m_fallDestinationGridCoords = newFallDestinationGridCoords;
	m_fallTimeTotal = CalculateFallTime(m_fallingColumnListHead->worldPos.z,newFallDestinationWorldSpace.z);
	m_fallTimeElapsed = 0;
	m_fallStartPointWorldSpace = m_fallingColumnListHead->worldPos;
}
BlockInstanceRenderData* FallingBlockColumn::GetFallingColumnListHead()
{
	return m_fallingColumnListHead;
}

#pragma once
#include <glm/glm.hpp>
#include <functional>
#include "FallingBlockFinishedEventArgs.h"


// forward declarations
template<typename T> class EventListener;
class BlockInstanceRenderData;
template<typename T> class Array3D;

typedef std::function<void(BlockInstanceRenderData*, int)> ColumnIterationDelegate;

/// <summary>
/// Represents a falling column of one or more blocks
/// that do not have a block below them.
/// </summary>
class FallingBlockColumn
{
public:
	FallingBlockColumn();
	void StartFalling(
		BlockInstanceRenderData* fallingBlockListHead,
		const glm::vec3& fallDestinationWorldSpace,
		const glm::ivec3& fallDestinationGridCoords,
		EventListener<InternalFallingBlockFinishedEventArgs>* manager,
		size_t index);
	void UpdateDestination(const glm::vec3& newFallDestinationWorldSpace, const glm::ivec3& newFallDestinationGridCoords);
	void Update(float deltaTime);
	bool IsFalling() const;
	size_t GetIndex();
	glm::ivec3 GetDestinationGridCoords();
	BlockInstanceRenderData* GetFallingColumnListHead();
private:
	void StopFalling();
private:
	BlockInstanceRenderData* m_fallingColumnListHead;
	bool m_falling = false;
	glm::vec3 m_fallStartPointWorldSpace;
	glm::vec3 m_fallDestinationWorldSpace;
	glm::ivec3 m_fallDestinationGridCoords;
	float m_fallTimeTotal;
	float m_fallTimeElapsed;
	EventListener<InternalFallingBlockFinishedEventArgs>* m_manager;
	size_t m_managerArrayIndex;
	double m_velocity = 0.0;
	double m_lastPos;
};


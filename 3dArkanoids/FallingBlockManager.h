#pragma once
#include "FallingBlock.h"
#define MAX_FALLING_COLUMNS 20
#include <vector>
#include <queue>
#include "Event.h"
#include "FallingBlockFinishedEventArgs.h"
#include "EngineUpdateFrameEventArgs.h"
#include "SizedQueue.h"
#include "MasterArrayUpdatedEventArgs.h"

enum class StartBlockFallingResult
{
	Success,
	FallingColumnsArrayFull,
	InvalidFall
};

class Game;
class FallingBlockManager :
	public EventListener<InternalFallingBlockFinishedEventArgs>, 
	public EventListener<EngineUpdateFrameEventArgs>,
	public EventListener<MasterArrayUpdatedEventArgs>
{
public:
	FallingBlockManager();

	/// <summary>
	/// subscribe this classes listener
	/// to an Event<MasterArrayUpdatedEventArgs>
	/// </summary>
	/// <param name="event"></param>
	void SubscribeAsListenerToMasterArrayUpdatedEvent(Event<MasterArrayUpdatedEventArgs>& event);

	/// <summary>
	/// Called when a column of blocks has reached its destination
	/// </summary>
	Event<FallingBlockFinishedEventArgs> BlockFinished;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="fallingColumnBase"></param>
	/// <param name="blocksArray"></param>
	/// <returns></returns>
	StartBlockFallingResult StartBlockFalling(BlockInstanceRenderData* fallingColumnBase, const Array3D<unsigned char>& blocksArray);


	// Inherited via EventListener
	virtual void OnEvent(InternalFallingBlockFinishedEventArgs e) override;
	virtual void OnEvent(EngineUpdateFrameEventArgs e) override;
	virtual void OnEvent(MasterArrayUpdatedEventArgs e) override;
private:
	bool ValidateFall(const BlockInstanceRenderData* fallingColumnHead, const Array3D<unsigned char>& blocksArray);
	glm::vec3 GetWorldspaceFallDestination(const BlockInstanceRenderData* fallingColumnBase, const glm::ivec3& gridCoordsDestination);
	glm::ivec3 GetGridCoordsFallDestination(const BlockInstanceRenderData* fallingColumnBase, const Array3D<unsigned char>& blocksArray);
	StartBlockFallingResult SetNextAvaliableIndexField();
private:
	FallingBlockColumn m_fallingColumns[MAX_FALLING_COLUMNS];
	SizedQueue<size_t, MAX_FALLING_COLUMNS> m_avaliableFallingColumnsIndexes;
	size_t m_nextAvaliableIndex;
	size_t m_numFallingColumns = 0;
};


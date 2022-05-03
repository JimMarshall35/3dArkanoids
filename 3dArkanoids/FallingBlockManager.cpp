#include "FallingBlockManager.h"
#include "BlockInstanceRenderData.h"
#include "Array3d.h"
#include "typedefs.h"
#include "PlayfieldDefs.h"

FallingBlockManager::FallingBlockManager()
{
	m_nextAvaliableIndex = 0;
	for (size_t i = 0; i < MAX_FALLING_COLUMNS; i++) {
		m_avaliableFallingColumnsIndexes.Enqueue(i);
	}
}

void FallingBlockManager::SubscribeAsListenerToMasterArrayUpdatedEvent(Event<MasterArrayUpdatedEventArgs>& event)
{
	event += this;
}

StartBlockFallingResult FallingBlockManager::StartBlockFalling(BlockInstanceRenderData* fallingColumnBase, const Array3D<unsigned char>& blocksArray)
{
	using namespace glm;


	if (!ValidateFall(fallingColumnBase, blocksArray)) {
		std::cout << "Invalid fall" << std::endl;
		return StartBlockFallingResult::InvalidFall;
	}

	if (SetNextAvaliableIndexField() == StartBlockFallingResult::FallingColumnsArrayFull)
		return StartBlockFallingResult::FallingColumnsArrayFull;

	var destinationGridCoords = GetGridCoordsFallDestination(fallingColumnBase, blocksArray);
	var destinationWorldSpace = GetWorldspaceFallDestination(fallingColumnBase, destinationGridCoords);
	m_fallingColumns[m_nextAvaliableIndex].StartFalling(
		fallingColumnBase,
		destinationWorldSpace,  
		destinationGridCoords,
		this,
		m_nextAvaliableIndex
	);
	
	return StartBlockFallingResult::Success;

}

bool FallingBlockManager::ValidateFall(const BlockInstanceRenderData* fallingColumnHead, const Array3D<unsigned char>& blocksArray)
{
	using namespace glm;
	const var& gridCoords = fallingColumnHead->atGridCoords;
	auto blockBelow = blocksArray[gridCoords + ivec3(0, 0, -1)];
	if (blockBelow != 0x00) {
		return false; // there is a block directly underneath 
	}
	return true;
}

glm::vec3 FallingBlockManager::GetWorldspaceFallDestination(const BlockInstanceRenderData* fallingColumnBase, const glm::ivec3& gridCoordsDestination)
{
	float fallAmountUnits = fallingColumnBase->worldPos.z - (gridCoordsDestination.z * (float)BLOCK_DEPTH_UNITS);

	return glm::vec3(
		fallingColumnBase->worldPos.x,
		fallingColumnBase->worldPos.y,
		fallingColumnBase->worldPos.z - fallAmountUnits
	);
}

glm::ivec3 FallingBlockManager::GetGridCoordsFallDestination(const BlockInstanceRenderData* fallingColumnBase, const Array3D<unsigned char>& blocksArray)
{
	using namespace glm;
	var fallStart = fallingColumnBase->atGridCoords;
	do {
		fallStart.z -= 1;
	}
	while (fallStart.z >= 0 &&  // todo - rewrite this whole function
		(blocksArray[fallStart] == 0x00 || (fallStart.z-1 >= 0 && (blocksArray[glm::ivec3(fallStart.x, fallStart.y, fallStart.z-1)] == 0x00))));
	fallStart.z += 1;
	return fallStart;
}

StartBlockFallingResult FallingBlockManager::SetNextAvaliableIndexField()
{
	if (m_avaliableFallingColumnsIndexes.IsEmpty()) {
		return StartBlockFallingResult::FallingColumnsArrayFull;
	}
	m_nextAvaliableIndex = m_avaliableFallingColumnsIndexes.Dequeue();
	return StartBlockFallingResult::Success;
}

void FallingBlockManager::OnEvent(MasterArrayUpdatedEventArgs e)
{
	auto finishedColumnIndex = e.GetFallingBlockManagerBlockIndex();
	auto& finishedColumn = m_fallingColumns[finishedColumnIndex];
	const auto& finishedColGridCoords = finishedColumn.GetDestinationGridCoords();
	m_avaliableFallingColumnsIndexes.Enqueue(finishedColumnIndex);
	for (int i = 0; i < MAX_FALLING_COLUMNS; i++) {
		if (i == finishedColumnIndex)
			continue;
		auto& otherColumn = m_fallingColumns[i];
		const auto& otherGridCoords = otherColumn.GetDestinationGridCoords();
		if (otherColumn.IsFalling() && 
			otherGridCoords.x == finishedColGridCoords.x &&
			otherGridCoords.y == finishedColGridCoords.y) {
			auto instanceRenderData = otherColumn.GetFallingColumnListHead();
			auto newGridCoords = GetGridCoordsFallDestination(instanceRenderData, *e.GetPlayFieldArray());
			auto newWorldCoords = GetWorldspaceFallDestination(instanceRenderData, newGridCoords);
			otherColumn.UpdateDestination(newWorldCoords, newGridCoords);
		}
	}
}

void FallingBlockManager::OnEvent(EngineUpdateFrameEventArgs e)
{
	for (int i = 0; i < MAX_FALLING_COLUMNS; i++) {
		auto& fallingCol = m_fallingColumns[i];
		if (fallingCol.IsFalling()) {
			fallingCol.Update(e.DeltaTime);
		}
	}
}

void FallingBlockManager::OnEvent(InternalFallingBlockFinishedEventArgs e)
{
	e.WrappedEvent.FinishedGridCoords = m_fallingColumns[e.ManagerBlockArrayIndex].GetDestinationGridCoords();
	BlockFinished(e.WrappedEvent);
}

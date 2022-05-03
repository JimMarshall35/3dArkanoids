#pragma once

#include <glm/glm.hpp>
class BlockInstanceRenderData;
struct FallingBlockFinishedEventArgs {
	FallingBlockFinishedEventArgs(BlockInstanceRenderData* e)
		: FinishedBlock(e){
			}
	/// <summary>
	/// rendering data for the finished block
	/// </summary>
	BlockInstanceRenderData* FinishedBlock;

	/// <summary>
	/// the grid coords in the master array
	/// at which the block has come to rest after falling
	/// </summary>
	glm::ivec3 FinishedGridCoords;

	/// <summary>
	/// index of the fallingBlockColumn object in FallingBlockManager's array
	/// To be passed back with MasterArrayUpdated after the games masterarray has been updated
	/// so that the memory slot for that block can be recycled
	/// </summary>
	size_t ManagerBlockArrayIndex;
};

struct InternalFallingBlockFinishedEventArgs {
	InternalFallingBlockFinishedEventArgs(const FallingBlockFinishedEventArgs& e, size_t index)
		: WrappedEvent(e), ManagerBlockArrayIndex(index) {
		WrappedEvent.ManagerBlockArrayIndex = index;

	}
	FallingBlockFinishedEventArgs WrappedEvent;
	size_t ManagerBlockArrayIndex;
};


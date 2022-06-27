#pragma once
#include <memory>
#include <functional>
#include <glm/glm.hpp>
#include "PlayfieldDefs.h"


class IRenderer;
class BlockInstanceRenderData {
public:
	BlockInstanceRenderData() {

	}
	BlockInstanceRenderData(glm::vec3 worldPos, glm::vec4 colour, bool shouldRender = true)
		:worldPos(worldPos), colour(colour), shouldRender(shouldRender)
	{

	}
	void IterateOverChildBlocksAndSelf(std::function<void(BlockInstanceRenderData*, int)> delegateFunc) {
		int onZLayer = 0;
		auto workingBlock = this;
		do {
			delegateFunc(workingBlock, onZLayer);
			workingBlock = workingBlock->child;
			onZLayer++;
		} while (workingBlock != nullptr);
	}
	glm::vec3 worldPos;
	glm::vec4 colour;
	glm::vec3 dims = { BLOCK_WIDTH_UNITS, BLOCK_HEIGHT_UNITS, BLOCK_DEPTH_UNITS };
	glm::ivec3 atGridCoords;
	size_t renderDataArrayIndex;
	BlockInstanceRenderData* child = nullptr; // the block above it
	std::shared_ptr<IRenderer> myRenderer = nullptr;
	bool shouldRender = true;
	void SetShouldRender(bool newval);
	void SetNewWorldPos(const glm::vec3& val);
	
};
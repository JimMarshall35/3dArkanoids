#pragma once
#include <glm/glm.hpp>
#include <string>
class Camera;
class BlockInstanceRenderData;
class IRenderer {
public:
	virtual void DrawCuboid(const glm::vec3& centeredAt, const glm::vec3& dimensions, const Camera& camera, const glm::vec3& colour) const  = 0;
	virtual void SetLightPos(const glm::vec3& value) = 0;
	virtual void SetLightColour(const glm::vec3& value) = 0;
	virtual void SetScreenDims(const glm::ivec2& value) = 0;
	virtual void DrawInstancedBlocks(const size_t numberToDraw, const Camera& camera) = 0;
	virtual void SetInstancedBlocksUbo(const BlockInstanceRenderData* instances, const size_t numberToDraw) = 0;
	virtual void SetCubeShouldRender(size_t indexCubeIsAt, bool newValue) = 0;
	virtual void SetCubePos(size_t indexCubeIsAt, const glm::vec3& newPos) = 0;
	virtual void SetCubeColour(size_t indexCubeIsAt, const glm::vec3& newColour) = 0;
	virtual void DrawTextAnchoredToBottomLeft(std::string text, float x, float y, float scale, glm::vec3 colour) const = 0;
	virtual void DrawTextAnchoredToTopLeft(std::string text, float xOffset, float yOffset, float scale, glm::vec3 colour) const = 0;

};
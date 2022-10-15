#pragma once
#include <glm/glm.hpp>
#include <string>
class Camera;
class BlockInstanceRenderData;
class IRenderer {
public:
	virtual void DrawCuboid(const glm::vec3& centeredAt, const glm::vec3& dimensions, const Camera& camera, const glm::vec3& colour) const  = 0;
	virtual void DrawSphere(const glm::vec3& centeredAt, const glm::vec3& dimensions, const Camera& camera, const glm::vec4& colour) const = 0;
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
	virtual void DrawTextAnchoredToTopRight(std::string text, float xOffset, float yOffset, float scale, glm::vec3 colour) const = 0;
	virtual void DrawTextAnchoredToBottomRight(std::string text, float xOffset, float yOffset, float scale, glm::vec3 colour) const = 0;

	// load a texture for 1x2 blocks 
	/*
	* 
	* assumes the textures for the blocks are laid out like this:
	*  _ __
	* |_|__|
	* |_|__|
	* |_|__|
	* |_|__|
	* 
	* ie one type of block per row of the texture with first the square end piece
	* and then the long rectangle piece.
	*/
	virtual void DesignateOneByTwoBlocksTexture(const std::string& blocksTextureIdentifier, int numBlocks) = 0;
	virtual void DrawTexturedOneByTwoInstancedBlocks(const size_t numberToDraw, const Camera& camera) = 0;
	virtual void SetInstancedTexturedBlocksUbo(const BlockInstanceRenderData* instances, const size_t numberToSet) = 0;
	virtual glm::vec2 getUvOffsetToNextOneByTwoBlock() = 0;

	virtual void DrawBillboard(const glm::vec3& woldPos, const glm::vec2& billboardSize, const Camera& camera, const std::string& identifier) const = 0;
	virtual unsigned int GetBaseTextSize() = 0;
	virtual float GetTextWidth(float scale, std::string text) = 0;
};
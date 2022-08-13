#pragma once
#include "Array3d.h"
#include "Array3DMap.h"
#include <glm/glm.hpp>
#include "BlockInstanceRenderData.h"
#include <memory>
#include "FallingBlockManager.h"
#include <iostream>
#include "EngineUpdateFrameEventArgs.h"
#include "MasterArrayUpdatedEventArgs.h"
#include "ILevelEditorServerGame.h"
#include "LevelEditorServerFactoryTypedef.h"
#include "SizedQueue.h"
#include "FrameMemoryAllocator.h"
#include "GameBlockTypes.h"
#include "Bat.h"
#include "Ball.h"


#define MAX_NUM_BALLS 50
class ILevelLoader;
class IRenderer;
class Camera;
class GameInput;

class Game : public EventListener<FallingBlockFinishedEventArgs>,
	public ILevelEditorServerGame
{
public:
	Game(ILevelLoader* levelLoader, IRenderer* renderer, LevelEditorServerFactory levelEditorServerFactory);
	void Draw(const Camera& camera);
	void SetScreenDims(const glm::ivec2& screenDims);
	void Update(float deltaT);
	int IndexOfRenderDataAt(const glm::ivec3& coords);
	void SaveLevelTest(std::string filePath);
	void RecieveGameInput(const GameInput& gameInput);
private:
	void InitializeRenderData();
	bool LinkAndValidateBlocksRenderData();
	glm::vec4 GetColourFromByteValue(const unsigned char byteCode);
private:
	SizedQueue<size_t, MAX_INSTANCES> m_deadBlockIndices;
	std::vector<BlockInstanceRenderData> m_blockRenderData = std::vector<BlockInstanceRenderData>(MAX_INSTANCES);
	size_t m_currentNumBlocks;
	Event<EngineUpdateFrameEventArgs> m_frameUpdateEvent;
	Event<MasterArrayUpdatedEventArgs> m_masterArrayUpdatedEvent;
	Array3D<unsigned char> m_playFieldArray;
	std::unique_ptr<ILevelLoader> m_levelLoader;
	std::unique_ptr<ILevelEditorServer> m_levelEditorServer;
	std::shared_ptr<IRenderer> m_renderer;
	FallingBlockManager m_fallingBlockManager;
	GameBlockTypes m_gameBlockTypes;

	Bat m_bat;
	Ball m_balls[MAX_NUM_BALLS];
	int m_currentNumBalls = 0;

	FrameMemoryAllocator m_frameAllocator;

	// Inherited via EventListener
	virtual void OnEvent(FallingBlockFinishedEventArgs e) override;

public:


	// Inherited via ILevelEditorServerGame
	virtual const Array3D<unsigned char>& GetBoardState() override;

	virtual EditBlockResultCode AddOrChangeBlock(const glm::ivec3& point, unsigned char newVal, unsigned char& oldBlockVal) override;

	virtual EditBlockResultCode RemoveBlock(const glm::ivec3& point) override;

	virtual SetBoardDescriptionResultCPP SetBoardState(const Array3D<unsigned char>& newState) override;

	virtual EditBlockResultCode BlockAtLocation(const glm::ivec3& point, unsigned char& blockCode) override;

	virtual std::vector<BlockTypeDescriptionEditor> GetPossibleBlocks() override;

	virtual void SetPossibleBlocks(const std::vector<BlockTypeDescriptionEditor>& possibleBlocks) override;

};
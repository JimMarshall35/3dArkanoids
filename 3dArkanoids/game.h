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
#include "GameFramework.h"
#include "BallManager.h"

#define MAX_NUM_BALLS 50
class ILevelLoader;
class IRenderer;
class Camera;
class GameInput;

class Game : 
	public EventListener<FallingBlockFinishedEventArgs>,
	public ILevelEditorServerGame,
	public UpdateableLayerBase,
	public DrawableLayerBase,
	public RecieveInputLayerBase
{
public:
	// implementation of IDrawableLayer
	virtual void Draw(const Camera& camera) const override;
	virtual bool MasksPreviousDrawableLayer() const override {
		return true;
	}
	virtual std::string GetDrawableLayerName() const override {
		return "Gameplay";
	}
	// implementation of IUpdateableLayer
	virtual void Update(float deltaT) override;
	virtual bool MasksPreviousUpdateableLayer() const override {
		return true;
	}
	virtual std::string GetUpdateableLayerName() const override {
		return "Gameplay";
	}
	// implementation of IRecieveInputLayer
	virtual void ReceiveInput(const GameInput& gameInput) override;
	virtual bool MasksPreviousInputLayer() const override {
		return true;
	}
	virtual std::string GetInputLayerName() const override {
		return "Gameplay";
	}

	Game(const std::shared_ptr<IRenderer>& renderer, LevelEditorServerFactory levelEditorServerFactory);
	void SetScreenDims(const glm::ivec2& screenDims);
	int IndexOfRenderDataAt(const glm::ivec3& coords);
	void SaveLevelTest(std::string filePath);
	void Init();
	void InitializeRenderData();
private:
	bool LinkAndValidateBlocksRenderData();
	

	glm::vec4 GetColourFromByteValue(const unsigned char byteCode);
	glm::vec2 GetUvOffsetFromByteValue(const unsigned char byteCode);
private:
	SizedQueue<size_t, MAX_INSTANCES> m_deadBlockIndices;
	std::vector<BlockInstanceRenderData> m_blockRenderData = std::vector<BlockInstanceRenderData>(MAX_INSTANCES);
	size_t m_currentNumBlocks;
	Event<EngineUpdateFrameEventArgs> m_frameUpdateEvent;
	Event<MasterArrayUpdatedEventArgs> m_masterArrayUpdatedEvent;
	Array3D<unsigned char> m_playFieldArray;
	std::unique_ptr<ILevelEditorServer> m_levelEditorServer;
	std::shared_ptr<IRenderer> m_renderer;
	FallingBlockManager m_fallingBlockManager;
	GameBlockTypes m_gameBlockTypes;
	BallManager m_ballManager;
	Bat m_bat;
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
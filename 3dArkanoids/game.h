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

class ILevelLoader;
class IRenderer;
class Camera;


class Game : public EventListener<FallingBlockFinishedEventArgs>,
	public ILevelEditorServerGame
{
public:
	Game(ILevelLoader* levelLoader, IRenderer* renderer, LevelEditorServerFactory levelEditorServerFactory);
	void Draw(const Camera& camera);
	void SetScreenDims(const glm::ivec2& screenDims);
	void Update(float deltaT);
	int IndexOfRenderDataAt(const glm::ivec3& coords);

private:
	void InitializeRenderData();
	bool LinkAndValidateBlocksRenderData();
private:
	BlockInstanceRenderData m_blockRenderData[MAX_POSSIBLE_BLOCKS];
	size_t m_currentNumBlocks;
	Event<EngineUpdateFrameEventArgs> m_frameUpdateEvent;
	Event<MasterArrayUpdatedEventArgs> m_masterArrayUpdatedEvent;
private:
	Array3D<unsigned char> m_playFieldArray;
	std::unique_ptr<ILevelLoader> m_levelLoader;
	std::unique_ptr<ILevelEditorServer> m_levelEditorServer;
	std::shared_ptr<IRenderer> m_renderer;
	FallingBlockManager m_fallingBlockManager;

	

	// Inherited via EventListener
	virtual void OnEvent(FallingBlockFinishedEventArgs e) override;

public:


	// Inherited via ILevelEditorServerGame
	virtual const Array3D<unsigned char>& GetBoardState() override;

	virtual EditBlockResultCode AddOrChangeBlock(const glm::ivec3& point, unsigned char newVal, unsigned char& oldBlockVal) override;

	virtual EditBlockResultCode RemoveBlock(const glm::ivec3& point) override;

	virtual SetBoardDescriptionResultCPP SetBoardState(const Array3D<unsigned char>& newState) override;

	virtual EditBlockResultCode BlockAtLocation(const glm::ivec3& point, unsigned char& blockCode) override;

};
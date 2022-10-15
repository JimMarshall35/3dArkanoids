#pragma once
#include "Array3d.h"
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
#include "VisualEffectsManager.h"
#include "SoundEffectsManager.h"
#include "CameraSpline.h"
#include "SystemAbstractionTypedefs.h"

#define MAX_NUM_BALLS 50

enum class GameState {
	IN_INTRO,
	PLAYING,

};
class ILevelLoader;
class IRenderer;
class IAudioPlayer;
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
	virtual void OnDrawablePush() override;
	virtual void OnDrawablePop() override;

	// implementation of IUpdateableLayer
	virtual void Update(float deltaT) override;
	virtual bool MasksPreviousUpdateableLayer() const override {
		return true;
	}
	virtual std::string GetUpdateableLayerName() const override {
		return "Gameplay";
	}
	virtual void OnUpdatePush() override;
	virtual void OnUpdatePop() override;

	// implementation of IRecieveInputLayer
	virtual void ReceiveInput(const GameInput& gameInput) override;
	virtual bool MasksPreviousInputLayer() const override {
		return true;
	}
	virtual std::string GetInputLayerName() const override {
		return "Gameplay";
	}
	virtual void OnInputPush() override;
	virtual void OnInputPop() override;

	Game(const std::shared_ptr<IRenderer>& renderer, std::shared_ptr<IAudioPlayer> audioPlayer, LevelEditorServerFactory levelEditorServerFactory, Camera* cam, ToggleCursorFunc toggleCursor);
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
	std::shared_ptr<IAudioPlayer> m_audioPlayer;
	FallingBlockManager m_fallingBlockManager;
	GameBlockTypes m_gameBlockTypes;
	BallManager m_ballManager;
	VisualEffectsManager m_visualEffectsManager;
	SoundEffectsManager m_soundEffectsManager;
	Bat m_bat;
	CameraSpline m_camManager;
	GameState m_state = GameState::IN_INTRO;
	ToggleCursorFunc m_toggleCursor;
	
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
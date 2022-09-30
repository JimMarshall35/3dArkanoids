#include "game.h"
#include <iostream>
#include <algorithm>

#include "BlockColourDefs.h"
#include "IRenderer.h"
#include "ILevelEditorServer.h"
#include "GameInput.h"
#include "GameToUiMessage.h"

size_t m_currentNumBlocks = 0;

Game::Game(const std::shared_ptr<IRenderer>& renderer, LevelEditorServerFactory levelEditorServerFactory)
{
	m_renderer = renderer;
	m_levelEditorServer = levelEditorServerFactory(this);
	
	
}

/// <summary>
/// 
/// </summary>
/// <param name="camera"></param>
void Game::Draw(const Camera& camera) const
{
	m_renderer->DrawCuboid( 
		{ 
			(m_playFieldArray.getW() * BLOCK_WIDTH_UNITS) / 2.0f - BLOCK_WIDTH_UNITS * 0.5f, 
			((m_playFieldArray.getH() * BLOCK_HEIGHT_UNITS) / 2.0f - BLOCK_HEIGHT_UNITS * 0.5f) - m_bat.GetDistanceFromFirstRow()*0.5f, 
			-(BLOCK_DEPTH_UNITS * 0.5f) - 1.0f 
		},
		{ 
			m_playFieldArray.getW() * BLOCK_WIDTH_UNITS ,
			(m_playFieldArray.getH() * BLOCK_HEIGHT_UNITS) + m_bat.GetDistanceFromFirstRow() ,
			1.0f
		},
		camera,
		{ 0.3,0.3,0.3 }
	);
	m_renderer->DrawTexturedOneByTwoInstancedBlocks(m_currentNumBlocks, camera);
	m_bat.Draw(m_renderer.get(), camera);
	m_ballManager.Draw(m_renderer.get(), camera);
}

/// <summary>
/// 
/// </summary>
/// <param name="screenDims"></param>
void Game::SetScreenDims(const glm::ivec2& screenDims)
{
	m_renderer->SetScreenDims(screenDims);
}

/// <summary>
/// 
/// </summary>
/// <param name="deltaT"></param>
void Game::Update(float deltaT)
{
	m_frameUpdateEvent(EngineUpdateFrameEventArgs{ deltaT });
	
}



/// <summary>
/// Initializes the rendering data from the array of
/// bytes loaded into m_playFieldArray by the level loader.
/// 
/// This is information about the blocks that need rendering <cref>BlockInstanceRenderData</cref>
/// </summary>
void Game::InitializeRenderData()
{
	m_currentNumBlocks = 0;
	for (int z = 0; z < PLAYFIELD_DEPTH_BLOCKS; z++) {
		for (int y = 0; y < PLAYFIELD_HEIGHT_BLOCKS; y++) {
			for (int x = 0; x < PLAYFIELD_WIDTH_BLOCKS; x++) {
				const unsigned char& byteAt = m_playFieldArray[x][y][z];
				if (byteAt == 0x00) continue;
				auto colour = glm::vec4(GetColourFromByteValue(byteAt));
				glm::vec3 worldPos(
					x * BLOCK_WIDTH_UNITS,
					y * BLOCK_HEIGHT_UNITS,
					z * BLOCK_DEPTH_UNITS
				);
				auto renderData = BlockInstanceRenderData(worldPos, colour, GetUvOffsetFromByteValue(byteAt));
				
				renderData.atGridCoords = glm::ivec3(x, y, z);                    // this blocks position in grid coordinates
				renderData.renderDataArrayIndex = m_currentNumBlocks;
				renderData.myRenderer = m_renderer;
				m_blockRenderData[m_currentNumBlocks++] = renderData;

			}
		}
	}

	// TODO - to fix alpha blending issues sort the blocks at this point. https://learnopengl.com/Advanced-OpenGL/Blending#:~:text=Advanced%2DOpenGL%2FBlending,behind%20it%20with%20varying%20intensity.

	//m_renderer->SetInstancedBlocksUbo(m_blockRenderData.data(), m_currentNumBlocks); // TODO rename SetInstancedBlocksUbo to something non openGL specific like InitializeInstancedBlocks
	m_renderer->SetInstancedTexturedBlocksUbo(m_blockRenderData.data(), m_currentNumBlocks);
}

/// <summary>
/// returns the index of the render data that matches the coords specified, -1 if not found
/// </summary>
/// <param name="coords"></param>
/// <returns>the index of the render data that matches the coords specified, -1 if not found</returns>
int Game::IndexOfRenderDataAt(const glm::ivec3& coords)
{
	for (int i = 0; i < m_currentNumBlocks; i++) {
		const auto& rd = m_blockRenderData[i];
		if (rd.atGridCoords == coords && rd.shouldRender) {
			return i;
		}
	}
	return -1;
}

void Game::SaveLevelTest(std::string filePath)
{
	m_playFieldArray.SaveToFile(filePath);
}

void Game::ReceiveInput(const GameInput& gameInput)
{
	// todo : sort this weirdness out. this function should just be the last line
	// this if check should go in m_ballManager.RecieveInput
	if (gameInput.Firing) {
		m_ballManager.ReleaseBalls();
	}
	m_ballManager.ReceiveInput(m_bat.RecieveInput(gameInput));
}

/// <summary>
/// returns the maximum stack size of blocks
/// </summary>
/// <param name="renderData"></param>
/// <param name="numBlocks"></param>
/// <returns></returns>
int GetMaxNumberOfChildren(BlockInstanceRenderData* renderData, size_t numBlocks) {
	int maxChildren = 0;
	for (int i = 0; i < numBlocks; i++) {
		auto& rd = renderData[i];
		int currentChildren = 0;
		while (rd.child != nullptr) {
			currentChildren++;
			rd = *rd.child;
		}
		if (currentChildren > maxChildren) {
			maxChildren = currentChildren;
		}
	}
	return maxChildren;
}

/// <summary>
/// Arranges the blocks into linked lists representing columns
/// along the z axis.
/// </summary>
/// <returns>
/// whether the initial arrangement is valid ie there are no free floating blocks
/// </returns>
bool Game::LinkAndValidateBlocksRenderData()
{
	bool valid = true;
	std::vector<BlockInstanceRenderData*> floatingBlocks;
	for (int i = 0; i < m_currentNumBlocks; i++) {
		auto& rd = m_blockRenderData[i];
		int indexOfBlockAbove = IndexOfRenderDataAt(glm::ivec3(rd.atGridCoords.x, rd.atGridCoords.y, rd.atGridCoords.z + 1));
		if (indexOfBlockAbove >= 0) {
			rd.child = &m_blockRenderData[indexOfBlockAbove];
		}
		if (rd.atGridCoords.z > 0) {
			int indexOfBlockBelow = IndexOfRenderDataAt(glm::ivec3(rd.atGridCoords.x, rd.atGridCoords.y, rd.atGridCoords.z - 1));
			if (indexOfBlockBelow < 0) {
				// a free floating block - the blocks z is higher than 0 
				// but there is no block below
				floatingBlocks.push_back(&rd);
				valid = false;
			}
		}
		
	}
	for (auto block : floatingBlocks) {
		m_fallingBlockManager.StartBlockFalling(block, m_playFieldArray);
	}
	return valid;
	//int maxChildren = GetMaxNumberOfChildren(m_blockRenderData, m_currentNumBlocks);
	//std::cout << "Highest stack of blocks is " << maxChildren + 1 << std::endl;
}

glm::vec4 Game::GetColourFromByteValue(const unsigned char byteCode)
{
	return m_gameBlockTypes[byteCode].Appearance.Colour;
}


void Game::OnEvent(FallingBlockFinishedEventArgs e)
{
	auto finishedColStart = e.FinishedBlock->atGridCoords;
	auto finishedColDestination = e.FinishedGridCoords;
	auto movementAmount = finishedColStart.z - finishedColDestination.z;

	e.FinishedBlock->IterateOverChildBlocksAndSelf([this, finishedColStart, movementAmount](BlockInstanceRenderData* workingBlock, int onZLayer) {
			auto cutIndex = finishedColStart + glm::ivec3(0, 0, onZLayer);
			auto cutValue = m_playFieldArray[cutIndex];
			m_playFieldArray[cutIndex] = 0x00;
			auto pasteIndex = glm::ivec3(cutIndex.x, cutIndex.y, cutIndex.z - movementAmount);
			m_playFieldArray[pasteIndex] = cutValue;
			workingBlock->atGridCoords = pasteIndex;
			if (pasteIndex.z > 0) {
				// set the block that has just landed as the 
				// child of the one it has landed on
				int indexOfLandedBlocksColumn = IndexOfRenderDataAt(pasteIndex);
				int indexOfNewParent = IndexOfRenderDataAt(glm::ivec3(pasteIndex.x, pasteIndex.y, pasteIndex.z - 1));
				m_blockRenderData[indexOfNewParent].child = &m_blockRenderData[indexOfLandedBlocksColumn]; 
			}

		});
	m_masterArrayUpdatedEvent(MasterArrayUpdatedEventArgs(&m_playFieldArray, e.ManagerBlockArrayIndex));
	m_levelEditorServer->NotifyNewBoardState(m_playFieldArray);
}

const Array3D<unsigned char>& Game::GetBoardState()
{
	// this function is only called by editor server and is the first thing called
	
	return m_playFieldArray;
}

EditBlockResultCode Game::AddOrChangeBlock(const glm::ivec3& point, unsigned char newVal, unsigned char& oldBlockVal)
{

	using namespace glm;

	if (newVal > BLOCK_BYTE_MAX ) {
		return INVALID_NEW_BYTE;
	}
	if (point.x < 0
		|| point.x >= m_playFieldArray.getW()
		|| point.y < 0
		|| point.y >= m_playFieldArray.getH()
		|| point.z < 0
		|| point.z >= m_playFieldArray.getD()) {
		return FAILURE_POINT_OUT_OF_BOUNDS;
	}
	if (m_playFieldArray[point] == newVal) {
		return NO_CHANGE;
	}
	auto returnedResult = SPACE_EMPTY;

	
	if (IndexOfRenderDataAt(point) >= 0) {
		oldBlockVal = m_playFieldArray[point];
		returnedResult = BLOCK_AT_SPACE;
	}

	m_playFieldArray[point] = newVal;
	// obviously not efficient to always re-create the entire board
	// but will do for now
	InitializeRenderData();
	LinkAndValidateBlocksRenderData();
	return returnedResult;
}

EditBlockResultCode Game::RemoveBlock(const glm::ivec3& point)
{
	if (point.x < 0
		|| point.x >= m_playFieldArray.getW()
		|| point.y < 0
		|| point.y >= m_playFieldArray.getH()
		|| point.z < 0
		|| point.z >= m_playFieldArray.getD()) {
		return FAILURE_POINT_OUT_OF_BOUNDS;
	}
	m_playFieldArray[point] = 0x00;
	int index = IndexOfRenderDataAt(point);
	if (index < 0) {
		return OTHER_FAILURE;
	}

	// set the block below's child to null
	if (point.z > 0) {
		int below = IndexOfRenderDataAt(point - glm::ivec3{ 0, 0, 1 });
		if (below >= 0) {
			m_blockRenderData[below].child = nullptr;
		}
		else {
			std::cout << "breakpoint\n";
		}
	}
	 
	auto& renderData = m_blockRenderData[index];
	renderData.SetShouldRender(false);

	// start the block above falling
	if (renderData.child != nullptr) {
		m_fallingBlockManager.StartBlockFalling(renderData.child, m_playFieldArray);
	}
	m_levelEditorServer->NotifyNewBoardState(m_playFieldArray);
	return BLOCK_AT_SPACE;
}

SetBoardDescriptionResultCPP Game::SetBoardState(const Array3D<unsigned char>& newState)
{
	m_playFieldArray = newState;
	InitializeRenderData();
	LinkAndValidateBlocksRenderData();

	return SUCCESS();
}

void Game::Init()
{
	m_fallingBlockManager.BlockFinished += this;
	m_fallingBlockManager.SubscribeAsListenerToMasterArrayUpdatedEvent(m_masterArrayUpdatedEvent);
	m_frameUpdateEvent += &m_fallingBlockManager;
	m_frameUpdateEvent += m_levelEditorServer.get();

	float LightPosZMultiplier = 5.0f; // this multiplied by height of the board is the lights z position
	m_renderer->SetLightPos(
		glm::vec3(
			((float)PLAYFIELD_WIDTH_BLOCKS / 2.0f) * (float)BLOCK_WIDTH_UNITS,
			((float)PLAYFIELD_HEIGHT_BLOCKS / 2.0f) * (float)BLOCK_HEIGHT_UNITS,
			(((float)PLAYFIELD_DEPTH_BLOCKS / 2.0f) * (float)BLOCK_DEPTH_UNITS) * LightPosZMultiplier)
	);
	m_renderer->SetLightColour(glm::vec3(1.0, 1.0, 1.0));

	//m_gameBlockTypes.Add(BlockTypeDescription{ glm::vec4{ 0.184f, 0.176f, 0.803f, 1.0f }, glm::vec2{ 0.0f,0.0f } });
	//m_gameBlockTypes.Add(BlockTypeDescription{ glm::vec4{ 1, 0, 0, 1 }, GetUvOffsetFromByteValue(2) });
	//m_gameBlockTypes.Add(BlockTypeDescription{ glm::vec4{ 0, 1, 0, 1 }, GetUvOffsetFromByteValue(3) });
	//m_gameBlockTypes.Add(BlockTypeDescription{ glm::vec4{ 0.094, 0.949, 0.898, 0.2 }, GetUvOffsetFromByteValue(4) });
	//m_gameBlockTypes.Add(BlockTypeDescription{ glm::vec4{ 0.98, 0.878, 0, 1 }, GetUvOffsetFromByteValue(5) });
	//m_gameBlockTypes.Add(BlockTypeDescription{ glm::vec4{ 1, 1, 1, 1 }, GetUvOffsetFromByteValue(6) });
	//m_gameBlockTypes.LoadFromFile("GameBlockTypesNew.jim");



	//m_playFieldArray.LoadFromFile("Level.jim");

	int w = m_playFieldArray.getW();

	m_bat.SetMinAndMaxXPos(0.0 - BLOCK_WIDTH_UNITS * 0.5f, ((w * BLOCK_WIDTH_UNITS) - BLOCK_WIDTH_UNITS) + BLOCK_WIDTH_UNITS * 0.5f);

	m_ballManager.Init(
		this,
		m_frameUpdateEvent,
		&m_bat);

	const auto ballStartingY =
		-(m_bat.GetDistanceFromFirstRow() + BLOCK_DEPTH_UNITS * 0.5f) + DEFAULT_BALL_RADIUS + (m_bat.GetDepthAndHeight().x * 0.5f);

	m_ballManager.AddBall(
		{ m_bat.GetXPos() , ballStartingY, 0 },
		glm::normalize(glm::vec3{ -0.5,1,0 }),
		true);

	InitializeRenderData();
	bool isValid = LinkAndValidateBlocksRenderData();
	if (!isValid) {

	}
}

glm::vec2 Game::GetUvOffsetFromByteValue(const unsigned char byteCode)
{

	return m_gameBlockTypes[byteCode].Appearance.AtlasUvOffset;
}

EditBlockResultCode Game::BlockAtLocation(const glm::ivec3& point, unsigned char& blockCode)
{
	if (point.x < 0
		|| point.x >= m_playFieldArray.getW()
		|| point.y < 0
		|| point.y >= m_playFieldArray.getH()
		|| point.z < 0
		|| point.z >= m_playFieldArray.getD()) {
		return EditBlockResultCode::FAILURE_POINT_OUT_OF_BOUNDS;
	}
	blockCode = m_playFieldArray[point];
	if (blockCode == 0x00) {
		return EditBlockResultCode::SPACE_EMPTY;
	}
	else {
		return EditBlockResultCode::BLOCK_AT_SPACE;
	}
}

std::vector<BlockTypeDescriptionEditor> Game::GetPossibleBlocks()
{
	std::vector<BlockTypeDescriptionEditor> returnedVector;
	auto nextBlockTypeIndex = m_gameBlockTypes.GetNextIndexToAdd();
	for (unsigned char i = 1; i < nextBlockTypeIndex; i++)
	{
		returnedVector.push_back(BlockTypeDescriptionEditor{ i, m_gameBlockTypes[i].Appearance.Colour });
	}
	return returnedVector;
}

void Game::SetPossibleBlocks(const std::vector<BlockTypeDescriptionEditor>& possibleBlocks)
{
	assert(possibleBlocks.size() <= 256);
	m_gameBlockTypes.Clear();
	for (const auto& block : possibleBlocks) {
		m_gameBlockTypes.Add(
			block.ToBlockTypeDescription()
		);
	}
}






#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>
struct BlockAppearance
{
	glm::vec4 Colour;
};

struct BlockTypeDescription {
	// will also contain gamplay characteristics of the block
	BlockAppearance Appearance;
};

class GameBlockTypes
{
public:
	BlockTypeDescription& operator[](unsigned char b);
	void Add(const BlockTypeDescription& blockTypeDescription);
	void Clear();
	void SaveToFile(std::string filePath);
	void LoadFromFile(std::string filePath);
	unsigned char GetNextIndexToAdd();
private:
	unsigned char m_nextIndexToAdd = 1;
	BlockTypeDescription m_blockTypes[256];
};


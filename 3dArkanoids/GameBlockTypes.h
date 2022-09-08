#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "ISerializable.h"


class BlockAppearance
{
public:
	glm::vec4 Colour;
	glm::vec2 AtlasUvOffset;
};

class BlockTypeDescription{
public:
	// will also contain gamplay characteristics of the block
	BlockAppearance Appearance;
};

class GameBlockTypes : ISerializable
{
public:
	BlockTypeDescription& operator[](unsigned char b);
	void Add(const BlockTypeDescription& blockTypeDescription);
	void Clear();
	virtual void SaveToFile(std::string filePath) const override;
	virtual void LoadFromFile(std::string filePath) override;
	virtual const std::vector<SerializableProperty>& GetSerializableProperties() override;
	// Inherited via ISerializable
	virtual int GetNumSerializableProperties() const override;

	unsigned char GetNextIndexToAdd();
private:
	unsigned char m_nextIndexToAdd = 1;
	BlockTypeDescription m_blockTypes[256];


	// Inherited via ISerializable
	virtual bool SetSerializableProperty(const SerializableProperty& p) override;



	// Inherited via ISerializable
};


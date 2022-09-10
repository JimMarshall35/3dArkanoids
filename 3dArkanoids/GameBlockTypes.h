#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <functional>
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

using SetColourFunc = std::function<void(const glm::vec4&)>;
using SetUvOffsetFunc = std::function<void(const glm::vec2&)>;

class BlockTypeSerializableNode : public ISerializablePropertiesNode {
public:
	BlockTypeSerializableNode() {}
	BlockTypeSerializableNode(const glm::vec4& m_colour, const glm::vec2& uvOffset, SetColourFunc setColourFunc, SetUvOffsetFunc setUvOffsetFunc);
	virtual const std::vector<SerializableProperty>& GetSerializableProperties() const override;
	virtual bool SetSerializableProperty(const SerializableProperty& p) override;
	virtual int GetNumSerializableProperties() const override;
private:
	std::vector<SerializableProperty> m_properties;
	std::function<void(const glm::vec4&)> m_setColourFunc;
	std::function<void(const glm::vec2&)> m_setUvOffsetFunc;
};

class GameBlockTypes : ISerializable
{
public:
	BlockTypeDescription& operator[](unsigned char b);
	void Add(const BlockTypeDescription& blockTypeDescription);
	void Clear();
	virtual void SaveToFile(std::string filePath) const override;
	virtual void LoadFromFile(std::string filePath) override;
	virtual const std::vector<SerializableProperty>& GetSerializableProperties() const override;
	// Inherited via ISerializable
	virtual int GetNumSerializableProperties() const override;

	unsigned char GetNextIndexToAdd();
	void InitialiseSerializablePropertiesArray();
private:
	unsigned char m_nextIndexToAdd = 1;
	BlockTypeDescription m_blockTypes[256];
	std::vector<SerializableProperty> m_serializableProperties;
	std::vector<BlockTypeSerializableNode> m_blockTypeSerializableNodes;



	// Inherited via ISerializable
	virtual bool SetSerializableProperty(const SerializableProperty& p) override;



	// Inherited via ISerializable
};


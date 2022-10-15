#include "GameBlockTypes.h"
#include <fstream>


BlockTypeDescription& GameBlockTypes::operator[](unsigned char b)
{
	return m_blockTypes[b];
}

void GameBlockTypes::Add(const BlockTypeDescription& blockTypeDescription)
{
	m_blockTypes[m_nextIndexToAdd++] = blockTypeDescription;
}

void GameBlockTypes::Clear()
{
	m_nextIndexToAdd = 1;
}

#include <iostream>
void GameBlockTypes::SaveToFile(std::string filePath) const
{
	std::ofstream file(filePath, std::ios::out | std::ios::binary);
	auto bufferSize = (m_nextIndexToAdd - 1) * sizeof(BlockTypeDescription) + sizeof(int); // + sizeof(int) for an int at the start signifying the number of game block types
	auto fileBuffer = std::make_unique<char[]>(bufferSize);
	SaveToBuffer(fileBuffer.get());
	file.write(
		fileBuffer.get(), // start at index 1 as index 0 is no block
		bufferSize);
}

void GameBlockTypes::LoadFromFile(std::string filePath)
{
	std::ifstream is(filePath, std::ios::in | std::ios::binary);
	// get length
	is.seekg(0, is.end);
	auto fileLen = is.tellg();
	is.seekg(0, is.beg);
	assert(fileLen <= sizeof(m_blockTypes));

	auto fileBuffer = std::make_unique<char[]>(fileLen);

	is.read(fileBuffer.get(), fileLen);

	
	LoadFromBuffer(fileBuffer.get());

	
	
	const auto& blockTypes = GetSerializableProperties()[0];

}

char* GameBlockTypes::SaveToBuffer(char* destination) const
{
	auto numBlockTypes = m_nextIndexToAdd - 1;
	auto writePtr = destination;
	memcpy(writePtr, &numBlockTypes, sizeof(int));
	writePtr += sizeof(int);
	for (int i = 1; i <= numBlockTypes; i++) {
		const BlockTypeDescription& type = m_blockTypes[i];
		memcpy(writePtr, &type.Appearance.AtlasUvOffset, sizeof(glm::vec2));
		writePtr += sizeof(glm::vec2);
		memcpy(writePtr, &type.Appearance.Colour, sizeof(glm::vec4));
		writePtr += sizeof(glm::vec4);
	}
	//memcpy(writePtr, (const char*)&m_blockTypes[1], numBlockTypes * sizeof(BlockTypeDescription));
	//writePtr += numBlockTypes * sizeof(BlockTypeDescription);
	return writePtr;
}

const char* GameBlockTypes::LoadFromBuffer(const char* source)
{
	auto numBlockTypes = *((int*)source);
	m_nextIndexToAdd = numBlockTypes + 1;
	auto readPtr = source + sizeof(int);
	for (int i = 1; i <= numBlockTypes; i++) {
		const BlockTypeDescription& type = *((BlockTypeDescription*)readPtr);
		m_blockTypes[i].Appearance.AtlasUvOffset = type.Appearance.AtlasUvOffset;
		m_blockTypes[i].Appearance.Colour = type.Appearance.Colour;
		readPtr += sizeof(BlockTypeDescription);
	}
	InitialiseSerializablePropertiesArray();
	//memcpy(&m_blockTypes[1], source + sizeof(int), numBlockTypes * sizeof(BlockTypeDescription));
	return readPtr;
}

unsigned char GameBlockTypes::GetNextIndexToAdd()
{
	return m_nextIndexToAdd;
}

void GameBlockTypes::InitialiseSerializablePropertiesArray()
{
	for (int i = 0; i < m_nextIndexToAdd - 1; i++) {
		auto& type = m_blockTypes[i + 1];
		auto node = BlockTypeSerializableNode(type.Appearance.Colour,type.Appearance.AtlasUvOffset,
			[this, i](const auto& vec) {m_blockTypes[i + 1].Appearance.Colour = vec; },
			[this, i](const auto& vec) {
				m_blockTypes[i + 1].Appearance.AtlasUvOffset = vec; });
		m_blockTypeSerializableNodes.push_back(node);
	}

	SerializableProperty prop;
	m_serializableProperties.clear();
	m_serializableProperties.push_back(prop);
	auto& p = m_serializableProperties.back();
	p.name = "blocktypes";
	p.type = SerializablePropertyType::SerializableNodesArray;
	p.data.SizeIfApplicable = m_blockTypeSerializableNodes.size();
	auto alloced = new ISerializablePropertiesNode * [p.data.SizeIfApplicable];
	p.data.dataUnion.childNodes = alloced;
	for (int i = 0; i < p.data.SizeIfApplicable; i++) {
		p.data.dataUnion.childNodes[i] = &m_blockTypeSerializableNodes[i];
	}



}

bool GameBlockTypes::SetSerializableProperty(const SerializableProperty& p)
{
	return false;
}

int GameBlockTypes::GetNumSerializableProperties() const
{
	return 1;
}

std::string GameBlockTypes::GetSerializableNodeName() const
{
	return "GameBlockTypes";
}

size_t GameBlockTypes::GetBinaryFileNumBytes() const
{
	return (m_nextIndexToAdd - 1) * sizeof(BlockTypeDescription) + sizeof(int);
}


const std::vector<SerializableProperty>& GameBlockTypes::GetSerializableProperties() const
{

	return m_serializableProperties;
}


const std::vector<SerializableProperty>& BlockTypeSerializableNode::GetSerializableProperties() const
{
	return m_properties;
}

BlockTypeSerializableNode::BlockTypeSerializableNode(const glm::vec4& colour, const glm::vec2& uvOffset, SetColourFunc setColourFunc, SetUvOffsetFunc setUvOffsetFunc)
{
	m_setColourFunc = setColourFunc;
	m_setUvOffsetFunc = setUvOffsetFunc;

	m_properties = std::vector<SerializableProperty>(2);
	m_properties[0].name = "colour";
	m_properties[0].type = SerializablePropertyType::Vec4;
	m_properties[0].data.dataUnion.Vec4 = colour;

	m_properties[1].name = "uvoffset";
	m_properties[1].type = SerializablePropertyType::Vec2;
	m_properties[1].data.dataUnion.Vec2 = uvOffset;
}

bool BlockTypeSerializableNode::SetSerializableProperty(const SerializableProperty& p)
{
	if (p.name == "colour") {
		if (p.type != SerializablePropertyType::Vec4) {
			return false;
		}
		
		m_properties[0].data.dataUnion.Vec4 = p.data.dataUnion.Vec4;
		m_setColourFunc(p.data.dataUnion.Vec4);
		return true;
	}
	else if (p.name == "uvoffset") {
		if (p.type != SerializablePropertyType::Vec2) {
			return false;
		}
		m_properties[1].data.dataUnion.Vec2 = p.data.dataUnion.Vec2;
		m_setUvOffsetFunc(p.data.dataUnion.Vec2);
		return true;
	}

	return false;
}

int BlockTypeSerializableNode::GetNumSerializableProperties() const
{
	return 2;
}

std::string BlockTypeSerializableNode::GetSerializableNodeName() const
{
	return "BlockTypeSerializableNode";
}

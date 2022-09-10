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
	file.write(
		(const char*)&m_blockTypes[1], // start at index 1 as index 0 is no block
		(m_nextIndexToAdd-1)*sizeof(BlockTypeDescription));
}

void GameBlockTypes::LoadFromFile(std::string filePath)
{
	std::ifstream is(filePath, std::ios::in | std::ios::binary);
	// get length
	is.seekg(0, is.end);
	int fileLength = is.tellg();
	is.seekg(0, is.beg);
	assert(fileLength <= sizeof(m_blockTypes));
	is.read((char*)&m_blockTypes[1], fileLength);
	m_nextIndexToAdd = (fileLength/ sizeof(BlockTypeDescription)) + 1;

	//for (int i = 1; i < m_nextIndexToAdd; i++) {
	//	const auto& colour = m_blockTypes[i].Appearance.Colour;
	//	std::cout << "{ " << colour.r << ", " << colour.g << ", " << colour.b << ", " << colour.a << " },\n";
	//}
	InitialiseSerializablePropertiesArray();
	
	const auto& blockTypes = GetSerializableProperties()[0];
	//auto numBlockTypes = blockTypes.data.SizeIfApplicable;
	//for (int i = 0; i < numBlockTypes; i++) {
	//	const auto blockType = blockTypes.data.dataUnion.childNodes[i];
	//	const auto& props = blockType->GetSerializableProperties();
	//	for (const auto& prop : props) {
	//		std::cout << prop.name << " type: " << (int)prop.type << "\n";
	//	}
	//	//const auto& blockTypeProps = blockType.GetSerializableProperties();
	//}
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
			[this, i](const auto& vec) {m_blockTypes[i + 1].Appearance.AtlasUvOffset = vec; });
		m_blockTypeSerializableNodes.push_back(node);
	}

	SerializableProperty prop;
	m_serializableProperties.push_back(prop);
	auto& p = m_serializableProperties.back();
	p.name = "block types";
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

	m_properties[1].name = "uv offset";
	m_properties[1].type = SerializablePropertyType::Vec2;
	m_properties[1].data.dataUnion.Vec2 = uvOffset;
}

bool BlockTypeSerializableNode::SetSerializableProperty(const SerializableProperty& p)
{
	if (p.name == "colour") {
		if (p.type != SerializablePropertyType::Vec4) {
			return false;
		}
		m_setColourFunc(p.data.dataUnion.Vec4);
		return true;
	}
	else if (p.name == "uv offset") {
		if (p.type != SerializablePropertyType::Vec2) {
			return false;
		}
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

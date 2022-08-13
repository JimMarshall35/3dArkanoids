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
}

unsigned char GameBlockTypes::GetNextIndexToAdd()
{
	return m_nextIndexToAdd;
}

bool GameBlockTypes::SetSerializableProperty(const SerializableProperty& p)
{
	return false;
}

int GameBlockTypes::GetNumSerializableProperties() const
{
	return 1;
}


const std::vector<SerializableProperty>& GameBlockTypes::GetSerializableProperties()
{
	std::vector<SerializableProperty> properties(1);
	properties[0].data.SizeIfApplicable = (m_nextIndexToAdd - 1) * sizeof(BlockTypeDescription);
	properties[0].data.dataUnion.Bytes = (char*)&m_blockTypes[1];
	properties[0].type = SerializablePropertyType::Bytes;
	properties[0].name = "BlockTypes";

	return properties;
}
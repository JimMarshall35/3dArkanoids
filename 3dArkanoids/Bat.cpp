#include "Bat.h"
#include "GameInput.h"
#include "IRenderer.h"
#include "PlayfieldDefs.h"
#include <iostream>
#include <format>
#include <fstream>
#include <memory>

Bat::Bat()
{
	//LoadFromFile("bat.jim");
}

double Bat::RecieveInput(const GameInput& input)
{
	auto oldX = m_xPos;
	m_xPos += input.XChange * m_sensitivity;
	if (m_xPos > m_xMaxPos) {
		m_xPos = m_xMaxPos;
	}
	else if (m_xPos < m_xMinPos) {
		m_xPos = m_xMinPos;
	}
	return m_xPos - oldX;
}

void Bat::SetMinAndMaxXPos(double xMin, double xMax)
{
	m_xMinPos = xMin + m_batWidth * 0.5f;
	m_xMaxPos = xMax - m_batWidth * 0.5f;
	m_xPos = m_xMinPos;
}

void Bat::Draw(const IRenderer* renderer, const Camera& cam) const
{
	renderer->DrawCuboid(
		{ m_xPos,-(m_distanceFromFirstRow + BLOCK_HEIGHT_UNITS * 0.5f),0 },
		{ m_batWidth,m_batDepthAndHeight.x,m_batDepthAndHeight.y },
		cam,
		{ 1.0,0.4,1.0 });
}


void Bat::SaveToFile(std::string path) const
{
	const auto fileSize = 2 * sizeof(float) + sizeof(double);

	std::ofstream file(path, std::ios::out | std::ios::binary);
	char stagingBuffer[fileSize];
	SaveToBuffer(stagingBuffer);

	file.write(stagingBuffer, sizeof(stagingBuffer));
}

void Bat::LoadFromFile(std::string path)
{
	const auto expectedFileSize = 2 * sizeof(float) + sizeof(double);

	std::ifstream is(path, std::ios::in | std::ios::binary);
	// get length
	is.seekg(0, is.end);
	int fileLength = is.tellg();
	is.seekg(0, is.beg);
	assert(fileLength <= expectedFileSize);
	char stagingBuffer[expectedFileSize];
	is.read(stagingBuffer, expectedFileSize);

	LoadFromBuffer(stagingBuffer);
}

char* Bat::SaveToBuffer(char* destination) const
{
	memcpy(destination, &m_batWidth, sizeof(float));
	memcpy(destination + sizeof(float), &m_distanceFromFirstRow, sizeof(float));
	memcpy(destination + 2 * sizeof(float), &m_sensitivity, sizeof(double));
	return destination + 3 * sizeof(float);
}

const char* Bat::LoadFromBuffer(const char* source)
{
	m_batWidth = *((float*)source);
	m_distanceFromFirstRow = *((float*)&source[sizeof(float)]);
	m_sensitivity = *((double*)&source[2 * sizeof(float)]);
	return source + 3 * sizeof(float);
}

const std::vector<SerializableProperty>& Bat::GetSerializableProperties() const
{
	static std::vector<SerializableProperty> props(NUM_BAT_SERIALIZABLE_PROPERTIES);
	props[0].name = "BatWidth";
	props[0].type = SerializablePropertyType::Float;
	props[0].data.dataUnion.Float = m_batWidth;

	props[1].name = "DistanceFromFirstRow";
	props[1].type = SerializablePropertyType::Float;
	props[1].data.dataUnion.Float = m_distanceFromFirstRow;

	props[2].name = "Sensitivity";
	props[2].type = SerializablePropertyType::Double;
	props[2].data.dataUnion.Float = m_sensitivity;

	return props;

}

bool Bat::SetSerializableProperty(const SerializableProperty& p)
{
	if (p.name == "BatWidth") {
		m_batWidth = p.data.dataUnion.Float;
		//SetMinAndMaxXPos(0.0 - BLOCK_WIDTH_UNITS * 0.5f, ((w * BLOCK_WIDTH_UNITS) - BLOCK_WIDTH_UNITS) + BLOCK_WIDTH_UNITS * 0.5f);
		return true;
	}
	else if (p.name == "DistanceFromFirstRow") {
		m_batWidth = p.data.dataUnion.Float;
		return true;
	}
	else if (p.name == "Sensitivity") {
		m_sensitivity = p.data.dataUnion.Double;
		return true;
	}
	return false;
}

int Bat::GetNumSerializableProperties() const
{
	return NUM_BAT_SERIALIZABLE_PROPERTIES;
}

std::string Bat::GetSerializableNodeName() const
{
	return "Bat";
}

size_t Bat::GetBinaryFileNumBytes() const
{
	return 2 * sizeof(float) + sizeof(double);
}

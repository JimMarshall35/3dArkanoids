#include "ISerializable.h"
#include <iostream>
#include <fstream>


void DebugPrintSerializablePropertiesNode(const ISerializablePropertiesNode* node, int numTabs = 0) {
	for (int i = 0; i < numTabs; i++) {
		std::cout << "\t";
	}
	std::cout << "serializable thing " << node->GetSerializableNodeName() << "\n";
	for (const auto& prop : node->GetSerializableProperties()) {
		for (int i = 0; i < numTabs + 1; i++) {
			std::cout << "\t";
		}
		std::cout << prop.name << ": ";
		switch (prop.type) {
		case SerializablePropertyType::Double:
			std::cout << prop.data.dataUnion.Double << "\n";
			break;
		case SerializablePropertyType::Float:
			std::cout << prop.data.dataUnion.Float << "\n";
			break;
		case SerializablePropertyType::Int16:
			std::cout << prop.data.dataUnion.Int16 << "\n";
			break;
		case SerializablePropertyType::Int32:
			std::cout << prop.data.dataUnion.Int32 << "\n";
			break;
		case SerializablePropertyType::Int8:
			std::cout << prop.data.dataUnion.Int8 << "\n";
			break;
		case SerializablePropertyType::Uint16:
			std::cout << prop.data.dataUnion.Uint16 << "\n";
			break;
		case SerializablePropertyType::Uint32:
			std::cout << prop.data.dataUnion.Uint32 << "\n";
			break;
		case SerializablePropertyType::Uint8:
			std::cout << prop.data.dataUnion.Uint8 << "\n";
			break;
		case SerializablePropertyType::Vec2:
			std::cout << "{ x: " << prop.data.dataUnion.Vec2.x << " y: " << prop.data.dataUnion.Vec2.y << " }\n";
			break;
		case SerializablePropertyType::Vec3:
			std::cout << "{ x: " << prop.data.dataUnion.Vec3.x << " y: " << prop.data.dataUnion.Vec3.y << " z: " << prop.data.dataUnion.Vec3.z << " }\n";
			break;
		case SerializablePropertyType::Vec4:
			std::cout << "{ x: " << prop.data.dataUnion.Vec4.x << " y: " << prop.data.dataUnion.Vec4.y << " z: " << prop.data.dataUnion.Vec4.z << " a: " << prop.data.dataUnion.Vec4.a << " }\n";
			break;
		case SerializablePropertyType::Bytes:
			std::cout << "[bytes]\n";
			break;
		case SerializablePropertyType::SerializableNode:
			std::cout << "\n";
			DebugPrintSerializablePropertiesNode(prop.data.dataUnion.childNodes[0], numTabs + 1);
			break;
		case SerializablePropertyType::SerializableNodesArray:
			//std::cout << "[SerializableNodesArray]\n";
			std::cout << "\n";
			auto numProperties = prop.data.SizeIfApplicable;
			for (int i = 0; i < numProperties; i++) {
				const auto child = prop.data.dataUnion.childNodes[i];
				DebugPrintSerializablePropertiesNode(child,numTabs + 1);
			}
			break;
		}
	}
}

void DebugPrintAllSerializableThings()
{
	const auto& list = AutoList<ISerializable>::GetList();
	for (const auto item : list) {
		//std::cout << "serializable thing " << item->GetSerializableNodeName() << "\n";
		DebugPrintSerializablePropertiesNode(item);
	}
}

size_t getBigFileRequiredSize() {
	size_t size = 0;
	const auto& list = AutoList<ISerializable>::GetList();
	for (const auto item : list) {
		//std::cout << "serializable thing " << item->GetSerializableNodeName() << "\n";
		size += item->GetBinaryFileNumBytes();
	}
	return size;
}

void SaveSerializableToSingleBigBinary(std::string filePath)
{
	auto bufferSize = getBigFileRequiredSize();

	auto stagingBuffer = std::make_unique<char[]>(bufferSize);

	auto writePtr = stagingBuffer.get();

	const auto& list = AutoList<ISerializable>::GetList();
	for (const auto item : list) {
		writePtr = item->SaveToBuffer(writePtr);
	}

	std::ofstream file(filePath, std::ios::out | std::ios::binary);
	file.write(stagingBuffer.get(), bufferSize);


}


void LoadSerializableFromSingleBigBinary(std::string filePath)
{

	// open file
	std::ifstream is(filePath, std::ios::in | std::ios::binary);

	// get length
	is.seekg(0, is.end);
	int fileLength = is.tellg();
	is.seekg(0, is.beg);

	// allocate buffer for the entire files contents and write files contents into it
	auto inputBuf = std::make_unique<char[]>(fileLength);
	is.read(inputBuf.get(), fileLength);

	const char* readPtr = inputBuf.get();
	const auto& list = AutoList<ISerializable>::GetList();
	for (auto item : list) {
		readPtr = item->LoadFromBuffer(readPtr);
	}


}

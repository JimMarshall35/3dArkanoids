#include "ISerializable.h"
#include <iostream>


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
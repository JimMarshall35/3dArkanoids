#include "ISerializable.h"
#include <iostream>

void DebugPrintAllSerializableThings()
{
	const auto& list = AutoList<ISerializable>::GetList();
	auto onItem = 0;
	for (const auto& item : list) {
		std::cout << "serializable thing " << onItem++ << "\n";
		for (const auto& prop : item->GetSerializableProperties()) {
			std::cout << "\t" << prop.name << ": ";
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
				std::cout << "x: " << prop.data.dataUnion.Vec2.x << " y: " << prop.data.dataUnion.Vec2.y << "\n";
				break;
			case SerializablePropertyType::Vec3:
				std::cout << "x: " << prop.data.dataUnion.Vec3.x << " y: " << prop.data.dataUnion.Vec3.y << " z: " << prop.data.dataUnion.Vec3.z << "\n";
				break;
			case SerializablePropertyType::Vec4:
				std::cout << "x: " << prop.data.dataUnion.Vec4.x << " y: " << prop.data.dataUnion.Vec4.y << " z: " << prop.data.dataUnion.Vec4.z << " a: " << prop.data.dataUnion.Vec4.a << "\n";
				break;
			case SerializablePropertyType::Bytes:
				std::cout << "[bytes]\n";
				break;
			case SerializablePropertyType::SerializableNode:
				std::cout << "[serializableNode]\n";
				break;
			case SerializablePropertyType::SerializableNodesArray:
				std::cout << "[SerializableNodesArray]\n";
				break;
			}
		}
	}
}
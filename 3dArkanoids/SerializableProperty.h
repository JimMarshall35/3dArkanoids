#pragma once
#include "SerializablePropertyType.h"
#include <glm/glm.hpp>
#include <string>

class ISerializablePropertiesNode;
union SerializablePropertyData {
	unsigned int Uint32;
	unsigned short Uint16;
	unsigned char Uint8;
	int Int32;
	short Int16;
	char Int8;
	float Float;
	double Double;
	char* Bytes;
	glm::vec2 Vec2;
	glm::vec3 Vec3;
	glm::vec4 Vec4;
	ISerializablePropertiesNode** childNodes;

};

struct SizedSerializablePropertyData {
	SerializablePropertyData dataUnion;
	int SizeIfApplicable;
};

enum class EditorUIElementType {
	None = 0,
	TextBox = 1,
	Slider = 2,
	ColourPicker = 4,
};

struct EditorUISpecification {
	EditorUIElementType type = EditorUIElementType::None;
	double minSliderValue;
	double maxSliderValue;
	double stepSize;
};

struct SerializableProperty {
	std::string name;
	SerializablePropertyType type;
	SizedSerializablePropertyData data;
	EditorUISpecification uiElementSpec;
	~SerializableProperty() {
		switch (type) {
		case SerializablePropertyType::SerializableNode:
			delete data.dataUnion.childNodes;
			break;
		case SerializablePropertyType::SerializableNodesArray:
			delete[] data.dataUnion.childNodes;
			break;
		}
	}
};
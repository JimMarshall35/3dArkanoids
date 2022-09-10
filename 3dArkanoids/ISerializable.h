#pragma once

#include "AutoList.h"
#include <string>
#include <glm/glm.hpp>
#include <vector>

enum class SerializablePropertyType {
	Uint32,
	Uint16,
	Uint8,
	Int32,
	Int16,
	Int8,
	Float,
	Double,
	Bytes,
	Vec2,
	Vec3,
	Vec4,
	SerializableNode,
	SerializableNodesArray
};

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


struct SerializableProperty {
	std::string name;
	SerializablePropertyType type;
	SizedSerializablePropertyData data;
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

class ISerializablePropertiesNode {
public:
	virtual const std::vector<SerializableProperty>& GetSerializableProperties() const = 0;
	virtual bool SetSerializableProperty(const SerializableProperty& p) =  0;
	virtual int GetNumSerializableProperties() const = 0;
};

class ISerializable :public ISerializablePropertiesNode, public AutoList<ISerializable> {
public:
	virtual void SaveToFile(std::string filePath) const = 0;
	virtual void LoadFromFile(std::string filePath) = 0;
protected:
	ISerializable()
		:AutoList<ISerializable>() {
	}
	ISerializable(AUTOLIST_CTOR_OPTIONS options)
		:AutoList<ISerializable>(options) {
		// ctor to be used when you don't want to be auto-listed
	}
};

void DebugPrintAllSerializableThings();